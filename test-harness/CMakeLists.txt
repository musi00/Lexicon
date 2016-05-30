add_executable(test-harness test-harness.cpp)
target_link_libraries(test-harness lexicon)
# enable C++11 option for this target
#set_property(TARGET test-harness PROPERTY CXX_STANDARD 11)
#set_property(TARGET test-harness PROPERTY CXX_STANDARD_REQUIRED ON)
