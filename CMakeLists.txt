cmake_minimum_required(VERSION 3.16)

project(computational_materials_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(hello cpp_foundations/hello.cpp)
add_executable(diffusivity cpp_foundations/diffusivity.cpp)
add_executable(diagnostic_exercise cpp_foundations/diagnostic_exercise.cpp)
target_compile_options(diagnostic_exercise PRIVATE
	-Wall
	-Wextra
	-Wpedantic
	-Wconversion
	-fsanitize=address,undefined
	-fno-omit-frame-pointer
	)
target_link_options(diagnostic_exercise PRIVATE
	-fsanitize=address,undefined
	)
add_library(arrhenius STATIC
    include/arrhenius.hpp
    src/arrhenius.cpp
)
########################################Diagnostic Exercise 2#################################
add_executable(diagnostic_exercise_new src/diagnostic_exercise.cpp)
target_compile_options(diagnostic_exercise_new PRIVATE
	-fsanitize=address,undefined
	-fno-omit-frame-pointer
	)
target_link_options(diagnostic_exercise_new PRIVATE
	-fsanitize=address,undefined
	)
target_include_directories(arrhenius PUBLIC /home/pushkar/computational_materials_learning/include)
target_link_libraries(diagnostic_exercise_new PRIVATE arrhenius)
############################Monotonicity Test########################################
add_executable(monotonicity_test src/monotonicity_test.cpp)
target_compile_options(monotonicity_test PRIVATE
	-fsanitize=address,undefined
	-fno-omit-frame-pointer
	)
target_link_options(monotonicity_test PRIVATE
	-fsanitize=address,undefined
	)
target_include_directories(arrhenius PUBLIC /home/pushkar/computational_materials_learning/include)
target_link_libraries(monotonicity_test PRIVATE arrhenius)

############################Arrhenius Test########################################
add_executable(arrhenius_test src/arrhenius_test.cpp)
target_compile_options(monotonicity_test PRIVATE
	-fsanitize=address,undefined
	-fno-omit-frame-pointer
	)
target_link_options(arrhenius_test PRIVATE
	-fsanitize=address,undefined
	)
target_include_directories(arrhenius PUBLIC /home/pushkar/computational_materials_learning/include)
target_link_libraries(arrhenius_test PRIVATE arrhenius)
###############################Adding Tests########################################
add_test(
    NAME unit_test_1
    COMMAND ./diagnostic_exercise_new
)
add_test(
    NAME unit_test_2
    COMMAND ./arrhenius_test
)
##################################Adding Doxygen for Docs######################################
find_package(Doxygen REQUIRED)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()
##################################Arrhenius Model Class Library and its tests######################################
add_library(arrhenius_model STATIC
    include/arrhenius_model.hpp
    src/arrhenius_model.cpp
)
target_link_libraries(arrhenius_model
    PRIVATE arrhenius
)
add_executable(arrhenius_model_test src/arrhenius_model_test.cpp)
target_compile_options(arrhenius_model_test PRIVATE
	-fsanitize=address,undefined
	-fno-omit-frame-pointer
	)
target_link_options(arrhenius_model_test PRIVATE
	-fsanitize=address,undefined
	)
target_include_directories(arrhenius_model PUBLIC /home/pushkar/computational_materials_learning/include)
target_link_libraries(arrhenius_model_test PRIVATE arrhenius_model)
add_test(
    NAME unit_test_3
    COMMAND ./arrhenius_model_test
)
#############Temperature Grid Library and its test######################################
add_library(temperature_grid STATIC
    src/temperature_grid.cpp
    include/temperature_grid.hpp
)
target_link_libraries(temperature_grid
    PRIVATE arrhenius	
)
add_executable(temperature_grid_test src/temperature_grid_test.cpp)
target_compile_options(temperature_grid_test PRIVATE
	-fsanitize=address,undefined
	-fno-omit-frame-pointer
	)
target_link_options(temperature_grid_test PRIVATE
	-fsanitize=address,undefined
	)
target_include_directories(temperature_grid PUBLIC /home/pushkar/computational_materials_learning/include)
target_link_libraries(temperature_grid_test PRIVATE temperature_grid)
add_test(
    NAME unit_test_4
    COMMAND ./temperature_grid_test
)
#####################################Scientific Workspace and its tests##############################################
add_library(scientific_workspace STATIC
    src/scientific_workspace.cpp
    include/scientific_workspace.hpp
)

target_include_directories(
    scientific_workspace
    PUBLIC
    /home/pushkar/computational_materials_learning/include
)

add_executable(
    scientific_workspace_test
    src/scientific_workspace_test.cpp
)


target_link_libraries(
    scientific_workspace_test
    PRIVATE
    scientific_workspace arrhenius
)

add_test(
    NAME unit_test_5
    COMMAND scientific_workspace_test
)
#####################################Adding DocTest through FetchContent Mechanism################################################
include(FetchContent)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        v2.4.12
)

FetchContent_MakeAvailable(doctest)

add_executable(
    scientific_workspace_doctest
    tests/scientific_workspace_doctest.cpp
)
target_compile_options(scientific_workspace_doctest PRIVATE
	-fsanitize=address,undefined
	-fno-omit-frame-pointer
	)

target_link_libraries(
    scientific_workspace_doctest
    PRIVATE
        scientific_workspace
        doctest::doctest
)

target_link_options(scientific_workspace_doctest PRIVATE
	-fsanitize=address,undefined
	)
	
add_test(
    NAME scientific_workspace_doctest
    COMMAND scientific_workspace_doctest
)
