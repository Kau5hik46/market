#pragma once
#include <string>
#include <memory>
#include <libpq-fe.h>
#include "core/Account.h"

class Database
{
public:
    Database(const std::string &host,
             const std::string &port,
             const std::string &dbname,
             const std::string &user,
             const std::string &password);
    ~Database();

    // Connection management
    void connect();
    void disconnect();
    bool isConnected() const;

    // Transaction management
    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

    // Account operations
    void saveAccount(const std::shared_ptr<Account> &account);
    std::shared_ptr<Account> loadAccount(const std::string &accountId);
    void deleteAccount(const std::string &accountId);

private:
    PGconn *conn_;
    std::string host_;
    std::string port_;
    std::string dbname_;
    std::string user_;
    std::string password_;

    void executeQuery(const std::string &query);
    std::string escapeString(const std::string &str);
};