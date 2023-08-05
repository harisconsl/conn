project(test VERSION 1.0)

file(GLOB SRC
    "*.cpp"
)

add_executable(test ${SRC})
target_include_directories(test PUBLIC ${Boost_INCLUDE_DIRS} ${MY_SUPPORT}/googletest/include)
target_link_libraries(test PUBLIC ${Boost_LIBRARIES} pthread common /home/pgupta/workspace/test/apache-log4cxx-1.1.0/install/lib64/liblog4cxx.so ${MY_SUPPORT}/googletest/lib/libgtest.a ${MY_SUPPORT}/googletest/lib/libgmock.a ${MY_SUPPORT}/googletest/lib/libgtest_main.a ${MY_SUPPORT}/googletest/lib/libgmock_main.a)





