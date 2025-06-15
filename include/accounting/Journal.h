#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <unordered_map>
#include "utils/Decimal.h"
#include "utils/IDGenerator.h"
#include "financial/Transaction.h"
#include "accounting/JournalEntry.h"

namespace market
{
    namespace accounting
    {
        class Journal
        {
        public:
            static std::shared_ptr<Journal> create(const std::string &name);

            void addEntry(std::shared_ptr<JournalEntry> entry);
            std::vector<std::shared_ptr<JournalEntry>> getEntries() const;
            std::vector<std::shared_ptr<JournalEntry>> getEntriesByAccount(const std::string &accountId) const;
            std::vector<std::shared_ptr<JournalEntry>> getEntriesByTransaction(const std::string &transactionId) const;
            std::vector<std::shared_ptr<JournalEntry>> getEntriesByDateRange(
                const std::chrono::system_clock::time_point &start,
                const std::chrono::system_clock::time_point &end) const;

            const std::string &getName() const { return name_; }
            const std::string &getId() const { return id_; }

        private:
            static IDGenerator idGen_;
            Journal(const std::string &id, const std::string &name);

            std::string id_;
            std::string name_;
            std::vector<std::shared_ptr<JournalEntry>> entries_;
            std::unordered_map<std::string, std::vector<size_t>> accountIndex_;
            std::unordered_map<std::string, std::vector<size_t>> transactionIndex_;
        };

    } // namespace accounting
} // namespace market