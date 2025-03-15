#include <iostream>
#include "Database.h"
#include "Course.h"
#include "Student.h"
#include "Teacher.h"
#include "Faculty.h"
#include "Department.h"
#include "Enrollment.h"
#include "Grade.h"
#include "StudentDepartment.h"
#include "TeacherDepartment.h"
#include "TeacherCourse.h"

int main() {
    try {
        setlocale(LC_ALL, "ru_RU.UTF-8");

        Database& db = Database::getInstance();
        db.executeSQLFile("./resources/schema.sql");

        Faculty faculty(0, "Computer Science");
        faculty.create();

        Department department(0, "Software Engineering", faculty.getId());
        department.create();

        department.setName("Software Engineering and Informatics");
        department.update();

        Teacher teacher(0, "Petr", "Petrovich", "Petrov", 40, "petr@example.com", "EMP98765");
        teacher.create();

        teacher.setLastName("Sidorov");
        teacher.update();

        Course course(0, "Algorithms", teacher.getId());
        course.create();

        course.setName("Algorithms and Data Structures");
        course.update();

        Student student(0, "Ivan", "Ivanovich", "Ivanov", 20, "ivan@example.com", "ST12345");
        student.create();

        student.setAge(21);
        student.update();

        Enrollment enrollment(student.getId(), course.getId());
        enrollment.create();

        setlocale(LC_NUMERIC, "C");
        Grade grade(student.getId(), course.getId(), 95.5, "Final Exam");
        grade.create();

        StudentDepartment studentDepartment(student.getId(), department.getId());
        studentDepartment.create();

        TeacherDepartment teacherDepartment(teacher.getId(), department.getId());
        teacherDepartment.create();

        TeacherCourse teacherCourse(teacher.getId(), course.getId());
        teacherCourse.create();

        teacherCourse.remove();
        teacherDepartment.remove();
        studentDepartment.remove();
        grade.remove();
        enrollment.remove();
        student.remove();
        course.remove();
        teacher.remove();
        department.remove();
        faculty.remove();

        teacher.removeInPeople();
        student.removeInPeople();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}