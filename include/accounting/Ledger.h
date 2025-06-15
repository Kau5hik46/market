#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <unordered_map>
#include "utils/Decimal.h"
#include "utils/IDGenerator.h"
#include "accounting/Journal.h"
#include "accounting/EntryType.h"

namespace market
{
    namespace accounting
    {

        class LedgerEntry
        {
        public:
            struct Entry
            {
                std::string journalEntryId;
                market::accounting::EntryType type;
                Decimal amount;
                std::chrono::system_clock::time_point timestamp;
            };

            static std::shared_ptr<LedgerEntry> create(
                const std::string &accountId,
                const std::string &journalEntryId,
                market::accounting::EntryType type,
                const Decimal &amount);

            const std::string &getId() const { return id_; }
            const std::string &getAccountId() const { return accountId_; }
            const std::string &getJournalEntryId() const { return journalEntryId_; }
            market::accounting::EntryType getType() const { return type_; }
            const Decimal &getAmount() const { return amount_; }
            const std::chrono::system_clock::time_point &getTimestamp() const { return timestamp_; }

        private:
            static IDGenerator idGen_;
            LedgerEntry(
                const std::string &id,
                const std::string &accountId,
                const std::string &journalEntryId,
                market::accounting::EntryType type,
                const Decimal &amount);

            std::string id_;
            std::string accountId_;
            std::string journalEntryId_;
            market::accounting::EntryType type_;
            Decimal amount_;
            std::chrono::system_clock::time_point timestamp_;
        };

        class Ledger
        {
        public:
            static std::shared_ptr<Ledger> create(const std::string &name);

            void addEntry(std::shared_ptr<LedgerEntry> entry);
            Decimal getBalance(const std::string &accountId) const;
            Decimal getBalance(const std::string &accountId, const std::chrono::system_clock::time_point &asOf) const;
            std::vector<std::shared_ptr<LedgerEntry>> getEntries(const std::string &accountId) const;
            std::vector<std::shared_ptr<LedgerEntry>> getEntries(
                const std::string &accountId,
                const std::chrono::system_clock::time_point &start,
                const std::chrono::system_clock::time_point &end) const;

            const std::string &getName() const { return name_; }
            const std::string &getId() const { return id_; }

        private:
            static IDGenerator idGen_;
            Ledger(const std::string &id, const std::string &name);

            std::string id_;
            std::string name_;
            std::unordered_map<std::string, std::vector<std::shared_ptr<LedgerEntry>>> accountEntries_;
        };

    } // namespace accounting
} // namespace market