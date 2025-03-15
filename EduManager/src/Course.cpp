#include "Course.h"
#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <string>

Course::Course(int id, const std::string& name, int teacherId)
    : id(id), name(name), teacherId(teacherId) {
}

int Course::getId() const { return id; }

std::string Course::getName() const { return name; }

void Course::setId(int newId) {
    id = newId;
}

void Course::setName(const std::string& newName) {
    name = newName;
}

void Course::setTeacherId(int newTeacherId) {
    teacherId = newTeacherId;
}

bool Course::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO courses(name, instructor_id) VALUES($1, $2) RETURNING id;";

    std::vector<std::string> paramStrings = { name, std::to_string(teacherId) };
    std::vector<const char*> paramValues = { paramStrings[0].c_str(), paramStrings[1].c_str() };

    PGresult* res = PQexecParams(conn, query, paramValues.size(), nullptr, paramValues.data(), nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        Logger::logError(PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    id = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return true;
}

bool Course::update() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE courses SET name = $1, instructor_id = $2 WHERE id = $3;";

    std::vector<std::string> paramStrings = { name, std::to_string(teacherId), std::to_string(id) };
    std::vector<const char*> paramValues = { paramStrings[0].c_str(), paramStrings[1].c_str(), paramStrings[2].c_str() };

    PGresult* res = PQexecParams(conn, query, paramValues.size(), nullptr, paramValues.data(), nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError(PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

bool Course::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM courses WHERE id = $1;";

    std::string idStr = std::to_string(id);
    const char* paramValues[] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError(PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

void Course::printInfo() const {
    std::cout << "Курс: " << name << " (Преподаватель ID: " << teacherId << ")" << std::endl;
}