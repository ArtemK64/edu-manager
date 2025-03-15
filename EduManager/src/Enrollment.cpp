#include "Enrollment.h"
#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <string>

Enrollment::Enrollment(int studentId, int courseId)
    : studentId(studentId), courseId(courseId) {
}

bool Enrollment::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO enrollments(student_id, course_id) VALUES($1, $2);";

    std::vector<std::string> paramStrings = { std::to_string(studentId), std::to_string(courseId) };
    std::vector<const char*> paramValues = { paramStrings[0].c_str(), paramStrings[1].c_str() };

    PGresult* res = PQexecParams(conn, query, paramValues.size(), nullptr, paramValues.data(), nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError(PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

bool Enrollment::update() { return true; }

bool Enrollment::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM enrollments WHERE student_id = $1 AND course_id = $2;";

    std::string studentIdStr = std::to_string(studentId);
    std::string courseIdStr = std::to_string(courseId);
    const char* paramValues[2] = { studentIdStr.c_str(), courseIdStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 2, nullptr, paramValues, nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

void Enrollment::printInfo() const {
    std::cout << "Student ID " << studentId << " Course ID " << courseId << std::endl;
}