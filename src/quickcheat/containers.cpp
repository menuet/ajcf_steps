
// https://en.cppreference.com/w/cpp/container

#include <type_traits>
#include <unordered_map>
#include <catch2/catch.hpp>
#include <list>
#include <map>
#include <utility>
#include <vector>

namespace {

    TEST_CASE("vector", "[containers]")
    {
        INFO("vector: automatically-resizing dynamic array of contiguous elements");
        INFO("        this is the container to use by default");

        // initialize with a list of values
        // element's type is deduced as int
        // vec's type is deduced as std::vector<int>
        std::vector vec{1, 2, 3, 4, 5, 6, 7};

        REQUIRE(vec == std::vector<int>{1, 2, 3, 4, 5, 6, 7});

        // iterate
        int i = 1;
        for (auto iter = vec.begin(); iter != vec.end(); ++iter)
        {
            const auto& value = *iter;
            REQUIRE(value == i);
            ++i;
        }

        // append to the end
        vec.push_back(45);
        vec.push_back(23);

        REQUIRE(vec == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 45, 23});

        // remove a the end
        vec.pop_back();

        REQUIRE(vec == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 45});

        // insert in the middle
        const auto iter_4th = next(vec.begin(), 3);
        vec.insert(iter_4th, 123);

        REQUIRE(vec == std::vector<int>{1, 2, 3, 123, 4, 5, 6, 7, 45});

        // remove in the middle
        const auto iter_6th = next(vec.begin(), 5);
        vec.erase(iter_6th);

        REQUIRE(vec == std::vector<int>{1, 2, 3, 123, 4, 6, 7, 45});

        // access at the end
        REQUIRE(vec.back() == 45);

        // access at the beginning
        REQUIRE(vec.front() == 1);

        // access by index
        REQUIRE(vec[3] == 123);
    }

    TEST_CASE("list", "[containers]")
    {
        INFO("list : linked-list");
        INFO("       you should avoid using it");

        // initialize
        std::list lst{1, 2, 3, 4, 5, 6, 7};

        REQUIRE(lst == std::list<int>{1, 2, 3, 4, 5, 6, 7});

        // iterate
        int i = 1;
        for (auto iter = lst.begin(); iter != lst.end(); ++iter)
        {
            const auto& value = *iter;
            REQUIRE(value == i);
            ++i;
        }

        // append at the end
        lst.push_back(45);

        REQUIRE(lst == std::list<int>{1, 2, 3, 4, 5, 6, 7, 45});

        // append at the beginning
        lst.push_front(98);

        REQUIRE(lst == std::list<int>{98, 1, 2, 3, 4, 5, 6, 7, 45});

        // remove at the end
        lst.pop_back();

        REQUIRE(lst == std::list<int>{98, 1, 2, 3, 4, 5, 6, 7});

        // remove at the beginning
        lst.pop_front();

        REQUIRE(lst == std::list<int>{1, 2, 3, 4, 5, 6, 7});

        // insert in the middle (fast if you already have an iterator)
        const auto iter_4th = next(lst.begin(), 3);
        lst.insert(iter_4th, 123);

        REQUIRE(lst == std::list<int>{1, 2, 3, 123, 4, 5, 6, 7});

        // remove in the middle (fast if you already have an iterator)
        const auto iter_6th = next(lst.begin(), 5);
        lst.erase(iter_6th);

        REQUIRE(lst == std::list<int>{1, 2, 3, 123, 4, 6, 7});

        // access at the end
        REQUIRE(lst.back() == 7);

        // access at the beginning
        REQUIRE(lst.front() == 1);
    }

    TEST_CASE("map", "[containers]")
    {
        INFO("map: sorted dictionary of elements");
        INFO("     use it when you have keys/values pairs");

        // initialize
        std::map<int, double> map{{3, 2.3}, {2, 5.5}, {1, 8.1}, {5, 9.1}, {7, 78.45}, {4, 32.12}, {6, 56.}};

        REQUIRE(map == std::map<int, double>{{1, 8.1}, {2, 5.5}, {3, 2.3}, {4, 32.12}, {5, 9.1}, {6, 56.}, {7, 78.45}});

        // iterate
        int i = 1;
        for (auto iter = map.begin(); iter != map.end(); ++iter)
        {
            const auto& key_value = *iter;
            REQUIRE(key_value.first == i);
            ++i;
        }

        // insert key/value
        map.insert({123, 45.67});

        REQUIRE(map == std::map<int, double>{
                           {1, 8.1}, {2, 5.5}, {3, 2.3}, {4, 32.12}, {5, 9.1}, {6, 56.}, {7, 78.45}, {123, 45.67}});

        // insert or update key's value
        map[123] = 3.333;
        map[42] = 1.2;

        REQUIRE(
            map ==
            std::map<int, double>{
                {1, 8.1}, {2, 5.5}, {3, 2.3}, {4, 32.12}, {5, 9.1}, {6, 56.}, {7, 78.45}, {42, 1.20}, {123, 3.333}});

        // remove with iterator
        const auto iter_2nd = next(map.begin(), 1);
        map.erase(iter_2nd);

        REQUIRE(map == std::map<int, double>{
                           {1, 8.1}, {3, 2.3}, {4, 32.12}, {5, 9.1}, {6, 56.}, {7, 78.45}, {42, 1.20}, {123, 3.333}});

        // remove with key
        map.erase(4);

        REQUIRE(map ==
                std::map<int, double>{{1, 8.1}, {3, 2.3}, {5, 9.1}, {6, 56.}, {7, 78.45}, {42, 1.20}, {123, 3.333}});
    }

    TEST_CASE("unordered_map", "[containers]")
    {
        INFO("unordered_map: hash table");
        INFO("               faster than map, use it when you have keys/values pairs and don't need sorted keys");

        // initialize
        std::unordered_map<int, double> umap{{1, 2.3}, {2, 5.5}, {3, 8.1}, {4, 9.1}, {5, 78.45}, {6, 32.12}, {7, 56.}};

        REQUIRE(umap == std::unordered_map<int, double>{
                            {1, 2.3}, {2, 5.5}, {3, 8.1}, {4, 9.1}, {5, 78.45}, {6, 32.12}, {7, 56.}});

        // iterate
        for (auto iter = umap.begin(); iter != umap.end(); ++iter)
        {
            const auto& key_value = *iter;
            REQUIRE(key_value.first >= 1);
            REQUIRE(key_value.first <= 7);
        }

        // insert key/value
        umap.insert({123, 45.67});

        REQUIRE(umap == std::unordered_map<int, double>{
                            {1, 2.3}, {2, 5.5}, {3, 8.1}, {4, 9.1}, {5, 78.45}, {6, 32.12}, {7, 56.}, {123, 45.67}});

        // insert or update key's value
        umap[123] = 3.333;
        umap[42] = 1.2;

        REQUIRE(umap ==
                std::unordered_map<int, double>{
                    {1, 2.3}, {2, 5.5}, {3, 8.1}, {4, 9.1}, {5, 78.45}, {6, 32.12}, {7, 56.}, {123, 3.333}, {42, 1.2}});

        // remove with key
        umap.erase(4);

        REQUIRE(umap == std::unordered_map<int, double>{
                            {1, 2.3}, {2, 5.5}, {3, 8.1}, {5, 78.45}, {6, 32.12}, {7, 56.}, {123, 3.333}, {42, 1.2}});

        // remove with iterator
        const auto iter_2nd = next(umap.begin(), 1);
        umap.erase(iter_2nd);
        // Cannot know which element was removed because elements are not sorted
    }

} // namespace
