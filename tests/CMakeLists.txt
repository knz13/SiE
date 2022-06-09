

#this CMakeLists was created with EasyCmake - V2 
#the repository can be found at https://github.com/knz13/EasyCmake_Cpp


cmake_minimum_required(VERSION 3.20)



#adding useful functions

function(DIR_EXISTS variable dir_path)

file(GLOB ${variable}_check ${dir_path}/*)

list(LENGTH ${variable}_check ${variable}_len)

if(${${variable}_len} EQUAL 0)

set(${variable} FALSE PARENT_SCOPE)

else()

set(${variable} TRUE PARENT_SCOPE)

endif()

endfunction()

#adding extra cmake libs
include(GNUInstallDirs)
include(ExternalProject)
include(FetchContent)


#project name
project("SiE_tests_project")

#creating executable
add_executable(SiE_MainTests


	${PROJECT_SOURCE_DIR}/tests.cpp

)

set_property(TARGET SiE_MainTests PROPERTY CXX_STANDARD 17)


#adding libraries...

target_link_libraries(SiE_MainTests PUBLIC SiE)

#------------------- Extra Lines ----------------

	# Extra Lines for alias copying_shader ...
add_custom_command(TARGET SiE_MainTests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/tests/base_shader $<TARGET_FILE_DIR:SiE_MainTests>/base_shader)
