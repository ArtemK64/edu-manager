#ifndef TEACHER_H
#define TEACHER_H

#include "Person.h"

class Teacher : public Person {
private:
    std::string employeeId;

public:
    Teacher(int id, const std::string& firstName, const std::string& middleName, const std::string& lastName, int age, const std::string& email, const std::string& employeeId);

    void setEmployeeId(const std::string& newEmployeeId);

    bool create() override;
    bool update() override;
    bool remove() override;

    void printInfo() const override;
};

#endif