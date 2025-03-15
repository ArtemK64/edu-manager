#include "Department.h"
#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <string>

Department::Department(int id, const std::string& name, int facultyId)
    : id(id), name(name), facultyId(facultyId) {
}

int Department::getId() const { return id; }

std::string Department::getName() const { return name; }

void Department::setId(int newId) {
    id = newId;
}

void Department::setName(const std::string& newName) {
    name = newName;
}

void Department::setFacultyId(int newFacultyId) {
    facultyId = newFacultyId;
}

bool Department::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO departments(name, faculty_id) VALUES($1, $2) RETURNING id;";

    std::vector<std::string> paramStrings = { name, std::to_string(facultyId) };
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

bool Department::update() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE departments SET name = $1, faculty_id = $2 WHERE id = $3;";

    std::string facultyIdStr = std::to_string(facultyId);
    std::string idStr = std::to_string(id);
    const char* paramValues[3] = { name.c_str(), facultyIdStr.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 3, nullptr, paramValues, nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

bool Department::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM departments WHERE id = $1;";
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

void Department::printInfo() const {
    std::cout << "Department: " << name << " (ID: " << id << ", Faculty ID: " << facultyId << ")" << std::endl;
}