#ifndef PERSON_H
#define PERSON_H

#include <string>
#include "Database.h"

class Person {
protected:
    int id;
    std::string firstName;
    std::string middleName;
    std::string lastName;
    int age;
    std::string email;

public:
    Person(int id, const std::string& firstName, const std::string& middleName,
        const std::string& lastName, int age, const std::string& email);
    virtual ~Person() = default;

    int getId() const;
    std::string getFirstName() const;
    std::string getMiddleName() const;
    std::string getLastName() const;
    int getAge() const;
    std::string getEmail() const;

    void setId(int newId);
    void setFirstName(const std::string& newFirstName);
    void setMiddleName(const std::string& newMiddleName);
    void setLastName(const std::string& newLastName);
    void setAge(int newAge);
    void setEmail(const std::string& newEmail);

    virtual int createInPeople();
    virtual bool updateInPeople();
    virtual bool removeInPeople();

    virtual bool create() = 0;
    virtual bool update() = 0;
    virtual bool remove() = 0;

    virtual void printInfo() const = 0;
};

#endif