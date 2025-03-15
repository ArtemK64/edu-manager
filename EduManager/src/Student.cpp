#include "Student.h"
#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <string>

Student::Student(int id, const std::string& firstName, const std::string& middleName,
    const std::string& lastName, int age, const std::string& email,
    const std::string& studentId)
    : Person(id, firstName, middleName, lastName, age, email), studentId(studentId) {
}

void Student::setStudentId(const std::string& newStudentId) {
    studentId = newStudentId;
}

bool Student::create() {
    int newId = createInPeople();
    if (newId == -1)
        return false;
    id = newId;

    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO students(id, student_id) VALUES($1, $2) RETURNING id;";

    std::vector<std::string> paramStrings = { std::to_string(id), studentId };
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

bool Student::update() {
    if (!updateInPeople()) {
        return false;
    }

    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE students SET student_id = $1 WHERE id = $2;";

    std::vector<std::string> paramStrings = { studentId, std::to_string(getId()) };
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

bool Student::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM students WHERE id = $1;";
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

void Student::printInfo() const {
    std::cout << "Student: " << getFirstName() << " " << getLastName()
        << " (Email: " << getEmail() << ", Student ID: " << studentId << ")" << std::endl;
}