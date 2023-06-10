cmake_minimum_required(VERSION 3.12)
project(MyProject)

# Set C++ standard version
set(CMAKE_CXX_STANDARD 17)

set(BOOST_ROOT $ENV{BOOST_HOME})

# Find the Boost libraries
find_package(Boost REQUIRED)

# Add source files
set(SOURCES
    client.cpp
)

# Add header files
set(HEADERS
    header.h
)

set(SOURCES1
    server.cpp
)

set(SOURCES2
    timer_loop.cpp
)


# Create an executable target
add_executable(client ${SOURCES} ${HEADERS})

add_executable(server ${SOURCES1} ${HEADERS})

add_executable(timer ${SOURCES2} ${HEADERS})

# Set include directories
target_include_directories(client PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(timer PRIVATE ${Boost_INCLUDE_DIRS})

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
 target_link_libraries(client PRIVATE ${Boost_LIBRARIES})
 target_link_libraries(server PRIVATE ${Boost_LIBRARIES})
 target_link_libraries(timer PRIVATE ${Boost_LIBRARIES})

