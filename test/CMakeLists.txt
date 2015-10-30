cmake_minimum_required(VERSION 2.6)

file(GLOB TEST_SRC *.cpp)

FILE(GLOB_RECURSE APP_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src/Common/*.cpp
	${CMAKE_SOURCE_DIR}/src/Conversation/*.cpp
	${CMAKE_SOURCE_DIR}/src/MsgThread/*.cpp
	${CMAKE_SOURCE_DIR}/src/Settings/*.cpp
	${CMAKE_SOURCE_DIR}/src/Viewer/*.cpp
)

FIND_PACKAGE(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/src/Common/AppControl/inc
	${CMAKE_SOURCE_DIR}/src/Common/ContactManager/inc
	${CMAKE_SOURCE_DIR}/src/Common/Controller/inc
	${CMAKE_SOURCE_DIR}/src/Common/MsgEngine/inc
	${CMAKE_SOURCE_DIR}/src/Common/Utils/inc
	${CMAKE_SOURCE_DIR}/src/Common/View/inc
	${CMAKE_CURRENT_SOURCE_DIR}/src/Conversation/Controller/inc/
	${CMAKE_CURRENT_SOURCE_DIR}/src/Conversation/View/inc/
	${CMAKE_CURRENT_SOURCE_DIR}/src/MsgThread/Controller/inc/
	${CMAKE_CURRENT_SOURCE_DIR}/src/MsgThread/View/inc/
	${CMAKE_CURRENT_SOURCE_DIR}/src/Settings/Controller/inc/
	${CMAKE_CURRENT_SOURCE_DIR}/src/Settings/View/inc/
	${CMAKE_CURRENT_SOURCE_DIR}/src/Viewer/Controller/inc/
	${CMAKE_CURRENT_SOURCE_DIR}/src/Viewer/View/inc/
)

ADD_DEFINITIONS(${pkgs_CFLAGS})
ADD_EXECUTABLE(messages-test
	${APP_SRCS}
	${TEST_SRC}
)

TARGET_LINK_LIBRARIES(messages-test
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	${pkgs_LDFLAGS}
)

install(TARGETS messages-test DESTINATION ${BINDIR})
