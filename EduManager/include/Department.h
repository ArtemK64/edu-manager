#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>

class Department {
private:
    int id;
    std::string name;
    int facultyId;

public:
    Department(int id, const std::string& name, int facultyId);

    int getId() const;
    std::string getName() const;

    void setId(int newId);
    void setName(const std::string& newName);
    void setFacultyId(int newFacultyId);

    bool create();
    bool update();
    bool remove();

    void printInfo() const;
};

#endif