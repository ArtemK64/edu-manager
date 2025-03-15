#include "TeacherDepartment.h"
#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <string>

TeacherDepartment::TeacherDepartment(int teacherId, int departmentId)
    : teacherId(teacherId), departmentId(departmentId) {
}

void TeacherDepartment::setTeacherId(int newTeacherId) {
    teacherId = newTeacherId;
}

void TeacherDepartment::setDepartmentId(int newDepartmentId) {
    departmentId = newDepartmentId;
}

bool TeacherDepartment::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO teacher_departments(teacher_id, department_id) VALUES($1, $2);";

    std::string teacherIdStr = std::to_string(teacherId);
    std::string departmentIdStr = std::to_string(departmentId);
    const char* paramValues[2] = { teacherIdStr.c_str(), departmentIdStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 2, nullptr, paramValues, nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

bool TeacherDepartment::update() {
    return true;
}

bool TeacherDepartment::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM teacher_departments WHERE teacher_id = $1 AND department_id = $2;";

    std::string teacherIdStr = std::to_string(teacherId);
    std::string departmentIdStr = std::to_string(departmentId);
    const char* paramValues[2] = { teacherIdStr.c_str(), departmentIdStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 2, nullptr, paramValues, nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

void TeacherDepartment::printInfo() const {
    std::cout << "Преподаватель ID " << teacherId << " на кафедре ID " << departmentId << std::endl;
}