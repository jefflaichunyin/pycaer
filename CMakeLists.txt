cmake_minimum_required(VERSION 3.15...3.25)

project(${SKBUILD_PROJECT_NAME} VERSION ${SKBUILD_PROJECT_VERSION})

FIND_PACKAGE(pybind11 CONFIG REQUIRED)
FIND_PACKAGE(libcaer CONFIG REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED COMPONENTS core imgproc highgui)

message(STATUS "CAER found: ${libcaer_FOUND}")
pybind11_add_module(davis MODULE src/main.cpp)
target_compile_definitions(davis PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_link_libraries(davis PRIVATE caer ${OpenCV_LIBS})
install(TARGETS davis DESTINATION pycaer)
