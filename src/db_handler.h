#pragma once

#include "db_connection_pool.h"
#include "tagged_uuid.h"
#include <string>
#include <vector>
#include <chrono>

struct RetiredPlayer {
    std::string name;
    int score;
    int64_t play_time_ms;
};

class DbHandler {
public:
    explicit DbHandler(const std::string& db_url, size_t pool_size = std::thread::hardware_concurrency());

    void InitializeDatabase();
    void SaveRetiredPlayer(const RetiredPlayer& player);
    std::vector<RetiredPlayer> GetRecords(int start, int max_items);

private:
    ConnectionPool pool_;
};