
// https://en.cppreference.com/w/cpp/language/reference

#include <catch2/catch.hpp>
#include <vector>

namespace { namespace references {

    void do_not_modify_the_object_passed_as_argument(
        // Definition of obj_param
        // reserves a memory zone on the stack
        // which contains a value copied from the value of the passed argment
        int obj_param)
    {
        // obj_param is modified
        // but this has not any effect on the object passed as argument
        obj_param = obj_param + 1;
    }

    void modify_object_passed_as_argument(
        // Definition of ref_param
        // creates an alias to the same memory zone as the passed argument's one
        // every modification on ref_param is applied to the passed argument itself
        int& ref_param)
    {
        // ref_param is modified, therefore the passed argument's value is modified too
        ref_param = ref_param + 1;
    }

    TEST_CASE("l-value references", "[references]")
    {
        // Definition of obj
        // reserves a memory zone for an integer on the stack
        // and assigns the value 3 to it
        int obj = 3;

        REQUIRE(obj == 3);

        // Definition of ref
        // does not reserves another memory zone,
        // it just creates an alias,
        // ref and obj are now two different names of the same memory zone
        int& ref = obj;

        REQUIRE(&ref == &obj);
        REQUIRE(ref == 3);

        // If we modify the value of ref
        // then we modify the value at the same memory zone as obj,
        // therefore we modify obj's value too
        ref = 4;

        // now obj equals 4 too
        REQUIRE(ref == 4);
        REQUIRE(obj == 4);

        // We give obj as argument to the called function
        // It uses a copy of the value of obj
        // in a dedicated memory zone separated from obj's memory zone
        do_not_modify_the_object_passed_as_argument(obj);

        // obj was not modified, it still equals 4
        REQUIRE(ref == 4);
        REQUIRE(obj == 4);

        // We give obj as argument to the called function
        // it does not use a copy of obj, it uses a reference to the same memory zone as obj
        modify_object_passed_as_argument(obj);

        // now, obj equals 5 (and ref equals 5, too)
        REQUIRE(obj == 5);
        REQUIRE(ref == 5);
    }

    std::vector<int> create_vector(int count)
    {
        std::vector<int> vec;
        for (int i = 0; i < count; ++i)
            vec.push_back(i);
        return vec;
    }

    std::vector<int> global_vec;

    void steal_vector_data(std::vector<int>&& temp_vec)
    {
        INFO("before tansfert:");
        CAPTURE(temp_vec);
        CAPTURE(global_vec);

        // global_vec steal the dynamically-allocated data of temp_vec
        // which does not need them because it will not be used anymore
        global_vec = std::move(temp_vec);

        INFO("after tansfert:");
        CAPTURE(global_vec);
    }

    TEST_CASE("r-value references", "[references]")
    {
        // One can call steal_vector_data
        // by giving as argument the returned value of another function.
        // the returned value of create_vector is a temporary transient object
        // which nobody uses except steal_vector_data
        INFO("calling steal_vector_data with some function's returned value:");
        steal_vector_data(create_vector(10));

        REQUIRE(global_vec == std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

        // One can also call steal_vector_data
        // by giving as argument a temporay anonymous object
        INFO("calling steal_vector_data with an anonymous temporary objet:");
        steal_vector_data(std::vector<int>{1, 2, 3});

        REQUIRE(global_vec == std::vector{1, 2, 3});

        std::vector<int> vec = create_vector(6);

        REQUIRE(vec == std::vector{0, 1, 2, 3, 4, 5});

#if 0
            INFO("calling  steal_vector_data with a named object (impossible)");
            // The compiler forbids to call steal_vector_data because vec is a non-temporary named objet
            steal_vector_data(vec);
#endif

        // One can also call steal_vector_data
        // by giving as argument a named object nested inside the special function std::move
        INFO("calling steal_vector_data with named object nested in std::move:");
        steal_vector_data(std::move(vec));

        // Usually, we must not access afterward an object which was nested in std::move
        // because it can now be in a non-usable state.
        // Some objects of some classes like std::vector can still be used,
        // because they are guaranteed to be in a usable-but-empty state
        REQUIRE(vec.empty());

        REQUIRE(global_vec == std::vector{0, 1, 2, 3, 4, 5});
    }

}} // namespace ::references
