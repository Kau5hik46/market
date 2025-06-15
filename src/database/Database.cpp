#include "database/Database.h"
#include "core/Account.h"
#include <sstream>
#include <stdexcept>

Database::Database(const std::string &host,
                   const std::string &port,
                   const std::string &dbname,
                   const std::string &user,
                   const std::string &password)
    : host_(host), port_(port), dbname_(dbname), user_(user), password_(password),
      conn_(nullptr)
{
}

Database::~Database()
{
    if (conn_)
    {
        disconnect();
    }
}

void Database::connect()
{
    if (conn_)
    {
        throw std::runtime_error("Already connected to database");
    }

    std::string conninfo = "host=" + host_ + " port=" + port_ +
                           " dbname=" + dbname_ + " user=" + user_ +
                           " password=" + password_;

    conn_ = PQconnectdb(conninfo.c_str());
    if (PQstatus(conn_) != CONNECTION_OK)
    {
        std::string error = PQerrorMessage(conn_);
        PQfinish(conn_);
        conn_ = nullptr;
        throw std::runtime_error("Failed to connect to database: " + error);
    }
}

void Database::disconnect()
{
    if (conn_)
    {
        PQfinish(conn_);
        conn_ = nullptr;
    }
}

bool Database::isConnected() const
{
    return conn_ != nullptr && PQstatus(conn_) == CONNECTION_OK;
}

void Database::beginTransaction()
{
    if (!isConnected())
    {
        throw std::runtime_error("Not connected to database");
    }
    executeQuery("BEGIN");
}

void Database::commitTransaction()
{
    if (!isConnected())
    {
        throw std::runtime_error("Not connected to database");
    }
    executeQuery("COMMIT");
}

void Database::rollbackTransaction()
{
    if (!isConnected())
    {
        throw std::runtime_error("Not connected to database");
    }
    executeQuery("ROLLBACK");
}

void Database::saveAccount(const std::shared_ptr<Account> &account)
{
    if (!isConnected())
    {
        throw std::runtime_error("Not connected to database");
    }
    if (!account)
    {
        throw std::invalid_argument("Account cannot be null");
    }

    std::stringstream ss;
    ss << "INSERT INTO accounts (id, name) VALUES ('"
       << escapeString(account->getId()) << "', '"
       << escapeString(account->getName()) << "')"
       << " ON CONFLICT (id) DO UPDATE SET name = '"
       << escapeString(account->getName()) << "'";

    executeQuery(ss.str());
}

std::shared_ptr<Account> Database::loadAccount(const std::string &accountId)
{
    if (!isConnected())
    {
        throw std::runtime_error("Not connected to database");
    }
    if (accountId.empty())
    {
        throw std::invalid_argument("Account ID cannot be empty");
    }

    std::stringstream ss;
    ss << "SELECT id, name FROM accounts WHERE id = '"
       << escapeString(accountId) << "'";

    PGresult *res = PQexec(conn_, ss.str().c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        std::string error = PQerrorMessage(conn_);
        PQclear(res);
        throw std::runtime_error("Failed to load account: " + error);
    }

    if (PQntuples(res) == 0)
    {
        PQclear(res);
        return nullptr;
    }

    std::string id = PQgetvalue(res, 0, 0);
    std::string name = PQgetvalue(res, 0, 1);
    PQclear(res);

    return std::make_shared<Account>(id, name);
}

void Database::deleteAccount(const std::string &accountId)
{
    if (!isConnected())
    {
        throw std::runtime_error("Not connected to database");
    }
    if (accountId.empty())
    {
        throw std::invalid_argument("Account ID cannot be empty");
    }

    std::stringstream ss;
    ss << "DELETE FROM accounts WHERE id = '"
       << escapeString(accountId) << "'";

    executeQuery(ss.str());
}

void Database::executeQuery(const std::string &query)
{
    PGresult *res = PQexec(conn_, query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::string error = PQerrorMessage(conn_);
        PQclear(res);
        throw std::runtime_error("Query execution failed: " + error);
    }
    PQclear(res);
}

std::string Database::escapeString(const std::string &str)
{
    char *escaped = PQescapeLiteral(conn_, str.c_str(), str.length());
    if (!escaped)
    {
        throw std::runtime_error("Failed to escape string: " + std::string(PQerrorMessage(conn_)));
    }
    std::string result(escaped);
    PQfreemem(escaped);
    return result;
}