# main executable
add_executable(grid
    main.cpp
    grid.hpp
)

set_target_properties(grid PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(grid PUBLIC cxx_std_20)
target_compile_options(grid PRIVATE ${SANITIZER_COMPILE_OPTIONS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
target_link_options(grid PRIVATE ${SANITIZER_LINK_OPTIONS})
target_link_libraries(grid PRIVATE ${SANITIZER_LINK_LIBRARIES} fmt::fmt)

# tests
if(BUILD_TESTING)
    add_executable(grid_tests
        tests/grid.cpp
        grid.hpp
    )

    set_target_properties(grid_tests PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_features(grid_tests PUBLIC cxx_std_20)
    target_compile_options(grid_tests PRIVATE ${SANITIZER_COMPILE_OPTIONS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
    target_link_options(grid_tests PRIVATE ${SANITIZER_LINK_OPTIONS})
    target_link_libraries(grid_tests PRIVATE ${SANITIZER_LINK_LIBRARIES} fmt::fmt Catch2::Catch2WithMain)

    add_test(NAME grid_tests COMMAND grid_tests)
endif()

# benchmark
add_executable(grid_benchmark
    benchmark.cpp
    grid.hpp
)

set_target_properties(grid_benchmark PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(grid_benchmark PUBLIC cxx_std_20)
target_compile_options(grid_benchmark PRIVATE ${SANITIZER_COMPILE_OPTIONS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
target_link_options(grid_benchmark PRIVATE ${SANITIZER_LINK_OPTIONS})
target_link_libraries(grid_benchmark PRIVATE ${SANITIZER_LINK_LIBRARIES} fmt::fmt)
target_include_directories(grid_benchmark PRIVATE ${NANOBENCH_INCLUDE_DIRS})
