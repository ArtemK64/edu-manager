#include "StudentDepartment.h"
#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <string>

StudentDepartment::StudentDepartment(int studentId, int departmentId)
    : studentId(studentId), departmentId(departmentId) {
}

void StudentDepartment::setStudentId(int newStudentId) {
    studentId = newStudentId;
}

void StudentDepartment::setDepartmentId(int newDepartmentId) {
    departmentId = newDepartmentId;
}

bool StudentDepartment::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO student_departments(student_id, department_id) VALUES($1, $2);";

    std::string studentIdStr = std::to_string(studentId);
    std::string departmentIdStr = std::to_string(departmentId);
    const char* paramValues[2] = { studentIdStr.c_str(), departmentIdStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 2, nullptr, paramValues, nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

bool StudentDepartment::update() { return true; }

bool StudentDepartment::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM student_departments WHERE student_id = $1 AND department_id = $2;";

    std::string studentIdStr = std::to_string(studentId);
    std::string departmentIdStr = std::to_string(departmentId);
    const char* paramValues[2] = { studentIdStr.c_str(), departmentIdStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 2, nullptr, paramValues, nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

void StudentDepartment::printInfo() const {
    std::cout << "Студент ID " << studentId << " на кафедре ID " << departmentId << std::endl;
}