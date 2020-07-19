
#pragma once

#include <type_traits>
#include <algorithm>
#include <iterator>
#include <memory>
#include <mutex>
#include <vector>

namespace musify::observer {

    template <typename EventT>
    struct Observer
    {
        virtual ~Observer() = default;
        virtual void update(const EventT& event) = 0;
    };

    template <typename EventT>
    struct Observable
    {
        void attach(const std::shared_ptr<Observer<EventT>>& observer)
        {
            m_observers.push_back(observer);
        }

        void detach(const std::shared_ptr<Observer<EventT>>& observer)
        {
            const auto iter = std::find_if(m_observers.begin(), m_observers.end(), [&](const auto& weak_observer) {
                const auto shared_observer = weak_observer.lock();
                return shared_observer && shared_observer == observer;
            });
            if (iter == m_observers.end())
                return;
            m_observers.erase(iter);
        }

        void notify(const EventT& event)
        {
            for (const auto& weak_observer : m_observers)
            {
                const auto shared_observer = weak_observer.lock();
                if (shared_observer)
                    shared_observer->update(event);
            }
        }

    private:
        std::vector<std::weak_ptr<Observer<EventT>>> m_observers;
    };

    template <typename EventT, typename CallableT>
    struct ObserverFromCallable : Observer<EventT>
    {
        template <typename AnyCallableT>
        ObserverFromCallable(AnyCallableT&& callable) : m_callable{std::forward<AnyCallableT>(callable)}
        {
        }

        void update(const EventT& event) override
        {
            m_callable(event);
        }

    private:
        CallableT m_callable;
    };

    template <typename EventT, typename CallableT>
    std::shared_ptr<ObserverFromCallable<EventT, std::decay_t<CallableT>>> make_observer(CallableT&& callable)
    {
        return std::make_shared<ObserverFromCallable<EventT, std::decay_t<CallableT>>>(
            std::forward<CallableT>(callable));
    }

    template <typename EventT>
    struct ThreadSafeObservable
    {
        using Observers = std::vector<std::weak_ptr<Observer<EventT>>>;

        void attach(const std::shared_ptr<Observer<EventT>>& observer)
        {
            std::lock_guard lock{m_mutex};

            auto new_spobservers = std::make_shared<Observers>();

            *new_spobservers = *m_spobservers;
            new_spobservers->push_back(observer);

            m_spobservers = new_spobservers;
        }

        void detach(const std::shared_ptr<Observer<EventT>>& observer)
        {
            std::lock_guard lock{m_mutex};

            const auto iter =
                std::find_if(m_spobservers->begin(), m_spobservers->end(), [&](const auto& weak_observer) {
                    const auto shared_observer = weak_observer.lock();
                    return shared_observer && shared_observer == observer;
                });
            if (iter == m_spobservers->end())
                return;
            const auto observer_index = iter - m_spobservers->begin();

            auto new_spobservers = std::make_shared<Observers>();
            *new_spobservers = *m_spobservers;
            new_spobservers->erase(new_spobservers->begin() + observer_index);

            m_spobservers = new_spobservers;
        }

        void notify(const EventT& event)
        {
            const auto spobservers = [this] {
                std::lock_guard lock{m_mutex};
                return m_spobservers;
            }();

            for (const auto& weak_observer : *spobservers)
            {
                const auto shared_observer = weak_observer.lock();
                if (shared_observer)
                    shared_observer->update(event);
            }
        }

    private:
        std::mutex m_mutex{};
        std::shared_ptr<Observers> m_spobservers{std::make_shared<Observers>()};
    };

} // namespace musify::observer
