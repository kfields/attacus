#include <functional>

namespace attacus {

class Event {
public:
};

template<typename T = void>
class Subscription {
public:
    typedef std::function<void(T)> TCallback;

    Subscription(const TCallback& callback) : callback_(callback) {

    }
    TCallback callback_;
};

template<typename T = void>
class EventSource {
public:
    typedef std::function<void(T)> TCallback;
    typedef Subscription<T> TSubscription;
    std::list<TSubscription*> subscriptions_;
    // Accessors
    void Publish(T value) {
        for (auto it = subscriptions_.begin(); it != subscriptions_.end(); ++it){
            TSubscription* subscription = *it;
            subscription->callback_(value);
        }
    }
    TSubscription* Subscribe(const TCallback& callback) {
        TSubscription* subscription = new Subscription<T>(callback);
        subscriptions_.push_back(subscription);
        return subscription;
    }
};

}// namespace attacus