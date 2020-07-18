
#include "musical_factory.hpp"
#include "musical_thing.hpp"

namespace musify::database {

    MusicalFactory::MusicalFactory() = default;

    MusicalFactory::~MusicalFactory() = default;

    std::unique_ptr<MusicalThing> MusicalFactory::create_thing(std::string_view thing_type_label,
                                                               std::string thing_name)
    {
        const auto iter = m_creators.find(thing_type_label);
        if (iter == m_creators.end())
            return nullptr;
        const auto& thing_creator = iter->second;
        return thing_creator(std::move(thing_name));
    }

    void MusicalFactory::register_creator(std::string_view thing_type_label, Creator thing_creator)
    {
        m_creators[thing_type_label] = std::move(thing_creator);
    }

} // namespace musify::database
