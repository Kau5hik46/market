#pragma once

#include <string>
#include <memory>
#include <vector>
#include "utils/Decimal.h"
#include "financial/Transaction.h"
#include "utils/IDGenerator.h"

namespace market::financial
{

    class Wallet
    {
    public:
        static std::shared_ptr<Wallet> create(const std::string &currency);

        const std::string &getId() const { return id_; }
        const std::string &getCurrency() const { return currency_; }

        void addTransaction(std::shared_ptr<market::financial::Transaction> transaction);
        const std::vector<std::shared_ptr<market::financial::Transaction>> &getTransactions() const { return transactions_; }
        void processTransaction(std::shared_ptr<market::financial::Transaction> transaction);
        Decimal getNetWorth() const;

    private:
        Wallet(const std::string &id, const std::string &currency);
        virtual ~Wallet() = default;

        std::string id_;
        std::string currency_;
        std::vector<std::shared_ptr<market::financial::Transaction>> transactions_;
        Decimal balance_{0};

        static IDGenerator idGen_;
    };

} // namespace market::financial