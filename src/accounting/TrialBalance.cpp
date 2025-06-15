#include "accounting/TrialBalance.h"
#include <iomanip>
#include <iostream>

namespace market
{
    namespace accounting
    {

        std::shared_ptr<TrialBalance> TrialBalance::create(std::shared_ptr<Ledger> ledger, const std::vector<std::pair<std::string, std::string>> &accountNames, bool showEmptyAccounts)
        {
            return std::shared_ptr<TrialBalance>(new TrialBalance(ledger, accountNames, showEmptyAccounts));
        }

        TrialBalance::TrialBalance(std::shared_ptr<Ledger> ledger, const std::vector<std::pair<std::string, std::string>> &accountNames, bool showEmptyAccounts)
            : ledger_(ledger), accountNames_(accountNames), totalDebits_(0), totalCredits_(0), showEmptyAccounts_(showEmptyAccounts)
        {
            compute();
        }

        void TrialBalance::compute()
        {
            lines_.clear();
            totalDebits_ = Decimal(0);
            totalCredits_ = Decimal(0);
            for (const auto &acc : accountNames_)
            {
                Decimal balance = ledger_->getBalance(acc.first);
                if (!showEmptyAccounts_ && balance == Decimal(0))
                {
                    continue;
                }
                AccountLine line;
                line.accountId = acc.first;
                line.accountName = acc.second;
                if (balance >= Decimal(0))
                {
                    line.debit = balance;
                    line.credit = Decimal(0);
                    totalDebits_ = totalDebits_ + balance;
                }
                else
                {
                    line.debit = Decimal(0);
                    line.credit = -balance;
                    totalCredits_ = totalCredits_ + (-balance);
                }
                lines_.push_back(line);
            }
        }

        void TrialBalance::generate(std::ostream &out) const
        {
            out << "\nTRIAL BALANCE\n";
            out << std::left << std::setw(16) << "Account ID" << std::setw(24) << "Account Name" << std::right << std::setw(16) << "Debit" << std::setw(16) << "Credit" << "\n";
            out << std::string(72, '-') << "\n";
            for (const auto &line : lines_)
            {
                out << std::left << std::setw(16) << line.accountId
                    << std::setw(24) << line.accountName
                    << std::right << std::setw(16) << line.debit.toString()
                    << std::setw(16) << line.credit.toString() << "\n";
            }
            out << std::string(72, '-') << "\n";
            out << std::left << std::setw(40) << "TOTALS"
                << std::right << std::setw(16) << totalDebits_.toString()
                << std::setw(16) << totalCredits_.toString() << "\n";
            out << (isBalanced() ? "BALANCED" : "NOT BALANCED") << "\n";
        }

    } // namespace accounting
} // namespace market