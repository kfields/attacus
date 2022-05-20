#include <gtest/gtest.h>

#include <rxcpp/rx.hpp>

#include <attacus/core/component.h>

using namespace attacus;

namespace test_event {

class TestPublisher : public Component
{
public:
    TestPublisher() {
        test_event_ = rxcpp::observable<>::create<bool>([=](rxcpp::subscriber<bool> out){
            auto it = observers_.insert(observers_.end(), out);
        });
    }
    void Publish(bool e) {
        test_event_.publish(e);
    }
    // Accessors
    // Data members
    rxcpp::observable<bool> test_event_;

    std::list<rxcpp::subscriber<bool>> observers_;

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