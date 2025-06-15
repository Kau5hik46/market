#include "accounting/Journal.h"
#include "utils/IDGenerator.h"
#include <stdexcept>

namespace market
{
    namespace accounting
    {
        IDGenerator Journal::idGen_{"JNL", 12};

        std::shared_ptr<Journal> Journal::create(const std::string &name)
        {
            if (name.empty())
            {
                throw std::invalid_argument("Journal name cannot be empty");
            }
            return std::shared_ptr<Journal>(new Journal(idGen_.next(), name));
        }

        Journal::Journal(const std::string &id, const std::string &name)
            : id_(id), name_(name) {}

        void Journal::addEntry(std::shared_ptr<JournalEntry> entry)
        {
            if (!entry)
            {
                throw std::invalid_argument("Entry cannot be null");
            }

            entries_.push_back(entry);
            size_t index = entries_.size() - 1;

            // Update indices
            for (const auto &e : entry->getEntries())
            {
                accountIndex_[e.accountId].push_back(index);
            }
            transactionIndex_[entry->getTransactionId()].push_back(index);
        }

        std::vector<std::shared_ptr<JournalEntry>> Journal::getEntries() const
        {
            return entries_;
        }

        std::vector<std::shared_ptr<JournalEntry>> Journal::getEntriesByAccount(const std::string &accountId) const
        {
            std::vector<std::shared_ptr<JournalEntry>> result;
            auto it = accountIndex_.find(accountId);
            if (it != accountIndex_.end())
            {
                for (size_t index : it->second)
                {
                    result.push_back(entries_[index]);
                }
            }
            return result;
        }

        std::vector<std::shared_ptr<JournalEntry>> Journal::getEntriesByTransaction(const std::string &transactionId) const
        {
            std::vector<std::shared_ptr<JournalEntry>> result;
            auto it = transactionIndex_.find(transactionId);
            if (it != transactionIndex_.end())
            {
                for (size_t index : it->second)
                {
                    result.push_back(entries_[index]);
                }
            }
            return result;
        }

        std::vector<std::shared_ptr<JournalEntry>> Journal::getEntriesByDateRange(
            const std::chrono::system_clock::time_point &start,
            const std::chrono::system_clock::time_point &end) const
        {
            std::vector<std::shared_ptr<JournalEntry>> result;
            for (const auto &entry : entries_)
            {
                auto timestamp = entry->getTimestamp();
                if (timestamp >= start && timestamp <= end)
                {
                    result.push_back(entry);
                }
            }
            return result;
        }

    } // namespace accounting
} // namespace market