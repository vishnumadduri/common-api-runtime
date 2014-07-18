cmake_minimum_required (VERSION 2.8.1)
file(GLOB CAPI_TEST_SRCS "*.cpp")
add_executable(CommonAPI_Tests ${CAPI_TEST_SRCS})
include_directories(
      src,
      ${gtest_SOURCE_DIR}/include
)
target_link_libraries(CommonAPI_Tests CommonAPI ${DL_LIBRARY} gtest ${PTHREAD_LIBRARY})
