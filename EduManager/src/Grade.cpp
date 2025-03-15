#include "Grade.h"
#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <string>

Grade::Grade(int studentId, int courseId, double value, const std::string& gradeType)
    : studentId(studentId), courseId(courseId), value(value), gradeType(gradeType) {
}

void Grade::setStudentId(int newStudentId) {
    studentId = newStudentId;
}

void Grade::setCourseId(int newCourseId) {
    courseId = newCourseId;
}

void Grade::setValue(double newValue) {
    value = newValue;
}

void Grade::setGradeType(const std::string& newGradeType) {
    gradeType = newGradeType;
}

bool Grade::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO grades(student_id, course_id, value, grade_type) VALUES($1, $2, $3, $4) RETURNING id;";

    std::string studentIdStr = std::to_string(studentId);
    std::string courseIdStr = std::to_string(courseId);
    std::string valueStr = std::to_string(value);
    const char* paramValues[4] = { studentIdStr.c_str(), courseIdStr.c_str(), valueStr.c_str(), gradeType.c_str() };

    PGresult* res = PQexecParams(conn, query, 4, nullptr, paramValues, nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_TUPLES_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

bool Grade::update() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE grades SET value = $1, grade_type = $2 WHERE student_id = $3 AND course_id = $4;";

    std::string valueStr = std::to_string(value);
    std::string studentIdStr = std::to_string(studentId);
    std::string courseIdStr = std::to_string(courseId);
    const char* paramValues[4] = { valueStr.c_str(), gradeType.c_str(), studentIdStr.c_str(), courseIdStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 4, nullptr, paramValues, nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

bool Grade::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM grades WHERE student_id = $1 AND course_id = $2;";

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

void Grade::printInfo() const {
    std::cout << "Grade: Student ID " << studentId << " Course ID " << courseId
        << " - Value: " << value << " Grade Type: " << gradeType << std::endl;
}