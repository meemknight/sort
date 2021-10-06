cmake_minimum_required(VERSION 3.13)
project(sort)

add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/profilerLib)


# Define MY_SOURCES to be a list of all the source files for my game 
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp")


add_executable("sort")
target_sources("sort" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp")

add_executable("pairingHeap")
target_sources("pairingHeap" PRIVATE "sources/pairingHeap.h" "sources/pairingHeapMain.cpp")


set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/") # This is useful to get an ASSETS_PATH in your IDE during development but you should comment this if you compile a release version and uncomment the next line
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") # Uncomment this line to setup the ASSETS_PATH macro to the final assets directory when you share the game
#add_definitions(-DRESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")



if(MSVC) # If using the VS compiler...

	set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

endif()

if(WIN32)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PLATFORM_WIN32="1") 
  
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/sort")



target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glad profilerLib)
