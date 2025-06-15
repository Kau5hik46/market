#pragma once

#include <string>
#include <memory>
#include <ostream>

namespace market
{
    namespace accounting
    {

        class IReport
        {
        public:
            virtual ~IReport() = default;
            virtual void generate(std::ostream &out) const = 0;
            virtual std::string getName() const = 0;
        };

        using IReportPtr = std::shared_ptr<IReport>;

    } // namespace accounting
} // namespace market