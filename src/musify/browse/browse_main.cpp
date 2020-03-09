
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
        mdb::display_database(database);
        break;
    }

    std::cout << '\n';
    std::cout << "Searching for artist 'Oasis'... ";
    if (const mdb::Artist* artist = mdb::find_artist(database, "Oasis"); artist)
        std::cout << "Found: " << *artist << "\n";
    else
        std::cout << "Unknown artist\n";

    std::cout << "Searching for Album 'Parachutes'... ";
    if (const auto* album = mdb::find_album(database, "Parachutes"); album)
        std::cout << "Found: " << *album << "\n";
    else
        std::cout << "Unknown album\n";

    std::cout << "Searching for Song 'Daylight'... ";
    if (const auto* song = mdb::find_song(database, "Daylight"); song)
        std::cout << "Found: " << *song << "\n";
    else
        std::cout << "Unknown song\n";

    return static_cast<int>(result);
}
