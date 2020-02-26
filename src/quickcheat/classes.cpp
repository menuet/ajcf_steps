
// https://en.cppreference.com/w/cpp/language/class c.f. Peter Sommerlad
// https://en.cppreference.com/w/cpp/memory/unique_ptr

#include <string_view>
#include <catch2/catch.hpp>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <memory>

namespace {

    namespace classes_value {

        class Regular
        {
        public:
            Regular() : m_pi(std::make_unique<int>(0))
            {
            }

            explicit Regular(int i) : m_pi(std::make_unique<int>(i))
            {
            }

            Regular(const Regular& autre) : m_pi(std::make_unique<int>(*autre.m_pi))
            {
            }

            Regular(Regular&& autre) = default;

            ~Regular() = default;

            Regular& operator=(const Regular& autre)
            {
                Regular(autre).swap(*this);
                return *this;
            }

            Regular& operator=(Regular&& autre) = default;

            void swap(Regular& autre)
            {
                m_pi.swap(autre.m_pi);
            }

            int value() const
            {
                return *m_pi;
            }

            bool operator==(const Regular& autre) const
            {
                return *m_pi == *autre.m_pi;
            }

            Regular& operator+=(const Regular& y)
            {
                *m_pi += *y.m_pi;
                return *this;
            }

            friend inline Regular operator+(Regular x, const Regular& y)
            {
                x += y;
                return x;
            }

            friend inline std::ostream& operator<<(std::ostream& os, const Regular& x)
            {
                os << *x.m_pi;
                return os;
            }

        private:
            std::unique_ptr<int> m_pi;
        };

        TEST_CASE("regular", "[classes]")
        {
            Regular reg1{3};

            REQUIRE(reg1.value() == 3);

            Regular reg2{5};

            REQUIRE(reg2.value() == 5);

            const auto reg3 = reg1 + reg2;

            REQUIRE(reg3.value() == 3 + 5);
        }

    } // namespace classes_value

    namespace classes_polymorphic {

        class BaseWithoutVirtual
        {
        public:
            std::string who_am_i()
            {
                return "BaseWithoutVirtual";
            }
        };

        class DerivedWithoutVirtual : public BaseWithoutVirtual
        {
        public:
            std::string who_am_i()
            {
                return "DerivedWithoutVirtual";
            }
        };

        class BaseWithVirtual
        {
        public:
            virtual ~BaseWithVirtual() = default;

            virtual std::string who_am_i()
            {
                return "BaseWithVirtual";
            }
        };

        class DerivedWithVirtual : public BaseWithVirtual
        {
        public:
            std::string who_am_i() override
            {
                return "DerivedWithVirtual";
            }
        };

        std::string who_is_it_by_ref(BaseWithoutVirtual& base)
        {
            return base.who_am_i();
        }

        std::string who_is_it_by_ref(BaseWithVirtual& base)
        {
            return base.who_am_i();
        }

        std::string who_is_it_by_copy(BaseWithoutVirtual base)
        {
            return base.who_am_i();
        }

        std::string who_is_it_by_copy(BaseWithVirtual base)
        {
            return base.who_am_i();
        }

        TEST_CASE("with and without virtual", "[classes]")
        {
            DerivedWithoutVirtual derived_without_virtual;

            REQUIRE(derived_without_virtual.who_am_i() == "DerivedWithoutVirtual");
            REQUIRE(who_is_it_by_ref(derived_without_virtual) == "BaseWithoutVirtual");
            REQUIRE(who_is_it_by_copy(derived_without_virtual) == "BaseWithoutVirtual");

            DerivedWithVirtual derived_with_virtual;

            REQUIRE(derived_with_virtual.who_am_i() == "DerivedWithVirtual");
            REQUIRE(who_is_it_by_ref(derived_with_virtual) == "DerivedWithVirtual");
            REQUIRE(who_is_it_by_copy(derived_with_virtual) == "BaseWithVirtual");
        }

    } // namespace classes_polymorphic

    namespace classes_resource_manager {

        class FileRAIIWithoutUP
        {
        public:
            FileRAIIWithoutUP(const char* file_name, const char* mode) : m_c_file(std::fopen(file_name, mode))
            {
            }

            ~FileRAIIWithoutUP()
            {
                if (m_c_file)
                    std::fclose(m_c_file);
            }

            FileRAIIWithoutUP(const FileRAIIWithoutUP&) = delete;

            FileRAIIWithoutUP& operator=(const FileRAIIWithoutUP&) = delete;

            std::FILE* get() const
            {
                return m_c_file;
            }

            explicit operator bool() const
            {
                return m_c_file != nullptr;
            }

        private:
            std::FILE* m_c_file;
        };

        class FileRAIIWithUP
        {
        public:
            FileRAIIWithUP(const char* file_name, const char* mode) : m_c_file(std::fopen(file_name, mode))
            {
            }

            std::FILE* get() const
            {
                return m_c_file.get();
            }

            explicit operator bool() const
            {
                return static_cast<bool>(m_c_file);
            }

        private:
            struct FileCloser
            {
                void operator()(std::FILE* c_file) const
                {
                    std::fclose(c_file);
                }
            };

            std::unique_ptr<std::FILE, FileCloser> m_c_file;
        };

        TEST_CASE("RAII", "[classes]")
        {
            INFO("Trying opening file");

            FileRAIIWithUP file("my_file.txt", "r");
            if (file)
            {
                INFO("Reading the first 49 bytes of the file");

                char buffer[50]{};
                const auto read_bytes_count = std::fread(buffer, 1, 49, file.get());

                REQUIRE(read_bytes_count <= 49);
            }
            else
            {
                REQUIRE(!file);
            }

            INFO("File's destructor closes the underlying file automatically when exiting the function");
        }

    } // namespace classes_resource_manager

} // namespace
