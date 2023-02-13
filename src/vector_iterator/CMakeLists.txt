# main executable
add_executable(vector_iterator
    main.cpp
    custom_vector.hpp
)

set_target_properties(vector_iterator PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(vector_iterator PUBLIC cxx_std_20)
target_compile_options(vector_iterator PRIVATE ${SANITIZER_COMPILE_OPTIONS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
target_link_options(vector_iterator PRIVATE ${SANITIZER_LINK_OPTIONS})
target_link_libraries(vector_iterator PRIVATE ${SANITIZER_LINK_LIBRARIES} fmt::fmt)

# tests
if(BUILD_TESTING)
    add_executable(vector_iterator_tests
        tests.cpp
        custom_vector.hpp
    )

    set_target_properties(vector_iterator_tests PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_features(vector_iterator_tests PUBLIC cxx_std_20)
    target_compile_options(vector_iterator_tests PRIVATE ${SANITIZER_COMPILE_OPTIONS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
    target_link_options(vector_iterator_tests PRIVATE ${SANITIZER_LINK_OPTIONS})
    target_link_libraries(vector_iterator_tests PRIVATE ${SANITIZER_LINK_LIBRARIES} fmt::fmt Catch2::Catch2WithMain)

    add_test(NAME vector_iterator_tests COMMAND vector_iterator_tests)
endif()
