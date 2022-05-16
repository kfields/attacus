#include <gtest/gtest.h>

#include <attacus/core/component.h>

using namespace attacus;

static const char *s_dummy_events = ComponentKit::Intern("dummy_events");

class DummyEvents : public Component
{
};

void set_k_dummy_events(Component &self, DummyEvents *that)
{
    self.components_[s_dummy_events] = that;
}

DummyEvents *k_dummy_events(Component &self)
{
    return static_cast<DummyEvents *>(self.components_[s_dummy_events]);
}

class DummyConsumer : public Component
{
};

// Demonstrate some basic assertions.
TEST(TestComponent, BasicAssertions)
{
    DummyEvents events;
    DummyConsumer consumer;

    set_k_dummy_events(consumer, &events);
    DummyEvents* rEvents = k_dummy_events(events);
    
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}