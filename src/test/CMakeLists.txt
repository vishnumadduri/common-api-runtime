cmake_minimum_required (VERSION 2.8.1)
file(GLOB CAPI_TEST_SRCS "*.cpp")
add_executable(CommonAPI_Tests ${CAPI_TEST_SRCS})
target_link_libraries(CommonAPI_Tests CommonAPI ${DL_LIBRARY} ${GTEST_LIB_PATH}/libgtest.a ${PTHREAD_LIBRARY})
