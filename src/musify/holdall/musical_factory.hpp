
#pragma once

#include "singleton.hpp"
#include <string_view>
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace musify::database {

    class MusicalThing;

    class MusicalFactory
    {
    public:
        using Creator = std::function<std::unique_ptr<MusicalThing>(std::string)>;

        MusicalFactory();
        ~MusicalFactory();

        MusicalFactory(MusicalFactory&&) = delete;
        MusicalFactory& operator=(MusicalFactory&&) = delete;

        std::unique_ptr<MusicalThing> create_thing(std::string_view thing_type_label, std::string thing_name);

        void register_creator(std::string_view thing_type_label, Creator thing_creator);

    private:
        using Creators = std::map<std::string_view, Creator>;

        Creators m_creators;
    };

    template <typename ThingT>
    struct MusicalThingRegistrar
    {
        MusicalThingRegistrar()
        {
            static constexpr std::string_view type_label = ThingT::type_label;
            auto thing_creator = [](std::string thing_name) { return std::make_unique<ThingT>(std::move(thing_name)); };
            singleton::Singleton<MusicalFactory>::get_instance().register_creator(type_label, std::move(thing_creator));
        }
    };

} // namespace musify::database
