#cmake_minimum_required(VERSION 3.12)

# Add source files
set(SOURCES
    test_tcp.cpp
)

add_executable(testtcp ${SOURCES} ${HEADERS})

#target_include_directories(test_tcp PRIVATE ${Boost_INCLUDE_DIRS})

# Add additional compile options if needed
# target_compile_options(MyExecutable PRIVATE -Wall -Wextra)

# Add additional libraries if needed
# target_link_libraries(MyExecutable PUBLIC my_library)

# Specify any required packages (e.g., find and link to Boost)
# find_package(Boost REQUIRED)
# target_link_libraries(MyExecutable PUBLIC Boost::boost)

# Specify any required external dependencies (e.g., include and link directories)
# target_include_directories(MyExecutable PRIVATE /path/to/external/include)
# target_link_directories(MyExecutable PRIVATE /path/to/external/lib)
# target_link_libraries(test_tcp PRIVATE ${Boost_LIBRARIES})


