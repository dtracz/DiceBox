cmake_minimum_required(VERSION 3.12)


# project name
set(DICE_BOX "DiceBox")
# third parties
set(OOML "OOML")
set(cxxopts "cxxopts")

# paths
set(DICEBOX_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(DICEBOX_SRC  ${DICEBOX_ROOT}/src)
set(DICEBOX_BIN  ${DICEBOX_ROOT}/bin)
set(THIRD_PARTY  ${DICEBOX_ROOT}/third_party)


# set up project
project(
    ${DICE_BOX}
    LANGUAGES CXX
)

# compile everything into one executable
file(GLOB ALL_SRCS
    ${DICEBOX_SRC}/base/*.cpp
    ${DICEBOX_SRC}/utils/*.cpp
    ${DICEBOX_SRC}/components/*.cpp
    ${DICEBOX_SRC}/packer/*.cpp
    ${DICEBOX_SRC}/parser/*.cpp
    ${DICEBOX_SRC}/main.cpp
)
add_executable(${DICE_BOX} ${ALL_SRCS})

# set c++ standard to 20
set_property(TARGET ${DICE_BOX} PROPERTY CXX_STANDARD 20)


# set up third parties
add_subdirectory(${THIRD_PARTY}/OOML)
add_subdirectory(${THIRD_PARTY}/cxxopts)


target_include_directories(${DICE_BOX}
    PUBLIC ${DICEBOX_SRC} 
    PUBLIC ${THIRD_PARTY}/OOML/src
    PUBLIC ${THIRD_PARTY}/cxxopts/include
)

target_link_directories(${DICE_BOX}
    PUBLIC ${DICEBOX_SRC}
    PUBLIC ${THIRD_PARTY}/OOML/lib
    # no linking required -- header only library
    # PUBLIC ${THIRD_PARTY}/cxxopts/...
)

target_link_libraries(${DICE_BOX}
    OOMLCore
    OOMLComponents
    OOMLParts
)


# set output directories
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DICEBOX_ROOT}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DICEBOX_ROOT}/lib)
# set(RUNTIME_OUTPUT_DIRECTORY ${DICEBOX_BIN})
set_target_properties(${DICE_BOX} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${DICEBOX_BIN}
)

