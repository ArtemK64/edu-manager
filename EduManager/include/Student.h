#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"

class Student : public Person {
private:
    std::string studentId;

public:
    Student(int id, const std::string& firstName, const std::string& middleName, const std::string& lastName, int age, const std::string& email, const std::string& studentId);

    void setStudentId(const std::string& newStudentId);

    bool create() override;
    bool update() override;
    bool remove() override;

    void printInfo() const override;
};

#endif