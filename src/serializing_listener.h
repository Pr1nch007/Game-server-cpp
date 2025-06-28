#pragma once

#include <filesystem>
#include <chrono>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "application.h"
#include "serialization.h"

namespace fs = std::filesystem;

namespace infrastructure {

class SerializingListener : public app::ApplicationListener {
public:
    SerializingListener(fs::path state_file, std::chrono::milliseconds save_period, 
        players::Players& players, model::Game& game)
    : state_file_(std::move(state_file)),
    save_period_(save_period),
    players_(players),
    game_(game){}

    void OnTick(std::chrono::milliseconds delta) override {
        if (save_period_.count() > 0) {
            time_since_last_save_ += delta;
            if (time_since_last_save_ >= save_period_) {
                SaveState();
                time_since_last_save_ = std::chrono::milliseconds::zero();
            }
        }
    }

    void OnShutdown() override {
        if (!state_file_.empty()) {
            SaveState();
        }
    }

    bool TryRestoreState() {
        if (state_file_.empty() || !fs::exists(state_file_)) {
            return false;
        }

        try {
            std::ifstream ifs(state_file_);
            boost::archive::text_iarchive ia(ifs);
            
            serialization::GameSessionsAndPlayersRepr game_repr;
            ia >> game_repr;
            
            game_repr.Restore(game_, players_);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Failed to restore state: " << e.what() << std::endl;
            return false;
        }
    }
private:
    void SaveState() {
        if (state_file_.empty()) return;

        fs::path tmp_file = state_file_;
        tmp_file += ".tmp";

        try {
            std::ofstream ofs(tmp_file);
            boost::archive::text_oarchive oa(ofs);

            serialization::GameSessionsAndPlayersRepr game_repr(game_.GetSessions(), players_);
            oa << game_repr;

            ofs.close();
            fs::rename(tmp_file, state_file_);
        } catch (const std::exception& e) {
            std::cerr << "Failed to save state: " << e.what() << std::endl;
            fs::remove(tmp_file);
        }
    }
    fs::path state_file_;
    std::chrono::milliseconds save_period_;
    std::chrono::milliseconds time_since_last_save_{0};
    players::Players& players_;
    model::Game& game_;
    bool auto_save_ = false;
};

}   