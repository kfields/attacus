#include <gtest/gtest.h>

#include <attacus/core/component.h>

using namespace attacus;

namespace test_event {

template<typename T>
class EventDelegate {
public:
    EventDelegate() {}
    EventDelegate(std::function<void(void)> callback) : callback_(callback) {}
    void operator()() {
        callback_();
    }
    // Data members
    std::function<void(void)> callback_;
};

template<typename T>
class Subscription {
public:
    typedef std::function<void(T)> TCallback;

    Subscription(const TCallback& callback) : callback_(callback) {

    }
    TCallback callback_;
};

template<typename T>
class Event {
public:
    typedef std::function<void(T)> TCallback;
    typedef Subscription<T> TSubscription;
    std::list<TSubscription*> subscriptions_;
    // Accessors
    void publish(T& value) {
        for (auto it = subscriptions_.begin(); it != subscriptions_.end(); ++it){
            TSubscription* subscription = *it;
            subscription->callback_(value);
        }
    }
    void subscribe(const TCallback& callback) {
        auto subscription = new Subscription<T>(callback);
        subscriptions_.push_back(subscription);
    }
};

class TestPublisher : public Component
{
public:
    void Publish(bool e) {
        test_event_.publish(e);
    }
    // Accessors
    // Data members
    Event<bool> test_event_;
};

class TestSubscriber : public Component
{
public:
    void Subscribe(TestPublisher& publisher) {
        publisher.test_event_.subscribe([](bool e){
            std::cout << "Received event:  "  << e << std::endl;
        });
    }
};

// Demonstrate some basic assertions.
TEST(TestEvent, BasicAssertions)
{
    TestPublisher publisher;
    TestSubscriber subscriber;

    subscriber.Subscribe(publisher);
    publisher.Publish(true);
    publisher.Publish(false);

    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

} //namespace test_event