project(common VERSION 1.0)

file(GLOB SRC
     "*.cpp"
)

file(GLOB HDR
     "*.h"
)

add_library(common STATIC ${SRC} ${HDR})
#target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(common PUBLIC /home/pgupta/workspace/test/apache-log4cxx-1.1.0/install/include)



