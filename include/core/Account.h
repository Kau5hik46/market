#pragma once

#include <string>
#include <memory>
#include <unordered_map>
// #include "financial/Wallet.h"
// #include "contracts/Contract.h"
// #include "financial/Asset.h"
// #include "financial/Liability.h"
#include "utils/IDGenerator.h"
#include "utils/Decimal.h"

namespace market
{
    namespace financial
    {
        class Wallet;
        class Asset;
        class Liability;
    }
    namespace contracts
    {
        class Contract;
    }
}

namespace market::core
{

    class Account
    {
    public:
        enum class AccountType
        {
            ASSET,
            LIABILITY,
            EQUITY,
            REVENUE,
            EXPENSE
        };

        static std::shared_ptr<Account> create(const std::string &name, AccountType type);

        const std::string &getId() const { return id_; }
        const std::string &getName() const { return name_; }
        AccountType getType() const { return type_; }

        void addWallet(std::shared_ptr<market::financial::Wallet> wallet);
        std::shared_ptr<market::financial::Wallet> getWallet(const std::string &walletId) const;
        const std::unordered_map<std::string, std::shared_ptr<market::financial::Wallet>> &getWallets() const { return wallets_; }

        void addAsset(std::shared_ptr<market::financial::Asset> asset);
        void removeAsset(const std::string &assetId);
        std::shared_ptr<market::financial::Asset> getAsset(const std::string &assetId) const;
        const std::unordered_map<std::string, std::shared_ptr<market::financial::Asset>> &getAssets() const { return assets_; }

        void addLiability(std::shared_ptr<market::financial::Liability> liability);
        void removeLiability(const std::string &liabilityId);
        std::shared_ptr<market::financial::Liability> getLiability(const std::string &liabilityId) const;
        const std::unordered_map<std::string, std::shared_ptr<market::financial::Liability>> &getLiabilities() const { return liabilities_; }

        void addContract(std::shared_ptr<market::contracts::Contract> contract);
        std::shared_ptr<market::contracts::Contract> getContract(const std::string &contractId) const;
        const std::unordered_map<std::string, std::shared_ptr<market::contracts::Contract>> &getContracts() const { return contracts_; }

        Decimal getBalance() const;

    private:
        Account(const std::string &id, const std::string &name, AccountType type);
        std::string id_;
        std::string name_;
        AccountType type_;
        std::unordered_map<std::string, std::shared_ptr<market::financial::Wallet>> wallets_;
        std::unordered_map<std::string, std::shared_ptr<market::financial::Asset>> assets_;
        std::unordered_map<std::string, std::shared_ptr<market::financial::Liability>> liabilities_;
        std::unordered_map<std::string, std::shared_ptr<market::contracts::Contract>> contracts_;

        static IDGenerator idGen_;
    };

} // namespace market::core