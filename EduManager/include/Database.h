#ifndef DATABASE_H
#define DATABASE_H

#include <libpq-fe.h>
#include <string>
#include <vector>
#include <memory>

class Database {
public:
    static Database& getInstance();
    PGconn* getConnection();
    void executeSQLFile(const std::string& filename);

    bool executeQuery(const std::string& query);
    std::vector<std::vector<std::string>> selectQuery(const std::string& query);

private:
    Database();
    ~Database();

    PGconn* conn;
};

#endif