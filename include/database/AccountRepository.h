#pragma once
#include "database/Repository.h"
#include "core/Account.h"

class AccountRepository : public Repository<Account>
{
public:
    void save(std::shared_ptr<Account> account) override;
    std::shared_ptr<Account> findById(const std::string &id) override;
    std::vector<std::shared_ptr<Account>> findAll() override;
    void remove(const std::string &id) override;
};