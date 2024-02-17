project(aggregator VERSION 1.0)

add_executable(aggregator main.cpp)
target_include_directories(aggregator PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(aggregator PUBLIC ${Boost_LIBRARIES} pthread common /home/pgupta/workspace/test/apache-log4cxx-1.1.0/install/lib64/liblog4cxx.so)





