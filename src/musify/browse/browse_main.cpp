
#include <holdall/database.hpp>
#include <iostream>

namespace mdb = musify::database;
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Please, provide the database file path as an argument\n";
        return -1;
    }

    fs::path database_file_path{argv[1]};
    if (database_file_path.is_relative())
        database_file_path = fs::current_path() / database_file_path;

    std::cout << "Loading database file " << database_file_path << "...\n";

    mdb::Database database;
    const auto result = mdb::load_database(database_file_path, database);
    switch (result)
    {
    case mdb::LoadingResult::FileNotFound:
        std::cerr << "The database file does not exist\n";
        break;
    case mdb::LoadingResult::FileNotReadable:
        std::cerr << "The database file is not readable\n";
        break;
    case mdb::LoadingResult::UnknownLineType:
        std::cerr << "The database file contains a line of unknow type\n";
        break;
    case mdb::LoadingResult::IncompleteLine:
        std::cerr << "The database file contains an incomplete line\n";
        break;
    case mdb::LoadingResult::ParsingError:
        std::cerr << "The database file contains some non-parsable data\n";
        break;
    case mdb::LoadingResult::DuplicateArtist:
        std::cerr << "The database file contains a duplicate artist\n";
        break;
    case mdb::LoadingResult::UnknownArtist:
        std::cerr << "The database file refers to an unknown artist\n";
        break;
    case mdb::LoadingResult::DuplicateAlbum:
        std::cerr << "The database file contains a duplicate album\n";
        break;
    case mdb::LoadingResult::UnknownAlbum:
        std::cerr << "The database file refers to an unknown album\n";
        break;
    case mdb::LoadingResult::DuplicateSong:
        std::cerr << "The database file contains a duplicate song\n";
        break;
    case mdb::LoadingResult::Ok:
        std::cout << "Database file's contents:\n";
        database.display();
        break;
    }

    std::cout << '\n';
    std::cout << "Searching for artist 'Oasis'... ";
    if (const mdb::Artist* artist = database.find_artist("Oasis"); artist)
        std::cout << "Found: " << *artist << "\n";
    else
        std::cout << "Unknown artist\n";

    std::cout << "Searching for Album 'Parachutes'... ";
    if (const auto* album = database.find_album("Parachutes"); album)
        std::cout << "Found: " << *album << "\n";
    else
        std::cout << "Unknown album\n";

    std::cout << "Searching for Song 'Daylight'... ";
    if (const auto* song = database.find_song("Daylight"); song)
        std::cout << "Found: " << *song << "\n";
    else
        std::cout << "Unknown song\n";

    std::cout << "Searching for anything named 'Supreme NTM'... ";
    if (const auto things = database.find_things("Supreme NTM"); !things.empty())
    {
        std::cout << "Found: " << things.size() << " things\n";
        for (const auto& thing_refwrap : things)
        {
            const auto& thing = thing_refwrap.get();
            std::cout << "Found: " << thing;
            if (const auto artist = dynamic_cast<const mdb::Artist*>(&thing); artist)
            {
                std::cout << " (albums: ";
                for (const auto& artist_album : artist->albums())
                {
                    std::cout << artist_album->name() << ", ";
                }
                std::cout << ')';
            }
            std::cout << '\n';
        }
    }
    else
        std::cout << "Nothing found\n";

    return static_cast<int>(result);
}
