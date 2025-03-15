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
        throw std::runtime_error("Ошибка: Пустые переменные окружения для подключения к БД!");
    }

    std::string connStr = "dbname=" + dbName +
        " user=" + dbUser +
        " password=" + dbPassword +
        " host=localhost port=5432";

    conn = PQconnectdb(connStr.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Ошибка подключения к БД: " << PQerrorMessage(conn) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (PQsetClientEncoding(conn, "UTF8") != 0) {
        std::cerr << "Ошибка установки кодировки UTF8: " << PQerrorMessage(conn) << std::endl;
    }

    std::cout << "Подключение к БД успешно!" << std::endl;
}

Database& Database::getInstance() {
    static Database instance;
    return instance;
}

PGconn* Database::getConnection() {
    if (conn == nullptr) {
        throw std::runtime_error("Ошибка: соединение с БД не установлено!");
    }
    return conn;
}

void Database::executeSQLFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return;
    }

    std::ostringstream sqlStream;
    sqlStream << file.rdbuf();
    std::string sqlQuery = sqlStream.str();

    if (sqlQuery.empty()) {
        std::cerr << "SQL-файл пуст! Проверьте содержимое " << filename << std::endl;
        return;
    }

    PGresult* res = PQexec(conn, sqlQuery.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Ошибка выполнения SQL: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return;
    }

    std::cout << "База данных успешно инициализирована!" << std::endl;
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