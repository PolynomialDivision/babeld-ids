cmake_minimum_required(VERSION 2.6)
PROJECT(babeld-ids)

ADD_DEFINITIONS(-Os -Wall -Werror --std=gnu99 -Wmissing-declarations -Wno-unknown-warning-option -Wno-format-truncation)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

SET(SOURCES main.c uci.c)

SET(LIBS
        ubox ubus json-c blobmsg_json uci owipcalc)

ADD_EXECUTABLE(babeld-ids ${SOURCES})

TARGET_LINK_LIBRARIES(babeld-ids ${LIBS})

INSTALL(TARGETS babeld-ids
        RUNTIME DESTINATION /usr/sbin/)
