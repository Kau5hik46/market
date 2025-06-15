#pragma once

#include "utils/Decimal.h"
#include "accounting/EntryType.h"
#include "utils/IDGenerator.h"
#include <string>
#include <vector>
#include <memory>
#include <chrono>

namespace market::accounting
{

    class JournalEntry
    {
    public:
        struct Entry
        {
            std::string accountId;
            EntryType type;
            Decimal amount;
            std::string description;
        };

        static std::shared_ptr<JournalEntry> create(
            const std::string &transactionId,
            const std::vector<Entry> &entries,
            const std::string &description = "");

        const std::string &getId() const { return id_; }
        const std::string &getTransactionId() const { return transactionId_; }
        const std::vector<Entry> &getEntries() const { return entries_; }
        const std::string &getDescription() const { return description_; }
        const std::chrono::system_clock::time_point &getTimestamp() const { return timestamp_; }

    private:
        static IDGenerator idGen_;
        JournalEntry(const std::string &id, const std::string &transactionId, const std::vector<Entry> &entries, const std::string &description);

        std::string id_;
        std::string transactionId_;
        std::vector<Entry> entries_;
        std::string description_;
        std::chrono::system_clock::time_point timestamp_;
    };

} // namespace market::accounting