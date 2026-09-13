add_catch(test_stack test.cpp)
target_compile_options(test_stack PRIVATE -Wno-self-assign-overloaded)