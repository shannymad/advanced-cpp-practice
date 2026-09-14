add_catch(test_queue test.cpp)
target_compile_options(test_queue PRIVATE -Wno-self-assign-overloaded)