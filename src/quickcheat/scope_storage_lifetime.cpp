
// https://en.cppreference.com/w/cpp/language/scope
// https://en.cppreference.com/w/cpp/language/storage_duration
// https://en.cppreference.com/w/cpp/language/lifetime

// entity : object
// scope : global
// linkage : external (this object's name is visible and usable in any other source file)
// storage : static
// lifetime : from program's startup to program's end
// initialization : false
bool very_dirty_global_var;

// entity : object
// scope : global
// linkage : internal (this object's name is not visible outside this source file)
// storage : static
// lifetime : from program's startup to program's end
// initialization : {0, 0, 0, 0, 0}
static int dirty_global_var_hidden_from_other_source_files[5];

// entity : namespace
// scope : global
namespace my_namespace {

    // entity : object
    // scope : namespace
    // linkage : external
    // storage : static
    // lifetime : from program's startup to program's end
    // initialization : 0.0
    // usage: my_namespace::my_dirty_global_var = 12.34;
    double my_dirty_global_var;

    // entity : namespace
    // scope : namespace
    namespace my_sub_namespace {

        // entity : object
        // scope : namespace
        // linkage : external
        // storage : static
        // lifetime : from program's startup to program's end
        // initialization : 0
        // note : no name collision between
        //        my_namespace::my_sub_namespace::my_dirty_global_var and
        //        my_namespace::my_dirty_global_var as long as we use the full qualified name
        int my_dirty_global_var;

    } // namespace my_sub_namespace

} // namespace my_namespace

// entity : function
// scope : global
// linkage : external
unsigned int func(

    // entity : object
    // scope : function
    // linkage : none
    // storage : automatic (stack)
    // lifetime : from function's startup to function's end
    // initialization : during the copy of the argument each time the function is called
    unsigned int param)
{

    // entity : object
    // scope : block
    // linkage : none
    // storage : static
    // lifetime : from the first time a thread calls this function until the program's end
    // initialization : at creation time with value 101
    static unsigned int result = 101;

    if (param < 10)
    {

        // entity : object
        // scope : block
        // linkage : none
        // storage : automatic (stack)
        // lifetime : from the definition until the end of the scoping block
        // initialization : none (garbage)
        unsigned int carre;

        carre = param * param;

        result = carre;
    } // note : end of life of carre

    return result;
} // note : result does not dy here, it dies at program's end

// entity : namespace
// scope : global for foo, namespace for boom
namespace foo { namespace boom {

    // entity : anonymous
    // scope : namespace (foo::boom)
    namespace {

        // entity : class
        // scope : namespace (foo::boom)
        // linkage : internal
        struct S
        {
        };

        // entity : function
        // scope : namespace (foo::boom)
        // linkage : internal
        void func()
        {
        }

        // entity : object
        // scope : namespace (foo::boom)
        // linkage : internal
        S s;

    } // namespace

    // entity : class
    // scope : namespace
    // linkage : external
    class C
    {

        // entity : member function
        // scope : class
        // linkage : external
        // usage : C c; c.member_function();
        void member_function()
        {
        }

        // entity : static member function
        // scope : class
        // linkage : external
        // usage : C::static_member_function();
        static void static_member_function()
        {
        }

        // entity : data member
        // scope : class block
        // linkage : external
        // storage : automatic (i.e. same storage as the enclosing object)
        // lifetime : from the enclosing object's birth to the enclosing object's death
        // initialization : none (garbage) (unless the enclosing object has a constructor which initializes the member)
        char data_member;

        // entity : data member
        // scope : class block
        // linkage : external
        // storage : automatic (i.e. same storage as the enclosing object)
        // lifetime : from the enclosing object's birth to the enclosing object's death
        // initialization : direct (can be overwritten by a constructor)
        int data_member_nsmdi{42};

        // entity : static data member
        // scope : class block
        // linkage : external
        // storage : static
        // lifetime : from program's startup to program's end
        static double static_data_member;
    };

    // initialization : mandatory and outside the class' definition
    double C::static_data_member = 3.14;

}} // namespace foo::boom

// entity : namespace
// scope : global
namespace babar {

    // entity : function
    // scope : namespace
    // linkage : internal (because of the keyword static)
    static void autre_fonc()
    {
        // entity : object (a pointer is still an object)
        // scope : block
        // linkage : none
        // storage : automatic (i.e stack)
        // lifetime : from the definition until the closing brace
        // initialization : none (garbage)
        foo::boom::C* pC;

        pC =
            // entity : object (dynamically allocated)
            // scope : none
            // linkage : none (because not named)
            // storage : dynamic (i.e. heap)
            // lifetime : from the call to new until the call to delete
            // initialization : none (garbage) unless the C's default constructor does it
            new foo::boom::C;

        // ... use the object through pC

        // note: end of life of the dynamic object pointed by pC
        delete pC;

    } // note : end of life of pC

} // namespace babar

// entity : object
// scope : global
// linkage : external
// storage : thread (one instance of this variable is created for each created thread)
// lifetime : from the thread's startup until the thread's end
// initialization : 0
thread_local int onePerThread;

// entity : object
// scope : global
// linkage : external
// storage : static
// lifetime : from the program's startup until the program's end
// initialization : none (garbage) unless the C's default constructor does it
foo::boom::C c;
