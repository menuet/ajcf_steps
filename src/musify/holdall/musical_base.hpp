
#pragma once

#include "musical_thing.hpp"

namespace musify::database {

    class MusicalBase : public MusicalThing
    {
    public:
        MusicalBase(std::string name) : m_name{name}
        {
        }

        virtual const std::string& name() const final
        {
            return m_name;
        }

    protected:
        virtual void to_stream(std::ostream& output_stream) const final;

    private:
        virtual void details_to_stream(std::ostream& output_stream) const = 0;

        std::string m_name{};
    };

} // namespace musify::database
