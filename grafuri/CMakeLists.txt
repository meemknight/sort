cmake_minimum_required(VERSION 3.13)
project(graf)

#add_subdirectory(thirdparty/...)


# Define MY_SOURCES to be a list of all the source files for my game 
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp")


add_executable("testGraf")
target_sources("testGraf" PRIVATE ${MY_SOURCES})

target_compile_features("testGraf" PUBLIC cxx_std_17)

target_compile_definitions("testGraf" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/") # This is useful to get an ASSETS_PATH in your IDE during development but you should comment this if you compile a release version and uncomment the next line
#target_compile_definitions("testGraf" PUBLIC RESOURCES_PATH="./resources/") # Uncomment this line to setup the ASSETS_PATH macro to the final assets directory when you share the game
#add_definitions(-DRESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")



if(MSVC) # If using the VS compiler...

	set_target_properties("testGraf" PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	set_target_properties("testGraf" PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

endif()

if(WIN32)
	target_compile_definitions("testGraf" PUBLIC PLATFORM_WIN32="1") 
  
endif()

target_include_directories("testGraf" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")


#target_link_libraries("testGraf" PRIVATE glad profilerLib)
