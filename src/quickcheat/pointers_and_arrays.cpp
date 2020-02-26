
// https://en.cppreference.com/w/cpp/language/pointer
// https://en.cppreference.com/w/cpp/language/array

#include <catch2/catch.hpp>
#include <gsl/span>
#include <array>
#include <iostream>

namespace {

    void do_not_modify_original_argument(
        // Definition of obj_param
        // creates a memory zone, each time the function is called,
        // which contains a copy of the value of the object passed as argument
        int obj_param)
    {
        // obj_param is modified
        // but this has not any effect on the object passed as argument
        obj_param = obj_param + 1;
    }

    void modify_object_which_address_is_passed_as_argument(
        // Definition of ptr_param
        // creates a memory zone, each time the function is called,
        // which contains the address of the passed argument.
        // Si the function modifies the value at the address pointed to by ptr_param,
        // then this modifies the value of the passed argument.
        int* ptr_param)
    {
        // we access the pointed object with operator *
        // then we assign a new value to the pointed object
        *ptr_param = 456;
    }

    TEST_CASE("pointers and arrays", "[pointers][arrays]")
    {
        // Definition of obj
        // reserves a memory zone for an integer on the stack
        // and assigns the value 3 to it
        int obj = 3;

        REQUIRE(obj == 3);

        // Definition of ptr
        // reserves a memory zone for storing an address of an integer
        // the type of ptr is "int*" (pointer to integer)
        // ptr points to the object obj because we assign its address to it
        int* ptr = &obj; // note: adress of an object is obtained with operator &

        REQUIRE(ptr == &obj);
        REQUIRE(*ptr == 3);

        int obj2 = 987;

        REQUIRE(obj2 == 987);

        // if we modify the value of ptr
        // then we make it point to another object (here obj2)
        // therefore we do not modify the value of obj
        ptr = &obj2;

        // now, the value of ptr has changed:
        // from "address of obj" to "address of obj2"
        REQUIRE(ptr != &obj);
        REQUIRE(ptr == &obj2);
        REQUIRE(*ptr == 987);

        // We give obj as argument to the called function
        // It uses a copy of the value of obj
        // in a dedicated memory zone separated from obj's memory zone
        do_not_modify_original_argument(obj);

        // obj was not modified, it still equals 3
        REQUIRE(obj == 3);

        // We give the address of obj to the called function
        // It uses a pointer containing the address of the memory zone of obj,
        // therefore, the called function can modified indirectly the value of obj
        modify_object_which_address_is_passed_as_argument(&obj);

        // now, obj equals 456
        REQUIRE(obj == 456);

        // We give the value of ptr (which contains the address of obj2) to the called function
        // It uses a pointer containing the same value (address of obj2),
        // therefore, the called function can modified indirectly the value of obj2
        modify_object_which_address_is_passed_as_argument(ptr);

        // now, obj2 equals 456
        REQUIRE(obj2 == 456);
        REQUIRE(ptr == &obj2);
        REQUIRE(*ptr == 456);

        // Definition of tab
        // reserves a memory zone of 3 contiguous integers on the stack
        int tab[3] = {10, 20, 30};

        REQUIRE(tab[0] == 10);
        REQUIRE(tab[1] == 20);
        REQUIRE(tab[2] == 30);

        // Definition of pint
        // reserves a memory zone on the stack
        // containing the address of the first element of tab
        // One say that an array can "decay" to a pinter
        // (tab can be mentaly viewed as a pointer to its first element)
        const int* pint = tab;

        // pint now contains the address of the first element of tab,
        // which contains the value 10
        REQUIRE(pint == tab);
        REQUIRE(*pint == 10);
        REQUIRE(tab[0] == 10);

        // One can increment the pointer
        // so that its value contains the address of the next integer in memory

        ++pint;

        // now point contains the address of the second element of tab
        REQUIRE(pint == tab + 1);
        REQUIRE(*pint == 20);
        REQUIRE(tab[1] == 20);

        ++pint;

        // now pint contains the address of the third element of tab
        REQUIRE(pint == tab + 2);
        REQUIRE(*pint == 30);
        REQUIRE(tab[2] == 30);

        ++pint;

        // now pint contains the address of the virtual element just after the last element of tab
        REQUIRE(pint == tab + 3);
#if 0
        // UB !!! On try to access to the memory zone at the address contained in pint
        // The compiler may not warn about it, but the program's behavior is undefined and cannot be predicated
        REQUIRE(*pint == ?);
#endif
    }

    struct S
    {
        int i;
        double d;
    };

    void example_access_to_member_through_pointer()
    {
        S s{1, 45.5};

        S* ps = &s;

        ps->i = 5;
        ps->d = 3.2;
    }

    TEST_CASE("C arrays", "[arrays]")
    {
        double tab[5] = {1.5, 5.2, 7.5, 9.2, 1.5};

        // One can know the size in bytes of an array with operator sizeof
        REQUIRE(sizeof(tab) == 40);

        // One can know the number of elements of an array with this formula
        constexpr auto taille_tab = sizeof(tab) / sizeof(tab[0]);

        REQUIRE(taille_tab == 5);

        // one can access elements with operator[]
        REQUIRE(tab[0] == Approx(1.5));
        REQUIRE(tab[3] == Approx(9.2));

        // one can get the address of an element by adding the index
        REQUIRE(tab + 2 == &tab[2]);
    }

    // takes a reference to an array of 3 elements
    int sum_of(int (&a)[3])
    {
        return a[0] + a[1] + a[2];
    }

    // takes a pointer to an integer
    int get_value_of_element_of_index_4(int* p)
    {
        // with a pointer, one can also use the indexed-access syntax, like an array
        return p[4];
        // but nothing prevents us from accessing non-allocated memory outside the bounds
    }

    TEST_CASE("C array decays to pointer", "[arrays][pointers]")
    {
        int tab[3] = {1, 2, 3};

        REQUIRE(sum_of(tab) == 1 + 2 + 3);

#if 0
        const auto value_at_index_4 = get_value_of_element_of_index_4(tab); // UB!!!
#endif
    }

    TEST_CASE("C++ std arrays", "[arrays]")
    {
        std::array arr{1.5, 5.2, 7.5, 9.2, 1.5};
        //        std::array<double, 5> arr{1.5, 5.2, 7.5, 9.2, 1.5}; // without deduction guides

        // One can know the size in bytes of an array with operator sizeof
        REQUIRE(sizeof(arr) == 40); // 5 * 8 bytes

        // One can know the number of elements of an array with its member function size()
        constexpr auto arr_count = arr.size();

        REQUIRE(arr_count == 5);

        // one can access elements with operator[]
        REQUIRE(arr[0] == Approx(1.5));
        REQUIRE(arr[3] == Approx(9.2));

        // one can get the address of the first element with data()
        REQUIRE(arr.data() + 2 == &arr[2]);
    }

    TEST_CASE("C++ std span", "[span]")
    {
        std::array arr{1.5, 5.2, 7.5, 9.2, 1.5}; // array owns its elements

        gsl::span spn{arr}; // span does not own its elements

        // One can know the number of elements in the span size()
        constexpr auto spn_count = spn.size();

        REQUIRE(spn_count == 5);

        // one can access elements with operator[]
        REQUIRE(spn[0] == Approx(1.5));
        REQUIRE(spn[3] == Approx(9.2));

        // one can get the address of the first element with data()
        REQUIRE(spn.data() + 2 == &spn[2]);
    }

} // namespace
