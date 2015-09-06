cmake_minimum_required(VERSION 3.2)
set(PROJNAME EGEFramework)
project( ${PROJNAME} )

set(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/opengl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/fbx/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glfw/include)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/fbx/lib/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glfw/lib/)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)


file(GLOB_RECURSE all_files 
	${DIRECTORY}/*.cpp
	${DIRECTORY}/*.hpp
	${DIRECTORY}/*.h
	${DIRECTORY}/*.c
)

add_executable( ${PROJNAME}
	${all_files}
)

foreach(f ${all_files})
    file(RELATIVE_PATH SRCGR ${DIRECTORY} ${f})
    set(SRCGR "src/${SRCGR}")
    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})
    string(REPLACE / \\ SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
endforeach()

set_target_properties(${PROJNAME} PROPERTIES OUTPUT_NAME_DEBUG ${PROJNAME}-Debug)
set_target_properties(${PROJNAME} PROPERTIES OUTPUT_NAME_RELEASE ${PROJNAME}-Release)

SET(FBXLIB optimized release/libfbxsdk-md debug debug/libfbxsdk-md)
target_link_libraries (${PROJNAME} ${FBXLIB})

SET(GLFWLIB optimized glfw3 debug glfw3_d)
target_link_libraries (${PROJNAME} ${GLFWLIB})
target_link_libraries (${PROJNAME} glu32)
target_link_libraries (${PROJNAME} opengl32)