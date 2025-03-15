#include "Teacher.h"
#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <string>

Teacher::Teacher(int id, const std::string& firstName, const std::string& middleName,
    const std::string& lastName, int age, const std::string& email,
    const std::string& employeeId)
    : Person(id, firstName, middleName, lastName, age, email), employeeId(employeeId) {
}

void Teacher::setEmployeeId(const std::string& newEmployeeId) {
    employeeId = newEmployeeId;
}

bool Teacher::create() {
    int newId = createInPeople();
    if (newId == -1)
        return false;
    id = newId;

    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO teachers(id, employee_id) VALUES($1, $2) RETURNING id;";

    std::vector<std::string> paramStrings = { std::to_string(id), employeeId };
    std::vector<const char*> paramValues = { paramStrings[0].c_str(), paramStrings[1].c_str() };

    PGresult* res = PQexecParams(conn, query, paramValues.size(), nullptr, paramValues.data(), nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        Logger::logError(PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

bool Teacher::update() {
    if (!updateInPeople()) {
        return false;
    }

    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE teachers SET employee_id = $1 WHERE id = $2;";

    std::vector<std::string> paramStrings = { employeeId, std::to_string(getId()) };
    std::vector<const char*> paramValues = {
        paramStrings[0].c_str(),
        paramStrings[1].c_str()
    };

    PGresult* res = PQexecParams(conn, query, paramValues.size(), nullptr, paramValues.data(), nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }
    PQclear(res);
    return success;
}

bool Teacher::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM teachers WHERE id = $1;";
    std::string idStr = std::to_string(getId());
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

void Teacher::printInfo() const {
    std::cout << "Teacher: " << getFirstName() << " " << getLastName()
        << " (Email: " << getEmail() << ", Employee ID: " << employeeId << ")" << std::endl;
}