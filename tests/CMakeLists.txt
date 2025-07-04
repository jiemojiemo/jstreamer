add_subdirectory(googletest)

add_executable(jstreamer_unit_test
        test_base_element.cpp
        test_property_spec_manager.cpp
        test_property_value_manager.cpp
        test_signal_manager.cpp
)
target_link_libraries(jstreamer_unit_test PRIVATE
        gmock_main
        jstreamer
)

add_test(
        NAME jstreamer_unit_test
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND jstreamer_unit_test
)