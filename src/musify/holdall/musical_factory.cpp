
#include "musical_factory.hpp"
#include "musical_things.hpp"

namespace musify::database {

    std::unique_ptr<MusicalThing> MusicalFactory::create_thing(std::string_view thing_type_label,
                                                               std::string thing_name)
    {
        if (thing_type_label == Artist::type_label)
            return std::make_unique<Artist>(std::move(thing_name));
        if (thing_type_label == Album::type_label)
            return std::make_unique<Album>(std::move(thing_name));
        if (thing_type_label == Song::type_label)
            return std::make_unique<Song>(std::move(thing_name));
        return nullptr;
    }

} // namespace musify::database
