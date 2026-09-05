add_catch(test_any test.cpp)
target_compile_options(test_any PRIVATE -Wno-self-assign-overloaded)
