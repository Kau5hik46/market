#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "utils/Decimal.h"
#include "core/Account.h"
#include "utils/IDGenerator.h"

namespace market::contracts
{

    class Contract
    {
    public:
        enum class State
        {
            DRAFT,
            PENDING,
            ACTIVE,
            COMPLETED,
            CANCELLED,
            DEFAULTED
        };

        static std::shared_ptr<Contract> create(const std::string &type, std::shared_ptr<market::core::Account> party1, std::shared_ptr<market::core::Account> party2);

        const std::string &getId() const { return id_; }
        const std::string &getType() const { return type_; }
        std::shared_ptr<market::core::Account> getParty1() const { return party1_; }
        std::shared_ptr<market::core::Account> getParty2() const { return party2_; }
        State getState() const { return state_; }
        const std::unordered_map<std::string, std::string> &getTerms() const { return terms_; }

        void setState(State newState) { state_ = newState; }

        // Contract terms
        void addTerm(const std::string &key, const std::string &value) { terms_[key] = value; }
        std::string getTerm(const std::string &key) const { return terms_.at(key); }

        // Validation
        virtual bool validateTerms() const
        {
            // Base implementation checks for required terms
            return hasRequiredTerms() && validateTermValues();
        }

    protected:
        Contract(const std::string &id, const std::string &type, std::shared_ptr<market::core::Account> party1, std::shared_ptr<market::core::Account> party2);

        // Helper methods for validation
        virtual bool hasRequiredTerms() const
        {
            // Check for common required terms
            return !terms_.empty() &&
                   terms_.count("amount") > 0 &&
                   terms_.count("currency") > 0;
        }

        virtual bool validateTermValues() const
        {
            // Validate term values
            try
            {
                if (terms_.count("amount") > 0)
                {
                    Decimal amount(terms_.at("amount"));
                    if (amount <= Decimal(0))
                    {
                        return false;
                    }
                }
                return true;
            }
            catch (const std::exception &)
            {
                return false;
            }
        }

    private:
        std::string id_;
        std::string type_;
        State state_;
        std::shared_ptr<market::core::Account> party1_;
        std::shared_ptr<market::core::Account> party2_;
        std::unordered_map<std::string, std::string> terms_;
        static IDGenerator idGen_;
    };
}