cmake_minimum_required(VERSION 3.15...3.25)

project(${SKBUILD_PROJECT_NAME} VERSION ${SKBUILD_PROJECT_VERSION})

find_package(pybind11 CONFIG REQUIRED)
find_package(libcaer CONFIG REQUIRED)
message(STATUS "CAER found: ${libcaer_FOUND}")
pybind11_add_module(davis MODULE src/main.cpp)
target_compile_definitions(davis PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_link_libraries(davis PRIVATE caer)
install(TARGETS davis DESTINATION pycaer)
