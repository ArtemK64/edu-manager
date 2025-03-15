#include "Database.h"
#include "Logger.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

Database::Database() {
    std::string dbName = getEnv("DB_NAME");
    std::string dbUser = getEnv("DB_USER");
    std::string dbPassword = getEnv("DB_PASSWORD");

    if (dbName.empty() || dbUser.empty() || dbPassword.empty()) {
        throw std::runtime_error("Error: Empty environment variables for connecting to the database");
    }

    std::string connStr = "dbname=" + dbName +
        " user=" + dbUser +
        " password=" + dbPassword +
        " host=localhost port=5432";

    conn = PQconnectdb(connStr.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Error connecting to the database: " << PQerrorMessage(conn) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (PQsetClientEncoding(conn, "UTF8") != 0) {
        std::cerr << "Error in setting UTF8 encoding: " << PQerrorMessage(conn) << std::endl;
    }

    std::cout << "Connection to the database is successful" << std::endl;
}

Database& Database::getInstance() {
    static Database instance;
    return instance;
}

PGconn* Database::getConnection() {
    if (conn == nullptr) {
        throw std::runtime_error("Error: connection to the database is not established");
    }
    return conn;
}

void Database::executeSQLFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: couldn't open the file " << filename << std::endl;
        return;
    }

    std::ostringstream sqlStream;
    sqlStream << file.rdbuf();
    std::string sqlQuery = sqlStream.str();

    if (sqlQuery.empty()) {
        std::cerr << "The SQL file is empty. Check the contents " << filename << std::endl;
        return;
    }

    PGresult* res = PQexec(conn, sqlQuery.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "SQL execution error: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return;
    }

    std::cout << "The database has been initialized successfully" << std::endl;
    PQclear(res);
}

bool Database::executeQuery(const std::string& query) {
    PGresult* res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError(PQerrorMessage(conn));
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

std::vector<std::vector<std::string>> Database::selectQuery(const std::string& query) {
    std::vector<std::vector<std::string>> results;
    PGresult* res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        Logger::logError(PQerrorMessage(conn));
        PQclear(res);
        return results;
    }
    int rows = PQntuples(res);
    int cols = PQnfields(res);
    for (int i = 0; i < rows; i++) {
        std::vector<std::string> row;
        for (int j = 0; j < cols; j++) {
            row.push_back(PQgetvalue(res, i, j));
        }
        results.push_back(row);
    }
    PQclear(res);
    return results;
}

Database::~Database() {
    if (conn) {
        PQfinish(conn);
    }
}