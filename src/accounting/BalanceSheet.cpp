#include "accounting/BalanceSheet.h"
#include "utils/IDGenerator.h"
#include <memory>
#include <stdexcept>

namespace market
{
    namespace accounting
    {

        IDGenerator BalanceSheet::idGen_{"BSH", 12};

        std::shared_ptr<BalanceSheet> BalanceSheet::create(
            const std::string &name,
            std::shared_ptr<Ledger> ledger,
            const std::chrono::system_clock::time_point &asOf)
        {
            if (name.empty())
                throw std::invalid_argument("BalanceSheet name cannot be empty");
            if (!ledger)
                throw std::invalid_argument("Ledger cannot be null");
            return std::shared_ptr<BalanceSheet>(new BalanceSheet(idGen_.next(), name, ledger, asOf));
        }

        BalanceSheet::BalanceSheet(
            const std::string &id,
            const std::string &name,
            std::shared_ptr<Ledger> ledger,
            const std::chrono::system_clock::time_point &asOf) : id_(id), name_(name), ledger_(ledger), asOf_(asOf) {}

        void BalanceSheet::addAssetAccount(const std::string &accountId, const std::string &accountName, bool isDebit)
        {
            updateSection(assets_, accountId, accountName, isDebit);
            accountTypes_[accountId] = isDebit;
            calculateTotals();
        }

        void BalanceSheet::addLiabilityAccount(const std::string &accountId, const std::string &accountName, bool isDebit)
        {
            updateSection(liabilities_, accountId, accountName, isDebit);
            accountTypes_[accountId] = isDebit;
            calculateTotals();
        }

        void BalanceSheet::addEquityAccount(const std::string &accountId, const std::string &accountName, bool isDebit)
        {
            updateSection(equity_, accountId, accountName, isDebit);
            accountTypes_[accountId] = isDebit;
            calculateTotals();
        }

        void BalanceSheet::updateSection(Section &section, const std::string &accountId, const std::string &accountName, bool isDebit)
        {
            Decimal balance = ledger_->getBalance(accountId, asOf_);
            section.accounts.push_back({accountId, accountName, balance, isDebit});
        }

        void BalanceSheet::calculateTotals()
        {
            assets_.total = Decimal(0);
            for (const auto &acc : assets_.accounts)
            {
                assets_.total = assets_.total + acc.balance;
            }
            liabilities_.total = Decimal(0);
            for (const auto &acc : liabilities_.accounts)
            {
                liabilities_.total = liabilities_.total + acc.balance;
            }
            equity_.total = Decimal(0);
            for (const auto &acc : equity_.accounts)
            {
                equity_.total = equity_.total + acc.balance;
            }
        }

    } // namespace accounting
} // namespace market