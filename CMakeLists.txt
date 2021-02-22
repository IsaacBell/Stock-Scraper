cmake_minimum_required(VERSION 3.0.0)
project(project VERSION 0.1.0)

include(CTest)
enable_testing()

# file(GLOB TARGET_SRC "./*.cpp" )

SET(SRCS
  ${PROJECT_SOURCE_DIR}/email.h
  ${PROJECT_SOURCE_DIR}/email.cpp
  ${PROJECT_SOURCE_DIR}/Date.h
  ${PROJECT_SOURCE_DIR}/File.h
  ${PROJECT_SOURCE_DIR}/ConnectHttp.h
  ${PROJECT_SOURCE_DIR}/Macros.h
  ${PROJECT_SOURCE_DIR}/HistoricalTradingData.h
  ${PROJECT_SOURCE_DIR}/TradingData.h
  ${PROJECT_SOURCE_DIR}/quant/random.h
  ${PROJECT_SOURCE_DIR}/quant/random.cpp
  ${PROJECT_SOURCE_DIR}/main.cpp
  ${PROJECT_SOURCE_DIR}/quant/maineq1.cpp
)

add_definitions( -DCURL_STATICLIB )

set( CURL_LIBRARY ${CMAKE_SOURCE_DIR}/lib )
set( CURL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include )
find_package( CURL )

include_directories( ${CURL_INCLUDE_DIRS} )
link_directories( ${CURL_LIBRARIES} )

add_executable(${PROJECT_NAME} ${TARGET_SRC})
target_link_libraries(${PROJECT_NAME} curl)

find_package(CURL CONFIG REQUIRED)
target_link_libraries(project PRIVATE CURL::libcurl)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# ADD_EXECUTABLE(Email ${SRCS})

# INSTALL(TARGETS Email DESTINATION bin)
