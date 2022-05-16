cmake_minimum_required(VERSION 3.4)

project(AttTests LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/Test.cmake)

include(${ATT_ROOT}/cmake/Core.cmake)
include(${ATT_ROOT}/cmake/RxCpp.cmake)

enable_testing()

add_executable(${THIS}
    test_component.cpp
    test_event.cpp
)

USES_TEST(${THIS})

USES_CORE(${THIS})
USES_RXCPP(${THIS})

include(GoogleTest)
gtest_discover_tests(${THIS})