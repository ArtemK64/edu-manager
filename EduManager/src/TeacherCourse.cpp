#include "TeacherCourse.h"
#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <string>

TeacherCourse::TeacherCourse(int teacherId, int courseId)
    : teacherId(teacherId), courseId(courseId) {
}

void TeacherCourse::setTeacherId(int newTeacherId) {
    teacherId = newTeacherId;
}

void TeacherCourse::setCourseId(int newCourseId) {
    courseId = newCourseId;
}

bool TeacherCourse::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO teacher_courses(teacher_id, course_id) VALUES($1, $2);";

    std::string teacherIdStr = std::to_string(teacherId);
    std::string courseIdStr = std::to_string(courseId);
    const char* paramValues[2] = { teacherIdStr.c_str(), courseIdStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 2, nullptr, paramValues, nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

bool TeacherCourse::update() { return true; }

bool TeacherCourse::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM teacher_courses WHERE teacher_id = $1 AND course_id = $2;";

    std::string teacherIdStr = std::to_string(teacherId);
    std::string courseIdStr = std::to_string(courseId);
    const char* paramValues[2] = { teacherIdStr.c_str(), courseIdStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 2, nullptr, paramValues, nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

void TeacherCourse::printInfo() const {
    std::cout << "Преподаватель ID " << teacherId << " на курс ID " << courseId << std::endl;
}