#pragma once

#include <string>
#include <memory>
#include "utils/Decimal.h"
#include "utils/IDGenerator.h"

namespace market::financial
{

    class LiabilityStrategy; // Forward declaration

    class Liability
    {
    public:
        static std::shared_ptr<Liability> create(const std::string &type, const Decimal &value);

        // Getters
        const std::string &getId() const { return id_; }
        const std::string &getType() const { return type_; }
        const Decimal &getValue() const { return value_; }

        // Value management
        void setStrategy(std::shared_ptr<LiabilityStrategy> strategy) { strategy_ = strategy; }
        virtual void updateValue(const Decimal &newValue);
        virtual Decimal calculateCurrentValue() const;

    protected:
        Liability(const std::string &id, const std::string &type, const Decimal &value);
        std::string id_;
        std::string type_;
        Decimal value_;
        std::shared_ptr<LiabilityStrategy> strategy_;
        static IDGenerator idGen_;
    };

} // namespace market::financial