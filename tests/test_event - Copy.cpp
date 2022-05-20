#include <gtest/gtest.h>

#include <rxcpp/rx.hpp>

#include <attacus/core/component.h>

using namespace attacus;

namespace test_event {

static const char *s_dummy_events = ComponentKit::Intern("dummy_events");

template<typename T>
class Event {
    rxcpp::observable<T> value_;
    // Accessors
    void publish(T& value) {
        value_.publish(value);
    }
    void subscribe() {
        value_.subscribe();
    }
};

class TestEvents : public Component
{
public:
    rxcpp::subjects::subject<int> OnSomething;
};

void set_k_dummy_events(Component &self, TestEvents *that)
{
    self.components_[s_dummy_events] = that;
}

TestEvents *k_dummy_events(Component &self)
{
    return static_cast<TestEvents *>(self.components_[s_dummy_events]);
}

class TestPublisher : public Component
{
public:
    // Accessors
    TestEvents& events() { return events_; }
    // Data members
    TestEvents events_;
};

class TestConsumer : public Component
{
    void Subscribe(TestPublisher& publisher) {
        publisher.events().OnSomething.get_observable().subscribe();
    }
};

// Demonstrate some basic assertions.
TEST(TestEvent, BasicAssertions)
{
    TestEvents events;
    TestConsumer consumer;

    set_k_dummy_events(consumer, &events);
    TestEvents* rEvents = k_dummy_events(events);
    
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

} //namespace test_event