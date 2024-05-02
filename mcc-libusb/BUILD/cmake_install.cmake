# Install script for directory: /home/harps3/MICADO/micado-bearingtest/mcc-libusb

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/opt/micado")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so.0.0.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so.0.0.1")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so.0.0.1"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/micado/lib/libmccusb.so.0.0.1")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/opt/micado/lib" TYPE SHARED_LIBRARY FILES "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/BUILD/libmccusb.so.0.0.1")
  if(EXISTS "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so.0.0.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so.0.0.1")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so.0.0.1")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/micado/lib/libmccusb.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/opt/micado/lib" TYPE SHARED_LIBRARY FILES "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/BUILD/libmccusb.so")
  if(EXISTS "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/opt/micado/lib/libmccusb.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/micado/include/mccusb.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/opt/micado/include" TYPE FILE FILES "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/mccusb.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/micado/include/pmd.h;/opt/micado/include/usb-500.h;/opt/micado/include/usb-1608G.h;/opt/micado/include/usb-20X.h;/opt/micado/include/usb-1208FS-Plus.h;/opt/micado/include/usb-1608FS-Plus.h;/opt/micado/include/usb-2020.h;/opt/micado/include/usb-ctr.h;/opt/micado/include/usb-2600.h;/opt/micado/include/usb-2408.h;/opt/micado/include/usb-2416.h;/opt/micado/include/usb-1608HS.h;/opt/micado/include/usb-1208HS.h;/opt/micado/include/usb-2001-tc.h;/opt/micado/include/usb-1024LS.h;/opt/micado/include/usb-1208LS.h;/opt/micado/include/usb-1608FS.h;/opt/micado/include/usb-7202.h;/opt/micado/include/usb-tc.h;/opt/micado/include/usb-dio24.h;/opt/micado/include/usb-dio96H.h;/opt/micado/include/usb-5200.h;/opt/micado/include/usb-temp.h;/opt/micado/include/usb-7204.h;/opt/micado/include/usb-1208FS.h;/opt/micado/include/usb-ssr.h;/opt/micado/include/usb-erb.h;/opt/micado/include/usb-pdiso8.c;/opt/micado/include/usb-1408FS.h;/opt/micado/include/usb-1616FS.h;/opt/micado/include/usb-3100.h;/opt/micado/include/usb-4303.h;/opt/micado/include/usb-tc-ai.h;/opt/micado/include/usb-dio32HS.h;/opt/micado/include/usb-tc-32.h;/opt/micado/include/bth-1208LS.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/opt/micado/include" TYPE FILE FILES
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/pmd.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-500.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-1608G.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-20X.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-1208FS-Plus.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-1608FS-Plus.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-2020.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-ctr.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-2600.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-2408.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-2416.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-1608HS.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-1208HS.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-2001-tc.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-1024LS.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-1208LS.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-1608FS.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-7202.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-tc.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-dio24.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-dio96H.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-5200.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-temp.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-7204.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-1208FS.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-ssr.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-erb.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-pdiso8.c"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-1408FS.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-1616FS.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-3100.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-4303.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-tc-ai.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-dio32HS.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/usb-tc-32.h"
    "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/bth-1208LS.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/harps3/MICADO/micado-bearingtest/mcc-libusb/BUILD/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
