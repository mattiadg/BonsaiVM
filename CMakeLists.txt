cmake_minimum_required(VERSION 3.15) 
project(bonsaiVM)


set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  vm_test
  tests/vm_test.cpp
  src/vm.cpp
  src/code.cpp
)

set_property(TARGET vm_test PROPERTY CXX_STANDARD 20)
target_compile_options(vm_test PRIVATE -fmodules-ts -Wall)
# Build std_modules before target which use std libraries.
set_directory_properties(PROPERTIES ADDITIONAL_CLEAN_FILES "gcm.cache")

target_link_libraries(
  vm_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(vm_test)
