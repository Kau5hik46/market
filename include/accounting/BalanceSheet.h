#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <unordered_map>
#include "utils/Decimal.h"
#include "utils/IDGenerator.h"
#include "accounting/Ledger.h"

namespace market
{
    namespace accounting
    {

        class BalanceSheet
        {
        public:
            struct AccountBalance
            {
                std::string accountId;
                std::string accountName;
                Decimal balance;
                bool isDebit;
            };

            struct Section
            {
                std::string name;
                std::vector<AccountBalance> accounts;
                Decimal total;
            };

            static std::shared_ptr<BalanceSheet> create(
                const std::string &name,
                std::shared_ptr<Ledger> ledger,
                const std::chrono::system_clock::time_point &asOf);

            void addAssetAccount(const std::string &accountId, const std::string &accountName, bool isDebit = true);
            void addLiabilityAccount(const std::string &accountId, const std::string &accountName, bool isDebit = false);
            void addEquityAccount(const std::string &accountId, const std::string &accountName, bool isDebit = false);

            const Section &getAssets() const { return assets_; }
            const Section &getLiabilities() const { return liabilities_; }
            const Section &getEquity() const { return equity_; }
            Decimal getTotalAssets() const { return assets_.total; }
            Decimal getTotalLiabilities() const { return liabilities_.total; }
            Decimal getTotalEquity() const { return equity_.total; }
            bool isBalanced() const { return assets_.total == (liabilities_.total + equity_.total); }

            const std::string &getName() const { return name_; }
            const std::string &getId() const { return id_; }
            const std::chrono::system_clock::time_point &getAsOf() const { return asOf_; }

        private:
            static IDGenerator idGen_;
            BalanceSheet(
                const std::string &id,
                const std::string &name,
                std::shared_ptr<Ledger> ledger,
                const std::chrono::system_clock::time_point &asOf);

            void updateSection(Section &section, const std::string &accountId, const std::string &accountName, bool isDebit);
            void calculateTotals();

            std::string id_;
            std::string name_;
            std::shared_ptr<Ledger> ledger_;
            std::chrono::system_clock::time_point asOf_;

            Section assets_{"Assets", {}, Decimal(0)};
            Section liabilities_{"Liabilities", {}, Decimal(0)};
            Section equity_{"Equity", {}, Decimal(0)};

            std::unordered_map<std::string, bool> accountTypes_; // true for debit, false for credit
        };

    } // namespace accounting
} // namespace market