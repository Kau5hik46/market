#include "accounting/IncomeStatement.h"
#include <iomanip>
#include <iostream>

namespace market
{
    namespace accounting
    {

        std::shared_ptr<IncomeStatement> IncomeStatement::create(
            std::shared_ptr<Ledger> ledger,
            const std::vector<std::pair<std::string, std::string>> &revenueAccounts,
            const std::vector<std::pair<std::string, std::string>> &expenseAccounts,
            bool showEmptyAccounts)
        {
            return std::shared_ptr<IncomeStatement>(new IncomeStatement(ledger, revenueAccounts, expenseAccounts, showEmptyAccounts));
        }

        IncomeStatement::IncomeStatement(
            std::shared_ptr<Ledger> ledger,
            const std::vector<std::pair<std::string, std::string>> &revenueAccounts,
            const std::vector<std::pair<std::string, std::string>> &expenseAccounts,
            bool showEmptyAccounts)
            : ledger_(ledger), revenueAccounts_(revenueAccounts), expenseAccounts_(expenseAccounts), totalRevenue_(0), totalExpenses_(0), showEmptyAccounts_(showEmptyAccounts)
        {
            compute();
        }

        void IncomeStatement::compute()
        {
            revenueLines_.clear();
            expenseLines_.clear();
            totalRevenue_ = Decimal(0);
            totalExpenses_ = Decimal(0);

            for (const auto &acc : revenueAccounts_)
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
                    revenueLines_.push_back(line);
                    totalRevenue_ = totalRevenue_ + balance;
                }
            }

            for (const auto &acc : expenseAccounts_)
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
                    expenseLines_.push_back(line);
                    totalExpenses_ = totalExpenses_ + (-balance);
                }
            }
        }

        void IncomeStatement::generate(std::ostream &out) const
        {
            out << "\nINCOME STATEMENT\n";
            out << std::left << std::setw(16) << "Account ID" << std::setw(24) << "Account Name" << std::right << std::setw(16) << "Amount" << "\n";
            out << std::string(56, '-') << "\n";
            out << "REVENUE\n";
            for (const auto &line : revenueLines_)
            {
                out << std::left << std::setw(16) << line.accountId
                    << std::setw(24) << line.accountName
                    << std::right << std::setw(16) << line.amount.toString() << "\n";
            }
            out << std::string(56, '-') << "\n";
            out << std::left << std::setw(40) << "Total Revenue"
                << std::right << std::setw(16) << totalRevenue_.toString() << "\n";
            out << std::string(56, '-') << "\n";
            out << "EXPENSES\n";
            for (const auto &line : expenseLines_)
            {
                out << std::left << std::setw(16) << line.accountId
                    << std::setw(24) << line.accountName
                    << std::right << std::setw(16) << line.amount.toString() << "\n";
            }
            out << std::string(56, '-') << "\n";
            out << std::left << std::setw(40) << "Total Expenses"
                << std::right << std::setw(16) << totalExpenses_.toString() << "\n";
            out << std::string(56, '-') << "\n";
            out << std::left << std::setw(40) << "Net Income"
                << std::right << std::setw(16) << getNetIncome().toString() << "\n";
        }

    } // namespace accounting
} // namespace market