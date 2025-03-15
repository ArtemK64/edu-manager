#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course {
private:
    int id;
    std::string name;
    int teacherId;

public:
    Course(int id, const std::string& name, int teacherId);

    int getId() const;
    std::string getName() const;

    void setId(int newId);
    void setName(const std::string& newName);
    void setTeacherId(int newTeacherId);

    bool create();
    bool update();
    bool remove();

    void printInfo() const;
};

#endif