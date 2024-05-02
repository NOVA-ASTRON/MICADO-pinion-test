cmake_minimum_required(VERSION 2.8)

project(movieoutput)

add_library(${PROJECT_NAME} SHARED "src/movie_output.cpp")

# For CMake < 3.9, we need to make the target ourselves

#if(NOT TARGET OpenMP::OpenMP_CXX)
#    find_package(Threads REQUIRED)
#    add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
#    set_property(TARGET OpenMP::OpenMP_CXX
#                 PROPERTY INTERFACE_COMPILE_OPTIONS ${OpenMP_CXX_FLAGS})
#    # Only works if the same flag is passed to the linker; use CMake 3.9+ otherwise (Intel, AppleClang)
#    set_property(TARGET OpenMP::OpenMP_CXX
#                 PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)
#
#endif()

target_link_libraries(${PROJECT_NAME} PUBLIC avcodec avutil swscale)

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 0.0.1)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/lib${PROJECT_NAME}.h)

target_include_directories(${PROJECT_NAME} PRIVATE .)

target_include_directories(${PROJECT_NAME} PRIVATE "include/")
target_include_directories(${PROJECT_NAME} PRIVATE "src/")
target_include_directories(${PROJECT_NAME} PRIVATE "/opt/harps3/include/")
target_include_directories(${PROJECT_NAME} PRIVATE "/opt/harps3/local/VimbaSDK")

#target_link_libraries(${PROJECT_NAME} improc VimbaCPP VimbaImageTransform zmq gomp pthread)

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-L/opt/harps3/lib/  -Wl,-rpath=/opt/harps3/lib/")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION "/opt/harps3/lib/"
    PUBLIC_HEADER DESTINATION "/opt/harps3/include/"
    )
#install(FILES "VimbaTransportLayer/VimbaGigETL.cti"  "VimbaTransportLayer/VimbaGigETL.xml" DESTINATION "/opt/harps3/share/"  )
