
// https://en.cppreference.com/w/cpp/language/new
// https://en.cppreference.com/w/cpp/language/delete

#include <catch2/catch.hpp>
#include <memory>
#include <vector>

namespace {

    struct S
    {
        S() = default;
        S(int i, int j) : i(i), j(j)
        {
        }

        int i{0};
        int j{0};
    };

    TEST_CASE("single-object allocation with throwing new", "[dynamic][allocation]")
    {
        // return the address of the allocated object
        // or throw std::bad_alloc exception if no memory available
        S* p = new S{1, 2};

        // Use allocated object
        REQUIRE(p->i == 1);
        REQUIRE(p->j == 2);

        // Destroy allocated object
        delete p;
    }

    TEST_CASE("single-object allocation with non-throwing new", "[dynamic][allocation]")
    {
        // return the address of the allocated object
        // or return nullptr if no memory available
        S* p = new (std::nothrow) S{1, 2};
        if (!p)
            return;

        // Use allocated object
        REQUIRE(p->i == 1);
        REQUIRE(p->j == 2);

        // Destroy allocated object
        delete p;
    }

    TEST_CASE("single-object allocation with std::make_unique", "[dynamic][allocation]")
    {
        // call new behind the scene
        // and store the resulting pointer as a member of the returned unique_ptr object
        std::unique_ptr<S> p = std::make_unique<S>(1, 2);

        // Use allocated object
        REQUIRE(p->i == 1);
        REQUIRE(p->j == 2);

    } // call delete behind the scene automatically in the unique_ptr object's destructor

    TEST_CASE("array allocation with new[]", "[dynamic][allocation]")
    {
        // return the address of the first object of the allocated array
        // or throw std::bad_alloc exception if no memory available
        S* p = new S[50];

        // Use any object of the allocated array
        REQUIRE(p[3].i == 0);
        REQUIRE(p[3].j == 0);

        delete[] p; // Careful: need to use delete[] or it is UB !!!
    }

    TEST_CASE("array allocation with std::make_unique", "[dynamic][allocation]")
    {
        // call new[50] behind the scene
        // and store the resulting pointer as a member of the returned unique_ptr object
        std::unique_ptr<S[]> p = std::make_unique<S[]>(50);

        // Use any object of the allocated array
        REQUIRE(p[3].i == 0);
        REQUIRE(p[3].j == 0);

    } // call delete[] behind the scene automatically in the unique_ptr object's destructor

    TEST_CASE("resizable array allocation with std::vector", "[dynamic][allocation]")
    {
        // call new[50] behind the scene
        // and store the resulting pointer as a member of the vector object
        std::vector<S> v(50);

        // Use any object of the allocated array
        REQUIRE(v[3].i == 0);
        REQUIRE(v[3].j == 0);

        v[12] = S{12, 34};

        // call new[100] behind the scene
        // copy all the objects of the old memory area to the new area
        // call delete[] behind the scene for the old area
        v.resize(100);

        // Use any object of the allocated array
        REQUIRE(v[12].i == 12);
        REQUIRE(v[12].j == 34);
        REQUIRE(v[75].i == 0);
        REQUIRE(v[75].j == 0);

    } // call delete[] behind the scene automatically in the vector object's destructor

} // namespace
