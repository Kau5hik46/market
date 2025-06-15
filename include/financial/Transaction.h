#pragma once
#include <string>
#include <memory>
#include <chrono>
#include "utils/Decimal.h"
#include "utils/IDGenerator.h"
#include "core/Account.h"

namespace market::financial
{

    class Asset;
    class Liability;

    class Transaction
    {
    public:
        enum class Type
        {
            DEPOSIT,
            WITHDRAWAL,
            TRANSFER,
            TRADE
        };
        enum class Status
        {
            PENDING,
            COMPLETED,
            FAILED
        };

        static std::shared_ptr<Transaction> create(Type type, const Decimal &amount, std::shared_ptr<market::core::Account> account, std::shared_ptr<Asset> asset, std::shared_ptr<Liability> liability);

        const std::string &getId() const { return id_; }
        Type getType() const { return type_; }
        const Decimal &getAmount() const { return amount_; }
        std::shared_ptr<market::core::Account> getAccount() const { return account_; }
        std::shared_ptr<Asset> getAsset() const { return asset_; }
        std::shared_ptr<Liability> getLiability() const { return liability_; }
        std::chrono::system_clock::time_point getTimestamp() const { return timestamp_; }
        Status getStatus() const { return status_; }
        void setStatus(Status status) { status_ = status; }

        void process();

    private:
        Transaction(const std::string &id, Type type, const Decimal &amount, std::shared_ptr<market::core::Account> account, std::shared_ptr<Asset> asset, std::shared_ptr<Liability> liability);
        std::string id_;
        Type type_;
        Decimal amount_;
        std::shared_ptr<market::core::Account> account_;
        std::shared_ptr<Asset> asset_;
        std::shared_ptr<Liability> liability_;
        std::chrono::system_clock::time_point timestamp_;
        Status status_;
        static IDGenerator idGen_;
    };

} // namespace market::financial