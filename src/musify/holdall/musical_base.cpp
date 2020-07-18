
#include "musical_base.hpp"
#include <iostream>

namespace musify::database {

    void MusicalBase::to_stream(std::ostream& output_stream) const
    {
        output_stream << "{{" << name() << ", " << concrete_type_label() << "}, ";
        details_to_stream(output_stream);
        output_stream << "}";
    }

} // namespace musify::database
