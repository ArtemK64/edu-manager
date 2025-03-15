#ifndef FACULTY_H
#define FACULTY_H

#include <string>

class Faculty {
private:
    int id;
    std::string name;

public:
    Faculty(int id, const std::string& name);

    int getId() const;
    std::string getName() const;

    void setId(int newId);
    void setName(const std::string& newName);

    bool create();
    bool update();
    bool remove();

    void printInfo() const;
};

#endif