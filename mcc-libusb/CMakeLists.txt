cmake_minimum_required(VERSION 2.8)

project(mccusb)

set(SRCS pmd.c  nist.c   usb-1608G.c usb-20X.c usb-1208FS-Plus.c usb-1608FS-Plus.c usb-2020.c
    usb-ctr.c usb-2600.c usb-2408.c usb-2416.c usb-1608HS.c usb-1208HS.c usb-2001-tc.c
    usb-1024LS.c usb-1208LS.c usb-1608FS.c usb-7202.c usb-tc.c usb-dio24.c usb-dio96H.c
    usb-5200.c usb-temp.c usb-7204.c usb-1208FS.c usb-ssr.c usb-erb.c usb-pdiso8.c
    usb-1408FS.c usb-1616FS.c usb-3100.c usb-4303.c usb-tc-ai.c usb-dio32HS.c usb-tc-32.c
    bth-1208LS.c
    )

set(HEADERS pmd.h usb-500.h usb-1608G.h usb-20X.h usb-1208FS-Plus.h usb-1608FS-Plus.h usb-2020.h
    usb-ctr.h usb-2600.h usb-2408.h usb-2416.h usb-1608HS.h usb-1208HS.h usb-2001-tc.h
    usb-1024LS.h usb-1208LS.h usb-1608FS.h usb-7202.h usb-tc.h usb-dio24.h usb-dio96H.h
    usb-5200.h usb-temp.h usb-7204.h usb-1208FS.h usb-ssr.h usb-erb.h usb-pdiso8.c
    usb-1408FS.h usb-1616FS.h usb-3100.h usb-4303.h usb-tc-ai.h usb-dio32HS.h usb-tc-32.h
    bth-1208LS.h
    )

if (MSVC OR MSYS OR MINGW)
   add_library(${PROJECT_NAME} SHARED  ${SRCS} ${HEADERS})
else()
   add_library(${PROJECT_NAME} SHARED  ${SRCS} ${HEADERS})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 0.0.1)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${PROJECT_NAME}.h)

target_include_directories(${PROJECT_NAME} PRIVATE .)

target_include_directories(${PROJECT_NAME} PRIVATE "include/")
target_include_directories(${PROJECT_NAME} PRIVATE "src/")

# suppress multichar warnings.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wno-multichar")

if (MSVC OR MSYS OR MINGW)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-L${CMAKE_INSTALL_PREFIX}/bin/  -Wl,-rpath=${CMAKE_INSTALL_PREFIX}/bin/ ")
else()
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-L${CMAKE_INSTALL_PREFIX}/lib/  -Wl,-rpath=${CMAKE_INSTALL_PREFIX}/lib/ ")
endif()

target_link_libraries(${PROJECT_NAME} usb-1.0)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include/"
    )

install(FILES ${HEADERS}
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include" )
