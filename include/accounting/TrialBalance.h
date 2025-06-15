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

        class TrialBalance : public IReport
        {
        public:
            struct AccountLine
            {
                std::string accountId;
                std::string accountName;
                Decimal debit;
                Decimal credit;
            };

            static std::shared_ptr<TrialBalance> create(std::shared_ptr<Ledger> ledger, const std::vector<std::pair<std::string, std::string>> &accountNames, bool showEmptyAccounts = false);

            void generate(std::ostream &out) const override;
            std::string getName() const override { return "Trial Balance"; }
            bool isBalanced() const { return totalDebits_ == totalCredits_; }
            Decimal getTotalDebits() const { return totalDebits_; }
            Decimal getTotalCredits() const { return totalCredits_; }
            const std::vector<AccountLine> &getLines() const { return lines_; }

        private:
            TrialBalance(std::shared_ptr<Ledger> ledger, const std::vector<std::pair<std::string, std::string>> &accountNames, bool showEmptyAccounts);
            void compute();

            std::shared_ptr<Ledger> ledger_;
            std::vector<std::pair<std::string, std::string>> accountNames_;
            std::vector<AccountLine> lines_;
            Decimal totalDebits_;
            Decimal totalCredits_;
            bool showEmptyAccounts_;
        };

    } // namespace accounting
} // namespace market