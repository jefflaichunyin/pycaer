cmake_minimum_required(VERSION 3.15...3.25)

project(${SKBUILD_PROJECT_NAME} VERSION ${SKBUILD_PROJECT_VERSION})

FIND_PACKAGE(pybind11 CONFIG REQUIRED)
FIND_PACKAGE(libcaer CONFIG REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED COMPONENTS core imgproc highgui)
find_library(LZ4_LIBRARY NAMES lz4 REQUIRED)

include_directories(include)


message(STATUS "CAER found: ${libcaer_FOUND}")
pybind11_add_module(davis MODULE src/davis.cpp)
target_compile_features(davis PRIVATE cxx_std_17)
target_compile_definitions(davis PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_link_libraries(davis PRIVATE caer ${OpenCV_LIBS})

install(TARGETS davis DESTINATION pycaer)

pybind11_add_module(aedat MODULE src/aedat.cpp)
target_compile_features(aedat PRIVATE cxx_std_17)
target_compile_definitions(aedat PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_link_libraries(aedat PRIVATE ${LZ4_LIBRARY} ${OpenCV_LIBS})
install(TARGETS aedat DESTINATION pycaer)
