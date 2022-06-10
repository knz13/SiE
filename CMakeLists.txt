

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
project("SiE_project")

#adding subdirectories...

add_subdirectory(tests)


# --------------------- Repository declarations ------------------------


#repository download and settings for alias sdl_repo...


	FetchContent_Declare(sdl_repo_project

		GIT_REPOSITORY https://github.com/libsdl-org/SDL

		GIT_TAG release-2.0.22

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/sdl_repo

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES sdl_repo_project)


#repository download and settings for alias glm...


	FetchContent_Declare(glm_project

		GIT_REPOSITORY https://github.com/g-truc/glm

		GIT_TAG 0.9.9.8

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/glm

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES glm_project)


#repository download and settings for alias glew...


	FetchContent_Declare(glew_project

		GIT_REPOSITORY https://github.com/Perlmint/glew-cmake

		GIT_TAG glew-cmake-2.2.0

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/glew

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES glew_project)


#repository download and settings for alias ecspp...


	FetchContent_Declare(ecspp_project

		GIT_REPOSITORY https://github.com/knz13/ecspp

		GIT_TAG origin/main

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/ecspp

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES ecspp_project)


#repository download and settings for alias yael...


	FetchContent_Declare(yael_project

		GIT_REPOSITORY https://github.com/knz13/YAEL

		GIT_TAG origin/main

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/yael

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES yael_project)


#repository download and settings for alias assimp...


	#adding options

	set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")

	set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

	set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE INTERNAL "")

	FetchContent_Declare(assimp_project

		GIT_REPOSITORY https://github.com/assimp/assimp

		GIT_TAG v5.2.4

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/assimp

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES assimp_project)


#repository download and settings for alias stb...


	FetchContent_Declare(stb_project

		GIT_REPOSITORY https://github.com/nothings/stb

		GIT_TAG origin/master

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/stb

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES stb_project)


#repository download and settings for alias async...


	#adding options

	set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

	FetchContent_Declare(async_project

		GIT_REPOSITORY https://github.com/Amanieu/asyncplusplus

		GIT_TAG v1.1

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/async

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES async_project)


	FetchContent_MakeAvailable(${${PROJECT_NAME}_DEPENDENCIES})

	foreach(X IN LISTS ${PROJECT_NAME}_DEPENDENCIES)

		FetchContent_GetProperties(${X}
			SOURCE_DIR ${X}_SOURCE_DIR
			BINARY_DIR ${X}_BINARY_DIR
		)

	endforeach() 

#creating library
add_library(SiE STATIC

	${PROJECT_SOURCE_DIR}/source/camera/camera.cpp

	${PROJECT_SOURCE_DIR}/source/general/color.cpp

	${PROJECT_SOURCE_DIR}/source/general/helpers.cpp

	${PROJECT_SOURCE_DIR}/source/general/math.cpp

	${PROJECT_SOURCE_DIR}/source/general/structures.cpp

	${PROJECT_SOURCE_DIR}/source/general/transform.cpp

	${PROJECT_SOURCE_DIR}/source/mesh/mesh.cpp

	${PROJECT_SOURCE_DIR}/source/object/game_object.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/framebuffer.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/index_buffer.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/shader.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/texture.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/vertex_array.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/vertex_buffer.cpp

	${PROJECT_SOURCE_DIR}/source/window/window.cpp

	${PROJECT_SOURCE_DIR}/source/model_loader/model_loader.cpp

	${PROJECT_SOURCE_DIR}/source/model_loader/texture_loader.cpp

)

set_property(TARGET SiE PROPERTY CXX_STANDARD 17)


#adding libraries...


target_link_libraries(SiE PUBLIC SDL2::SDL2-static)

target_link_libraries(SiE PUBLIC SDL2::SDL2main)

target_link_libraries(SiE PUBLIC libglew_static)

target_link_libraries(SiE PUBLIC assimp)

target_link_libraries(SiE PUBLIC Async++)


#adding includes...


target_include_directories(SiE PUBLIC ${PROJECT_SOURCE_DIR}/source)

target_include_directories(SiE PUBLIC ${PROJECT_SOURCE_DIR}/vendor)

target_include_directories(SiE PUBLIC ${PROJECT_SOURCE_DIR}/vendor/sdl_repo/include)

target_include_directories(SiE PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glm/)

target_include_directories(SiE PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glew/include)

target_include_directories(SiE PUBLIC ${PROJECT_SOURCE_DIR}/vendor/ecspp/include)

target_include_directories(SiE PUBLIC ${PROJECT_SOURCE_DIR}/vendor/yael/include)

target_include_directories(SiE PUBLIC ${PROJECT_SOURCE_DIR}/vendor/assimp/include)

target_include_directories(SiE PUBLIC ${PROJECT_SOURCE_DIR}/vendor/stb/)

target_include_directories(SiE PUBLIC ${PROJECT_SOURCE_DIR}/vendor/async/include)
