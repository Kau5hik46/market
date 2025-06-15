#include "accounting/TrialBalance.h"
#include "accounting/IncomeStatement.h"
#include "accounting/CashFlowStatement.h"
#include "accounting/Ledger.h"
#include "accounting/Journal.h"
#include "accounting/JournalEntry.h"
#include "utils/Decimal.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace market::accounting;

int main()
{
    // Create a ledger
    auto ledger = Ledger::create("Main Ledger");

    // Create a journal
    auto journal = Journal::create("Main Journal");

    // Define some accounts (ID, Name)
    std::vector<std::pair<std::string, std::string>> accounts = {
        {"ACC001", "Cash"},
        {"ACC002", "Accounts Receivable"},
        {"ACC003", "Revenue"},
        {"ACC004", "Expenses"},
        {"ACC005", "Accounts Payable"}};

    // Simulate some transactions
    // Transaction 1: Revenue of 1000
    std::vector<JournalEntry::Entry> entries1 = {
        {"ACC001", market::accounting::EntryType::DEBIT, Decimal(1000), "Revenue received"},
        {"ACC003", market::accounting::EntryType::CREDIT, Decimal(1000), "Revenue recorded"}};
    auto entry1 = JournalEntry::create("TRX001", entries1, "Revenue transaction");
    journal->addEntry(entry1);

    // Transaction 2: Expense of 500
    std::vector<JournalEntry::Entry> entries2 = {
        {"ACC004", market::accounting::EntryType::DEBIT, Decimal(500), "Expense incurred"},
        {"ACC001", market::accounting::EntryType::CREDIT, Decimal(500), "Cash paid for expense"}};
    auto entry2 = JournalEntry::create("TRX002", entries2, "Expense transaction");
    journal->addEntry(entry2);

    // Update ledger with journal entries
    for (const auto &entry : journal->getEntries())
    {
        for (const auto &e : entry->getEntries())
        {
            auto ledgerEntry = LedgerEntry::create(e.accountId, entry->getId(), e.type, e.amount);
            ledger->addEntry(ledgerEntry);
        }
    }

    // Generate reports
    // 1. Trial Balance
    auto trialBalance = TrialBalance::create(ledger, accounts, false);
    std::cout << "Trial Balance Report:" << std::endl;
    trialBalance->generate(std::cout);

    // 2. Income Statement
    std::vector<std::pair<std::string, std::string>> revenueAccounts = {{"ACC003", "Revenue"}};
    std::vector<std::pair<std::string, std::string>> expenseAccounts = {{"ACC004", "Expenses"}};
    auto incomeStatement = IncomeStatement::create(ledger, revenueAccounts, expenseAccounts, false);
    std::cout << "\nIncome Statement Report:" << std::endl;
    incomeStatement->generate(std::cout);

    // 3. Cash Flow Statement
    std::vector<std::pair<std::string, std::string>> inflowAccounts = {{"ACC001", "Cash"}};
    std::vector<std::pair<std::string, std::string>> outflowAccounts = {{"ACC001", "Cash"}};
    auto cashFlowStatement = CashFlowStatement::create(ledger, inflowAccounts, outflowAccounts, false);
    std::cout << "\nCash Flow Statement Report:" << std::endl;
    cashFlowStatement->generate(std::cout);

    return 0;
}