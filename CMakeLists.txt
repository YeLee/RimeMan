CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(RimeMan)
ADD_EXECUTABLE(${PROJECT_NAME} "main.cpp" "rimedata.cpp")
SET(CMAKE_CXX_FLAGS "-std=c++11")

FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CURL_LIBRARY})
ELSE(CURL_FOUND)
  MESSAGE(FATAL_ERROR "curl not found!")
ENDIF(CURL_FOUND)

FIND_PACKAGE(Boost COMPONENTS filesystem system regex)
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Boost::filesystem
        Boost::system Boost::regex)
ELSE(Boost_FOUND)
    MESSAGE(FATAL_ERROR "Boost not found!")
ENDIF(Boost_FOUND)

FIND_PACKAGE(PkgConfig)

PKG_CHECK_MODULES(jsoncpp REQUIRED jsoncpp)
IF(jsoncpp_FOUND)
  INCLUDE_DIRECTORIES(${jsoncpp_INCLUDE_DIRS})
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${jsoncpp_LIBRARIES})
ELSE(jsoncpp_FOUND)
  MESSAGE(FATAL_ERROR "jsoncpp not found!")
ENDIF(jsoncpp_FOUND)

PKG_CHECK_MODULES(GIT REQUIRED libgit2)
IF(GIT_FOUND)
  INCLUDE_DIRECTORIES(${GIT_INCLUDE_DIRS})
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${GIT_LIBRARIES})
ELSE(GIT_FOUND)
  MESSAGE(FATAL_ERROR "git2 not found!")
ENDIF(GIT_FOUND)

ADD_DEFINITIONS(-DPROJECT_NAME=\"${PROJECT_NAME}\")
