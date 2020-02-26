
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <chrono>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>

namespace {

    // a tuple is similar to a struct, which data members don't have names
    // we access a tuple's data member with std::get and the index or type of the member
    // on can use structured bindings to access members via names
    TEST_CASE("usage of std::tuple", "[tuple][functional]")
    {
        using Tuple = std::tuple<int, char, double, std::string>;

        Tuple t{123, 'A', 45.67, "hello"};

        REQUIRE(t == Tuple{123, 'A', 45.67, "hello"});

        std::get<0>(t) = -78;

        REQUIRE(std::get<0>(t) == -78);
        REQUIRE(t == Tuple{-78, 'A', 45.67, "hello"});

        std::get<double>(t) = 1.2;

        REQUIRE(std::get<double>(t) == Approx(1.2));
        REQUIRE(t == Tuple{-78, 'A', 1.2, "hello"});

        const auto& [i, c, d, s] = t;

        REQUIRE(i == -78);
        REQUIRE(c == 'A');
        REQUIRE(d == Approx(1.2));
        REQUIRE(s == "hello");
    }

    struct Visitor
    {
        std::string operator()(int i) const
        {
            return fmt::format("The variant contains the value {} of type int", i);
        }

        std::string operator()(const std::string& s) const
        {
            return fmt::format("The variant contains the value {} of type std::string", s);
        }
    };

    // std::variant is a replacement to a C-style union
    // we access the variant' data with std::visit or std::get or std::get_if and the data type
    TEST_CASE("usage of std::variant", "[variant][functional]")
    {
        // If we construct a variant with default constructor,
        // then its value is the default value of the first type
        std::variant<int, std::string> v;

        REQUIRE(v.index() == 0);
        REQUIRE(std::holds_alternative<int>(v));
        REQUIRE(std::get<int>(v) == 0);

        v = 1234;

        int* pi = std::get_if<int>(&v);
        REQUIRE(pi != nullptr);
        REQUIRE(*pi == 1234);

        v = "hello"; // the value is now an std::string and equals "hello"

        pi = std::get_if<int>(&v);
        REQUIRE(pi == nullptr);

        std::string* ps = std::get_if<std::string>(&v);
        REQUIRE(ps != nullptr);
        REQUIRE(*ps == "hello");

        const auto visitation_result = std::visit(Visitor{}, v);
        REQUIRE(visitation_result == "The variant contains the value hello of type std::string");
    }

    // an optional<T> is an object which contains either something of type T or nothing (nullopt)
    class ConfigurationDatabase
    {
    public:
        void set(const std::string& name, const std::string& value)
        {
            m_settings[name] = value;
        }

        std::optional<std::string> get(const std::string& name) const
        {
            const auto iter_setting = m_settings.find(name);
            if (iter_setting == m_settings.end())
                return std::nullopt;
            const auto& value = iter_setting->second;
            return value;
        }

        std::string display_all() const
        {
            std::stringstream ss;
            for (const auto& [name, value] : m_settings)
            {
                ss << "Setting '" << name << "' = '" << value << "'\n";
            }
            return ss.str();
        }

    private:
        std::map<std::string, std::string> m_settings;
    };

    TEST_CASE("usage of std::optional", "[optional][functional]")
    {
        ConfigurationDatabase cdb;

        cdb.set("window.title", "hello world");
        cdb.set("window.width", "640");
        cdb.set("window.height", "480");
        cdb.set("backup.date_time", "2019/12/09 09:30:00");
        cdb.set("backup.file", "c:\\hello\\world.cpp");
        cdb.set("compiler", "msvc");

        const auto settings = cdb.display_all();

        REQUIRE(!settings.empty());

        const auto title = cdb.get("window.title");

        REQUIRE(title.has_value());
        REQUIRE(title.value() == "hello world");

        const auto unknown = cdb.get("unknown");

        REQUIRE(!unknown.has_value());
    }

} // namespace
