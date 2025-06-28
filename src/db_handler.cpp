#include "db_handler.h"
#include "tagged_uuid.h"
#include <pqxx/pqxx>
#include <stdexcept>
#include <iostream>


DbHandler::DbHandler(const std::string& db_url, size_t pool_size)
    : pool_(pool_size, [db_url]() {
        try {
            auto conn = std::make_shared<pqxx::connection>(db_url);
            return conn;
        } catch (const pqxx::sql_error& e) {
            std::cerr << "Failed to create database connection: " << e.what() << std::endl;
            throw std::runtime_error("Database connection initialization failed: " + std::string(e.what()));
        } catch (const pqxx::broken_connection& e) {
            std::cerr << "Failed to create database connection: " << e.what() << std::endl;
            throw std::runtime_error("Database connection initialization failed: " + std::string(e.what()));
        }
    }) {}

void DbHandler::InitializeDatabase() {
    try {
        auto conn = pool_.GetConnection();
        pqxx::work txn(*conn);
        txn.exec0(
            "CREATE TABLE IF NOT EXISTS retired_players ("
            "id UUID PRIMARY KEY, "
            "name TEXT NOT NULL, "
            "score INTEGER NOT NULL, "
            "play_time_ms BIGINT NOT NULL"
            ")");
        txn.exec0(
            "CREATE INDEX IF NOT EXISTS idx_retired_players "
            "ON retired_players (score DESC, play_time_ms ASC, name ASC)");
        txn.commit();

        conn->prepare("insert_player",
                      "INSERT INTO retired_players (id, name, score, play_time_ms) "
                      "VALUES ($1, $2, $3, $4)");
        conn->prepare("select_records",
                      "SELECT name, score, play_time_ms FROM retired_players "
                      "ORDER BY score DESC, play_time_ms ASC, name ASC "
                      "LIMIT $1 OFFSET $2");
    } catch (const pqxx::sql_error& e) {
        std::cerr << "Failed to initialize database: " << e.what() << std::endl;
        throw std::runtime_error("Database initialization failed: " + std::string(e.what()));
    } catch (const pqxx::broken_connection& e) {
        std::cerr << "Failed to initialize database: " << e.what() << std::endl;
        throw std::runtime_error("Database initialization failed: " + std::string(e.what()));
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error during database initialization: " << e.what() << std::endl;
        throw std::runtime_error("Unexpected error during database initialization: " + std::string(e.what()));
    }
}   

void DbHandler::SaveRetiredPlayer(const RetiredPlayer& player) {
    try {
        auto conn = pool_.GetConnection();
        pqxx::work txn(*conn);

        util::TaggedUUID<RetiredPlayer> uuid = util::TaggedUUID<RetiredPlayer>::New();
        std::string uuid_str = uuid.ToString();

        txn.exec_prepared("insert_player", uuid_str, player.name, player.score, player.play_time_ms);
        txn.commit();
    } catch (const pqxx::sql_error& e) {
        std::cerr << "Failed to save retired player: " << e.what() << std::endl;
        throw std::runtime_error("Failed to save retired player: " + std::string(e.what()));
    } catch (const pqxx::broken_connection& e) {
        std::cerr << "Failed to save retired player: " << e.what() << std::endl;
        throw std::runtime_error("Failed to save retired player: " + std::string(e.what()));
    }
}

std::vector<RetiredPlayer> DbHandler::GetRecords(int start, int max_items) {
    try {
        auto conn = pool_.GetConnection();
        pqxx::work txn(*conn);
        auto result = txn.exec_prepared("select_records", max_items, start);

        std::vector<RetiredPlayer> records;
        records.reserve(result.size());
        for (const auto& row : result) {
            records.push_back({
                row[0].as<std::string>(),
                row[1].as<int>(),
                row[2].as<int64_t>()
            });
        }
        return records;
    } catch (const pqxx::sql_error& e) {
        std::cerr << "Failed to retrieve records: " << e.what() << std::endl;
        throw std::runtime_error("Failed to retrieve records: " + std::string(e.what()));
    } catch (const pqxx::broken_connection& e) {
        std::cerr << "Failed to retrieve records: " << e.what() << std::endl;
        throw std::runtime_error("Failed to retrieve records: " + std::string(e.what()));
    }
}