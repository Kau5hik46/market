#include "accounting/JournalEntry.h"

IDGenerator market::accounting::JournalEntry::idGen_{"JEN", 12};

namespace market::accounting
{

    std::shared_ptr<JournalEntry> JournalEntry::create(
        const std::string &transactionId,
        const std::vector<Entry> &entries,
        const std::string &description)
    {
        if (entries.empty())
        {
            throw std::invalid_argument("Journal entry must have at least one entry");
        }

        // Validate double-entry bookkeeping
        Decimal totalDebits(0);
        Decimal totalCredits(0);
        for (const auto &entry : entries)
        {
            if (entry.amount <= Decimal(0))
            {
                throw std::invalid_argument("Entry amount must be positive");
            }
            if (entry.type == EntryType::DEBIT)
            {
                totalDebits = totalDebits + entry.amount;
            }
            else
            {
                totalCredits = totalCredits + entry.amount;
            }
        }

        if (totalDebits != totalCredits)
        {
            throw std::invalid_argument("Debits and credits must be equal");
        }

        return std::shared_ptr<JournalEntry>(new JournalEntry(idGen_.next(), transactionId, entries, description));
    }

    JournalEntry::JournalEntry(
        const std::string &id,
        const std::string &transactionId,
        const std::vector<Entry> &entries,
        const std::string &description)
        : id_(id), transactionId_(transactionId), entries_(entries), description_(description), timestamp_(std::chrono::system_clock::now()) {}

} // namespace market::accounting