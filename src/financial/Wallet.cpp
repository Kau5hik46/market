#include "financial/Wallet.h"
#include <stdexcept>

namespace market::financial
{

    IDGenerator Wallet::idGen_{"WLT", 9};

    std::shared_ptr<Wallet> Wallet::create(const std::string &currency)
    {
        if (currency.empty() || currency.size() != 3)
        {
            throw std::invalid_argument("Currency must be a 3-letter code");
        }
        return std::make_shared<Wallet>(idGen_.next(), currency);
    }

    Wallet::Wallet(const std::string &id, const std::string &currency)
        : id_(id), currency_(currency) {}

    void Wallet::addTransaction(std::shared_ptr<Transaction> transaction)
    {
        if (!transaction)
            throw std::invalid_argument("Transaction cannot be null");
        transactions_.push_back(transaction);
    }

    void Wallet::processTransaction(std::shared_ptr<Transaction> transaction)
    {
        if (!transaction)
            throw std::invalid_argument("Transaction cannot be null");
        switch (transaction->getType())
        {
        case Transaction::Type::DEPOSIT:
            balance_ = balance_ + transaction->getAmount();
            break;
        case Transaction::Type::WITHDRAWAL:
            balance_ = balance_ - transaction->getAmount();
            break;
        case Transaction::Type::TRANSFER:
        case Transaction::Type::TRADE:
            // Process transaction logic here
            break;
        default:
            throw std::runtime_error("Unknown transaction type");
        }
        transaction->setStatus(Transaction::Status::COMPLETED);
    }

    Decimal Wallet::getNetWorth() const
    {
        return balance_;
    }

} // namespace market::financial