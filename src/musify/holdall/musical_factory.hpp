
#pragma once

#include "strong_types.hpp"
#include <string_view>
#include <memory>
#include <string>

namespace musify::database {

    class MusicalThing;

    class MusicalFactory
    {
    public:
        std::unique_ptr<MusicalThing> create_thing(std::string_view thing_type_label, std::string thing_name);
    };

} // namespace musify::database
