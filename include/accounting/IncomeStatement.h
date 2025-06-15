#pragma once

#include "accounting/IReport.h"
#include "accounting/Ledger.h"
#include <vector>
#include <string>
#include <memory>

namespace market
{
    namespace accounting
    {

        class IncomeStatement : public IReport
        {
        public:
            struct AccountLine
            {
                std::string accountId;
                std::string accountName;
                Decimal amount;
            };

            static std::shared_ptr<IncomeStatement> create(
                std::shared_ptr<Ledger> ledger,
                const std::vector<std::pair<std::string, std::string>> &revenueAccounts,
                const std::vector<std::pair<std::string, std::string>> &expenseAccounts,
                bool showEmptyAccounts = false);

            void generate(std::ostream &out) const override;
            std::string getName() const override { return "Income Statement"; }
            Decimal getTotalRevenue() const { return totalRevenue_; }
            Decimal getTotalExpenses() const { return totalExpenses_; }
            Decimal getNetIncome() const { return totalRevenue_ - totalExpenses_; }
            const std::vector<AccountLine> &getRevenueLines() const { return revenueLines_; }
            const std::vector<AccountLine> &getExpenseLines() const { return expenseLines_; }

        private:
            IncomeStatement(
                std::shared_ptr<Ledger> ledger,
                const std::vector<std::pair<std::string, std::string>> &revenueAccounts,
                const std::vector<std::pair<std::string, std::string>> &expenseAccounts,
                bool showEmptyAccounts);
            void compute();

            std::shared_ptr<Ledger> ledger_;
            std::vector<std::pair<std::string, std::string>> revenueAccounts_;
            std::vector<std::pair<std::string, std::string>> expenseAccounts_;
            std::vector<AccountLine> revenueLines_;
            std::vector<AccountLine> expenseLines_;
            Decimal totalRevenue_;
            Decimal totalExpenses_;
            bool showEmptyAccounts_;
        };

    } // namespace accounting
} // namespace market