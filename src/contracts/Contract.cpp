#include "contracts/Contract.h"
#include "core/Account.h"
#include <stdexcept>

namespace market::contracts
{

    IDGenerator Contract::idGen_{"CNT", 9};

    std::shared_ptr<Contract> Contract::create(const std::string &type, std::shared_ptr<market::core::Account> party1, std::shared_ptr<market::core::Account> party2)
    {
        if (type.empty())
            throw std::invalid_argument("Contract type cannot be empty");
        if (!party1 || !party2)
            throw std::invalid_argument("Both parties must be valid");
        if (party1 == party2)
            throw std::invalid_argument("Parties must be different accounts");
        return std::shared_ptr<Contract>(new Contract(idGen_.next(), type, party1, party2));
    }

    Contract::Contract(const std::string &id, const std::string &type, std::shared_ptr<market::core::Account> party1, std::shared_ptr<market::core::Account> party2)
        : id_(id), type_(type), party1_(party1), party2_(party2), state_(State::DRAFT)
    {
        if (id.empty())
            throw std::invalid_argument("Contract ID cannot be empty");
    }

    bool Contract::hasRequiredTerms() const
    {
        return !terms_.empty() && terms_.count("amount") > 0 && terms_.count("currency") > 0;
    }

    bool Contract::validateTermValues() const
    {
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

} // namespace market::contracts