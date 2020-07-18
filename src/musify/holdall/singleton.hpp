
#pragma once

namespace musify::singleton {

    template <typename T>
    class Singleton
    {
        T m_obj{};

        Singleton() = default;

    public:
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator=(Singleton&&) = delete;

        static T& get_instance()
        {
            static Singleton<T> unique_instance{};
            return unique_instance.m_obj;
        }
    };

} // namespace musify::singleton
