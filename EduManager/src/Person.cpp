#include "Person.h"
#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <string>

Person::Person(int id, const std::string& firstName, const std::string& middleName,
    const std::string& lastName, int age, const std::string& email)
    : id(id), firstName(firstName), middleName(middleName), lastName(lastName), age(age), email(email) {
}

int Person::getId() const { return id; }
std::string Person::getFirstName() const { return firstName; }
std::string Person::getMiddleName() const { return middleName; }
std::string Person::getLastName() const { return lastName; }
int Person::getAge() const { return age; }
std::string Person::getEmail() const { return email; }

void Person::setId(int newId) {
    id = newId;
}

void Person::setFirstName(const std::string& newFirstName) {
    firstName = newFirstName;
}

void Person::setMiddleName(const std::string& newMiddleName) {
    middleName = newMiddleName;
}

void Person::setLastName(const std::string& newLastName) {
    lastName = newLastName;
}

void Person::setAge(int newAge) {
    age = newAge;
}

void Person::setEmail(const std::string& newEmail) {
    email = newEmail;
}

int Person::createInPeople() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO people(first_name, middle_name, last_name, age, email) VALUES($1, $2, $3, $4, $5) RETURNING id;";

    std::vector<std::string> paramStrings = { firstName, middleName, lastName, std::to_string(age), email };
    std::vector<const char*> paramValues = {
        paramStrings[0].c_str(),
        paramStrings[1].c_str(),
        paramStrings[2].c_str(),
        paramStrings[3].c_str(),
        paramStrings[4].c_str()
    };

    PGresult* res = PQexecParams(conn, query, paramValues.size(), nullptr, paramValues.data(), nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        Logger::logError(PQerrorMessage(conn));
        PQclear(res);
        return -1;
    }

    if (PQntuples(res) == 0) {
        Logger::logError("Error: couldn't get ID from People");
        PQclear(res);
        return -1;
    }

    int newId = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return newId;
}

bool Person::updateInPeople() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE people SET first_name = $1, middle_name = $2, last_name = $3, age = $4, email = $5 WHERE id = $6;";

    std::vector<std::string> paramStrings = {
        firstName, middleName, lastName, std::to_string(age), email, std::to_string(id)
    };
    std::vector<const char*> paramValues = {
        paramStrings[0].c_str(),
        paramStrings[1].c_str(),
        paramStrings[2].c_str(),
        paramStrings[3].c_str(),
        paramStrings[4].c_str(),
        paramStrings[5].c_str()
    };

    PGresult* res = PQexecParams(conn, query, paramValues.size(), nullptr, paramValues.data(), nullptr, nullptr, 0);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);

    if (!success) {
        Logger::logError(PQerrorMessage(conn));
    }

    PQclear(res);
    return success;
}

bool Person::removeInPeople() {

    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM people WHERE id = $1;";

    std::string idStr = std::to_string(id);
    const char* paramValues[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError("SQL error: " + std::string(PQerrorMessage(conn)));
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}