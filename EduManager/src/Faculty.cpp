#include "Faculty.h"
#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <string>

Faculty::Faculty(int id, const std::string& name)
    : id(id), name(name) {
}

int Faculty::getId() const { return id; }

std::string Faculty::getName() const { return name; }

void Faculty::setId(int newId) {
    id = newId;
}

void Faculty::setName(const std::string& newName) {
    name = newName;
}

bool Faculty::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO faculties(name) VALUES($1) RETURNING id;";

    const char* paramValues[1] = { name.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        Logger::logError(PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    id = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return true;
}

bool Faculty::update() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE faculties SET name = $1 WHERE id = $2;";

    std::string idStr = std::to_string(id);
    const char* paramValues[2] = { name.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 2, nullptr, paramValues, nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

bool Faculty::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM faculties WHERE id = $1;";
    std::string idStr = std::to_string(id);
    const char* paramValues[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError(PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

void Faculty::printInfo() const {
    std::cout << "Faculty: " << name << " (ID: " << id << ")" << std::endl;
}