cmake_minimum_required(VERSION 3.8)

project( im_data_tests )

enable_testing()
include(CTest)

# create the testing file and list of tests
create_test_sourcelist (Tests
    ${PROJECT_NAME}.cpp
    # add tests here
    draft_01.cpp
)

# add the executable
add_executable (${PROJECT_NAME} ${Tests})
target_link_libraries( ${PROJECT_NAME} PRIVATE im_data )

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_14)

# remove the test driver source file
set (TestsToRun ${Tests})
remove (TestsToRun ${PROJECT_NAME}.cpp)

# Add all the ADD_TEST for each test
foreach (test ${TestsToRun})
    get_filename_component (TName ${test} NAME_WE)
    add_test (NAME ${TName} COMMAND ${PROJECT_NAME} ${TName})
endforeach ()


