
add_executable(add_example add.cpp)
target_compile_features(add_example PRIVATE cxx_std_23)
set_target_properties(add_example PROPERTIES CXX_EXTENSIONS OFF)

add_executable(mul_example mul.cpp)
target_compile_features(mul_example PRIVATE cxx_std_23)
set_target_properties(mul_example PROPERTIES CXX_EXTENSIONS OFF)

