
#pragma once

#include <string>
#include <vector>

namespace musify { namespace artists {

    std::vector<std::string> ask_new_artists();

    void sort_new_artists(std::vector<std::string>& artists_names);

    bool save_new_artists(const std::vector<std::string>& artists_names);

}} // namespace musify::artists
