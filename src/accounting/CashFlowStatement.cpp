#include "accounting/CashFlowStatement.h"
#include <iomanip>
#include <iostream>

namespace market
{
    namespace accounting
    {

        std::shared_ptr<CashFlowStatement> CashFlowStatement::create(
            std::shared_ptr<Ledger> ledger,
            const std::vector<std::pair<std::string, std::string>> &inflowAccounts,
            const std::vector<std::pair<std::string, std::string>> &outflowAccounts,
            bool showEmptyAccounts)
        {
            return std::shared_ptr<CashFlowStatement>(new CashFlowStatement(ledger, inflowAccounts, outflowAccounts, showEmptyAccounts));
        }

        CashFlowStatement::CashFlowStatement(
            std::shared_ptr<Ledger> ledger,
            const std::vector<std::pair<std::string, std::string>> &inflowAccounts,
            const std::vector<std::pair<std::string, std::string>> &outflowAccounts,
            bool showEmptyAccounts)
            : ledger_(ledger), inflowAccounts_(inflowAccounts), outflowAccounts_(outflowAccounts), totalInflows_(0), totalOutflows_(0), showEmptyAccounts_(showEmptyAccounts)
        {
            compute();
        }

        void CashFlowStatement::compute()
        {
            inflowLines_.clear();
            outflowLines_.clear();
            totalInflows_ = Decimal(0);
            totalOutflows_ = Decimal(0);

            for (const auto &acc : inflowAccounts_)
            {
                Decimal balance = ledger_->getBalance(acc.first);
                if (!showEmptyAccounts_ && balance == Decimal(0))
                {
                    continue;
                }
                if (balance > Decimal(0))
                {
                    AccountLine line;
                    line.accountId = acc.first;
                    line.accountName = acc.second;
                    line.amount = balance;
                    inflowLines_.push_back(line);
                    totalInflows_ = totalInflows_ + balance;
                }
            }

            for (const auto &acc : outflowAccounts_)
            {
                Decimal balance = ledger_->getBalance(acc.first);
                if (!showEmptyAccounts_ && balance == Decimal(0))
                {
                    continue;
                }
                if (balance < Decimal(0))
                {
                    AccountLine line;
                    line.accountId = acc.first;
                    line.accountName = acc.second;
                    line.amount = -balance;
                    outflowLines_.push_back(line);
                    totalOutflows_ = totalOutflows_ + (-balance);
                }
            }
        }

        void CashFlowStatement::generate(std::ostream &out) const
        {
            out << "\nCASH FLOW STATEMENT\n";
            out << std::left << std::setw(16) << "Account ID" << std::setw(24) << "Account Name" << std::right << std::setw(16) << "Amount" << "\n";
            out << std::string(56, '-') << "\n";
            out << "CASH INFLOWS\n";
            for (const auto &line : inflowLines_)
            {
                out << std::left << std::setw(16) << line.accountId
                    << std::setw(24) << line.accountName
                    << std::right << std::setw(16) << line.amount.toString() << "\n";
            }
            out << std::string(56, '-') << "\n";
            out << std::left << std::setw(40) << "Total Inflows"
                << std::right << std::setw(16) << totalInflows_.toString() << "\n";
            out << std::string(56, '-') << "\n";
            out << "CASH OUTFLOWS\n";
            for (const auto &line : outflowLines_)
            {
                out << std::left << std::setw(16) << line.accountId
                    << std::setw(24) << line.accountName
                    << std::right << std::setw(16) << line.amount.toString() << "\n";
            }
            out << std::string(56, '-') << "\n";
            out << std::left << std::setw(40) << "Total Outflows"
                << std::right << std::setw(16) << totalOutflows_.toString() << "\n";
            out << std::string(56, '-') << "\n";
            out << std::left << std::setw(40) << "Net Cash Flow"
                << std::right << std::setw(16) << getNetCashFlow().toString() << "\n";
        }

    } // namespace accounting
} // namespace market