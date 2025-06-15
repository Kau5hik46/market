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

        class CashFlowStatement : public IReport
        {
        public:
            struct AccountLine
            {
                std::string accountId;
                std::string accountName;
                Decimal amount;
            };

            static std::shared_ptr<CashFlowStatement> create(
                std::shared_ptr<Ledger> ledger,
                const std::vector<std::pair<std::string, std::string>> &inflowAccounts,
                const std::vector<std::pair<std::string, std::string>> &outflowAccounts,
                bool showEmptyAccounts = false);

            void generate(std::ostream &out) const override;
            std::string getName() const override { return "Cash Flow Statement"; }
            Decimal getTotalInflows() const { return totalInflows_; }
            Decimal getTotalOutflows() const { return totalOutflows_; }
            Decimal getNetCashFlow() const { return totalInflows_ - totalOutflows_; }
            const std::vector<AccountLine> &getInflowLines() const { return inflowLines_; }
            const std::vector<AccountLine> &getOutflowLines() const { return outflowLines_; }

        private:
            CashFlowStatement(
                std::shared_ptr<Ledger> ledger,
                const std::vector<std::pair<std::string, std::string>> &inflowAccounts,
                const std::vector<std::pair<std::string, std::string>> &outflowAccounts,
                bool showEmptyAccounts);
            void compute();

            std::shared_ptr<Ledger> ledger_;
            std::vector<std::pair<std::string, std::string>> inflowAccounts_;
            std::vector<std::pair<std::string, std::string>> outflowAccounts_;
            std::vector<AccountLine> inflowLines_;
            std::vector<AccountLine> outflowLines_;
            Decimal totalInflows_;
            Decimal totalOutflows_;
            bool showEmptyAccounts_;
        };

    } // namespace accounting
} // namespace market