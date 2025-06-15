#include "core/Account.h"
#include "financial/Wallet.h"
#include "contracts/Contract.h"
#include <stdexcept>

namespace market::core
{

    IDGenerator Account::idGen_{"ACC", 9};

    std::shared_ptr<Account> Account::create(const std::string &name, AccountType type)
    {
        if (name.empty())
        {
            throw std::invalid_argument("Account name cannot be empty");
        }
        return std::shared_ptr<Account>(new Account(idGen_.next(), name, type));
    }

    Account::Account(const std::string &id, const std::string &name, AccountType type)
        : id_(id), name_(name), type_(type)
    {
        if (id.empty())
        {
            throw std::invalid_argument("Account ID cannot be empty");
        }
        if (name.empty())
        {
            throw std::invalid_argument("Account name cannot be empty");
        }
    }

    void Account::addWallet(std::shared_ptr<market::financial::Wallet> wallet)
    {
        if (!wallet)
        {
            throw std::invalid_argument("Wallet cannot be null");
        }
        if (wallets_.find(wallet->getId()) != wallets_.end())
        {
            throw std::runtime_error("Wallet with ID " + wallet->getId() + " already exists");
        }
        wallets_[wallet->getId()] = wallet;
    }

    std::shared_ptr<market::financial::Wallet> Account::getWallet(const std::string &walletId) const
    {
        auto it = wallets_.find(walletId);
        return it != wallets_.end() ? it->second : nullptr;
    }

    void Account::addContract(std::shared_ptr<market::contracts::Contract> contract)
    {
        if (!contract)
        {
            throw std::invalid_argument("Contract cannot be null");
        }
        if (contracts_.find(contract->getId()) != contracts_.end())
        {
            throw std::runtime_error("Contract with ID " + contract->getId() + " already exists");
        }
        contracts_[contract->getId()] = contract;
    }

    std::shared_ptr<market::contracts::Contract> Account::getContract(const std::string &contractId) const
    {
        auto it = contracts_.find(contractId);
        return it != contracts_.end() ? it->second : nullptr;
    }

    Decimal Account::getBalance() const
    {
        Decimal balance(0);
        for (const auto &wallet : wallets_)
        {
            balance = balance + wallet.second->getNetWorth();
        }
        switch (type_)
        {
        case AccountType::ASSET:
        case AccountType::EXPENSE:
            return balance;
        case AccountType::LIABILITY:
        case AccountType::EQUITY:
        case AccountType::REVENUE:
            return Decimal(0) - balance;
        default:
            throw std::runtime_error("Unknown account type");
        }
    }

} // namespace market::core