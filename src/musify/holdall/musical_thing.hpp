
#pragma once

#include <iosfwd>
#include <string>

namespace musify::database {

    enum class ParsingResult
    {
        Ok,
        IncompleteData,
        InvalidData,
    };

    class MusicalThing
    {
        friend std::ostream& operator<<(std::ostream& output_stream, const MusicalThing& thing);

    public:
        virtual ~MusicalThing() = default;

        virtual const std::string& name() const = 0;

        virtual std::string_view concrete_type_label() const = 0;

        virtual ParsingResult parse_details(std::string_view details) = 0;

    protected:
        virtual void to_stream(std::ostream& output_stream) const = 0;
    };

} // namespace musify::database
