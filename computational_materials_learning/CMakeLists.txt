	cmake_minimum_required(VERSION 3.16)

project(computational_materials_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
