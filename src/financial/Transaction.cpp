#include "financial/Transaction.h"
#include "core/Account.h"
#include "financial/Asset.h"
#include "financial/Liability.h"
#include <stdexcept>

namespace market::financial
{

    IDGenerator Transaction::idGen_{"TRX", 12};

    std::shared_ptr<Transaction> Transaction::create(Type type, const Decimal &amount, std::shared_ptr<market::core::Account> account, std::shared_ptr<Asset> asset, std::shared_ptr<Liability> liability)
    {
        if (amount <= Decimal(0))
            throw std::invalid_argument("Transaction amount must be positive");
        if (!account)
            throw std::invalid_argument("Account cannot be null");
        return std::shared_ptr<Transaction>(new Transaction(idGen_.next(), type, amount, account, asset, liability));
    }

    Transaction::Transaction(const std::string &id, Type type, const Decimal &amount, std::shared_ptr<market::core::Account> account, std::shared_ptr<Asset> asset, std::shared_ptr<Liability> liability)
        : id_(id), type_(type), amount_(amount), account_(account), asset_(asset), liability_(liability), timestamp_(std::chrono::system_clock::now()), status_(Status::PENDING) {}

    void Transaction::process()
    {
        if (!account_)
            throw std::runtime_error("Account not set");
        switch (type_)
        {
        case Type::DEPOSIT:
            if (!asset_)
                throw std::runtime_error("Asset not set for deposit");
            asset_->updateValue(asset_->getValue() + amount_);
            break;
        case Type::WITHDRAWAL:
            if (!asset_)
                throw std::runtime_error("Asset not set for withdrawal");
            if (asset_->getValue() < amount_)
                throw std::runtime_error("Insufficient funds");
            asset_->updateValue(asset_->getValue() - amount_);
            break;
        case Type::TRANSFER:
            if (!asset_ || !liability_)
                throw std::runtime_error("Asset and liability must be set for transfer");
            if (asset_->getValue() < amount_)
                throw std::runtime_error("Insufficient funds");
            asset_->updateValue(asset_->getValue() - amount_);
            liability_->updateValue(liability_->getValue() + amount_);
            break;
        case Type::TRADE:
            if (!asset_ || !liability_)
                throw std::runtime_error("Asset and liability must be set for trade");
            asset_->updateValue(asset_->getValue() + amount_);
            liability_->updateValue(liability_->getValue() - amount_);
            break;
        default:
            throw std::runtime_error("Unknown transaction type");
        }
        status_ = Status::COMPLETED;
    }

} // namespace market::financial