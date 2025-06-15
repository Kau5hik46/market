#include "accounting/Ledger.h"
#include <stdexcept>
#include <algorithm>

namespace market
{
    namespace accounting
    {

        IDGenerator LedgerEntry::idGen_{"LEN", 12};
        IDGenerator Ledger::idGen_{"LDG", 12};

        std::shared_ptr<LedgerEntry> LedgerEntry::create(
            const std::string &accountId,
            const std::string &journalEntryId,
            EntryType type,
            const Decimal &amount)
        {
            if (accountId.empty())
            {
                throw std::invalid_argument("Account ID cannot be empty");
            }
            if (journalEntryId.empty())
            {
                throw std::invalid_argument("Journal entry ID cannot be empty");
            }
            if (amount <= Decimal(0))
            {
                throw std::invalid_argument("Amount must be positive");
            }

            return std::shared_ptr<LedgerEntry>(new LedgerEntry(
                idGen_.next(),
                accountId,
                journalEntryId,
                type,
                amount));
        }

        LedgerEntry::LedgerEntry(
            const std::string &id,
            const std::string &accountId,
            const std::string &journalEntryId,
            EntryType type,
            const Decimal &amount) : id_(id),
                                     accountId_(accountId),
                                     journalEntryId_(journalEntryId),
                                     type_(type),
                                     amount_(amount),
                                     timestamp_(std::chrono::system_clock::now())
        {
        }

        std::shared_ptr<Ledger> Ledger::create(const std::string &name)
        {
            if (name.empty())
            {
                throw std::invalid_argument("Ledger name cannot be empty");
            }
            return std::shared_ptr<Ledger>(new Ledger(idGen_.next(), name));
        }

        Ledger::Ledger(const std::string &id, const std::string &name)
            : id_(id), name_(name) {}

        void Ledger::addEntry(std::shared_ptr<LedgerEntry> entry)
        {
            if (!entry)
            {
                throw std::invalid_argument("Entry cannot be null");
            }
            accountEntries_[entry->getAccountId()].push_back(entry);
        }

        Decimal Ledger::getBalance(const std::string &accountId) const
        {
            return getBalance(accountId, std::chrono::system_clock::now());
        }

        Decimal Ledger::getBalance(const std::string &accountId, const std::chrono::system_clock::time_point &asOf) const
        {
            auto it = accountEntries_.find(accountId);
            if (it == accountEntries_.end())
            {
                return Decimal(0);
            }

            Decimal balance(0);
            for (const auto &entry : it->second)
            {
                if (entry->getTimestamp() <= asOf)
                {
                    if (entry->getType() == market::accounting::EntryType::DEBIT)
                    {
                        balance = balance + entry->getAmount();
                    }
                    else
                    {
                        balance = balance - entry->getAmount();
                    }
                }
            }
            return balance;
        }

        std::vector<std::shared_ptr<LedgerEntry>> Ledger::getEntries(const std::string &accountId) const
        {
            auto it = accountEntries_.find(accountId);
            if (it == accountEntries_.end())
            {
                return {};
            }
            return it->second;
        }

        std::vector<std::shared_ptr<LedgerEntry>> Ledger::getEntries(
            const std::string &accountId,
            const std::chrono::system_clock::time_point &start,
            const std::chrono::system_clock::time_point &end) const
        {
            auto entries = getEntries(accountId);
            std::vector<std::shared_ptr<LedgerEntry>> result;

            std::copy_if(entries.begin(), entries.end(), std::back_inserter(result),
                         [&](const std::shared_ptr<LedgerEntry> &entry)
                         {
                             auto timestamp = entry->getTimestamp();
                             return timestamp >= start && timestamp <= end;
                         });

            return result;
        }

    } // namespace accounting
} // namespace market