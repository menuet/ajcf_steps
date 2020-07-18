
#include "musical_thing.hpp"
#include <iostream>

namespace musify::database {

    std::ostream& operator<<(std::ostream& output_stream, const MusicalThing& thing)
    {
        thing.to_stream(output_stream);
        return output_stream;
    }

} // namespace musify::database
