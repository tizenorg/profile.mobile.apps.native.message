cmake_minimum_required(VERSION 2.6)

file(GLOB TEST_SRC TC/*.cpp)

FILE(GLOB_RECURSE APP_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src/Common/*.cpp
	${CMAKE_SOURCE_DIR}/src/Conversation/*.cpp
	${CMAKE_SOURCE_DIR}/src/MsgThread/*.cpp
	${CMAKE_SOURCE_DIR}/src/Settings/*.cpp
	${CMAKE_SOURCE_DIR}/src/Viewer/*.cpp
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/thirdparties/gmock/gtest/include
	${CMAKE_CURRENT_SOURCE_DIR}/thirdparties/gmock/include
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

ADD_SUBDIRECTORY(thirdparties/gmock)

ADD_DEFINITIONS(${pkgs_CFLAGS})
ADD_EXECUTABLE(messages-test
	${APP_SRCS}
	${TEST_SRC}
)

TARGET_LINK_LIBRARIES(messages-test
	gmock
	${pkgs_LDFLAGS}
)

install(TARGETS messages-test DESTINATION ${BINDIR})
