CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(message CXX)

#source
FILE(GLOB_RECURSE SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

#include file
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES}
                    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/AppControl/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/ContactManager/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/Controller/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/MsgEngine/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/Utils/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/View/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Conversation/Controller/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Conversation/View/inc/
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/Conversation/Utils/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/MsgThread/Controller/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/MsgThread/View/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Settings/Controller/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Settings/View/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Viewer/Controller/inc/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Viewer/View/inc/
)

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED
            bundle
            capi-appfw-application
            capi-appfw-app-manager
            capi-appfw-preference
            capi-base-utils-i18n
            capi-content-media-content
            capi-system-device
            capi-system-info
            capi-media-image-util
            capi-media-metadata-extractor
            capi-media-player
            capi-system-system-settings
            capi-telephony
            capi-system-runtime-info
            contacts-service2
            db-util
            dlog
            elementary
            ecore
            ecore-input
            ecore-imf
            ecore-file
            eina
            glib-2.0
            libexif
            msg-service
            capi-messaging-messages
            notification
            tapi
            libxml-2.0
            libpng
            efl-extension
            storage
)

ADD_DEFINITIONS("-DAPP_DIR=\"${PKGDIR}\"")

FOREACH(flag ${pkgs_CFLAGS})
    SET(EXTRA_CXXFLAGS "${EXTRA_CXXFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXXFLAGS} -fpie -Wall -std=c++11 -fpermissive")

#Set executable
ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS})

ADD_SUBDIRECTORY(res)

CONFIGURE_FILE(message.manifest.in ${PKGNAME}.manifest) # TODO: check

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})
INSTALL(FILES tizen-manifest-private.xml DESTINATION ${MANIFESTDIR} RENAME ${PKGNAME}.xml)
INSTALL(FILES shared/res/message.png DESTINATION ${ICONDIR})

#test
if(DEFINED TEST)
    add_subdirectory(test)
endif(DEFINED TEST)