project(example VERSION 1.0)

add_executable(testtcp test_tcp.cpp)
target_include_directories(testtcp PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(testtcp PUBLIC ${Boost_LIBRARIES} pthread common /home/pgupta/workspace/test/apache-log4cxx-1.1.0/install/lib64/liblog4cxx.so)

add_executable(testunix test_unix.cpp)
target_include_directories(testunix PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(testunix PUBLIC ${Boost_LIBRARIES} pthread common /home/pgupta/workspace/test/apache-log4cxx-1.1.0/install/lib64/liblog4cxx.so)


add_executable(testsm test_sm.cpp)
target_include_directories(testsm PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(testsm PUBLIC ${Boost_LIBRARIES} pthread common /home/pgupta/workspace/test/apache-log4cxx-1.1.0/install/lib64/liblog4cxx.so)

add_executable(testbook test_book.cpp)
target_include_directories(testbook PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(testbook PUBLIC ${Boost_LIBRARIES} pthread common /home/pgupta/workspace/test/apache-log4cxx-1.1.0/install/lib64/liblog4cxx.so)





