
#include <catch2/catch.hpp>
#include <cstring>
#include <string>

namespace {

    TEST_CASE("explicit conversions", "[conversions]")
    {
        double d = 12.34;

        int i = (int)d; // C-style conversion
        // note: why should you avoid C-style conversions?
        //       because it allows different kinds of conversions via a unique ambiguous syntax

        REQUIRE(i == 12);

        // C++-style conversion that tells more information about the kind of conversion:
        // this is a narrowing conversion: we know that we can loose precision
        int j = static_cast<int>(d);

        REQUIRE(j == 12);

        const char c = 'A';
        // C++-style conversion that tells more information about the kind of conversion:
        // this is a const conversion
        // very dangerous, because we can now modify an immutable memory zone
        char mutable_c = const_cast<char&>(c);

#if 0
        mutable_c = 'B'; // This would be Undefined Behavior!!!
#endif

        REQUIRE(mutable_c == 'A');

        struct Base
        {
            virtual ~Base() = default;
            virtual std::string what() const
            {
                return "Base";
            }
        };

        struct Derived : Base
        {
            std::string what() const override
            {
                return "Derived";
            }
        };

        Derived derived;
        Base& base = derived;
        // C++-style conversion that tells more information about the kind of conversion:
        // this is a conversion from a pointer to Base to a pointer to Derived
        // this may return nullptr in case the pointer to Base does not really point to a concret Derived
        Derived* derived_again = dynamic_cast<Derived*>(&base);

        REQUIRE(derived_again != nullptr);
        REQUIRE(derived_again->what() == "Derived");

        char c_style_string[] = "ABC";

        // warning: you should avoid using void*
        // it loses type information
        void* pointer_to_anything = c_style_string;

        // C++-style conversion that tells more information about the kind of conversion:
        // this is a very dangerous conversion
        // that reinterprets the type of an object as another type
        // You should avoid using reinterpret_cast
        char* pointer_to_chars = reinterpret_cast<char*>(pointer_to_anything);

        REQUIRE(std::strcmp(pointer_to_chars, "ABC") == 0);
    }

    void func(std::string s)
    {
    }

    class ConvertibleThing
    {
    public:
        // implicitly convertible from int
        ConvertibleThing(int v) : m_i(v)
        {
        }

        // explicitely convertible from double
        explicit ConvertibleThing(double v) : m_i(static_cast<int>(v))
        {
        }

        // implicitely convertible to double
        operator double() const
        {
            return m_i;
        }

        // explicitely convertible to std::string
        explicit operator std::string() const
        {
            return std::to_string(m_i);
        }

    private:
        int m_i;
    };

    void take_ref_to_non_mutable_convertible_thing(const ConvertibleThing& ct)
    {
        // ct can be copied implicitely to a double because of implicit ConvertibleThing::operator double()
        double d = ct;

        // ct can be copied only explicitely (with a cast) because of explicit ConvertibleThing::operator
        // std::string()
        std::string s = static_cast<std::string>(ct);
    }

    TEST_CASE("implicit conversions and temporary objects", "[conversions]")
    {
        // the compiler creates a temporary anonymous string object
        // pass it to the function that want a string object
        // and then, when the func is finished, it destroys the tempoary object
        // (this has a non-explicit impact on performance)
        func("hello");

        // We can show the reader that there is a created tempoary object
        // by using an explicit syntax for temporary objects creation
        func(std::string("ola"));

        // compiler creates implicitely a temporary ConvertibleThing object
        take_ref_to_non_mutable_convertible_thing(123);

        // equivalent to:
        take_ref_to_non_mutable_convertible_thing(ConvertibleThing{123});

        // equivalent to:
        {
            const ConvertibleThing small_lifetime_object{123};
            take_ref_to_non_mutable_convertible_thing(small_lifetime_object);
        }

        // compiler is not allowed to implicitely create a temporary object
#if 0
        take_ref_to_non_mutable_convertible_thing(12.45); // compilation error
#endif

        // we can however tell the compiler we want this conversion
        take_ref_to_non_mutable_convertible_thing(ConvertibleThing{12.45});
    }

} // namespace
