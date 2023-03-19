cmake_minimum_required(VERSION 3.0)


# project name
set(DICE_BOX "DiceBox")
# third parties
set(OOML "OOML")

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
    ${DICEBOX_SRC}/*.cpp
)
add_executable(${DICE_BOX} ${ALL_SRCS})

# set c++ standard to 20
set_property(TARGET ${DICE_BOX} PROPERTY CXX_STANDARD 20)


# set up third parties
add_subdirectory(${THIRD_PARTY}/OOML)


target_include_directories(${DICE_BOX}
    PUBLIC ${DICEBOX_SRC} 
    PUBLIC ${THIRD_PARTY}/OOML/src
    )

target_link_directories(${DICE_BOX}
    PUBLIC ${DICEBOX_SRC}
    PUBLIC ${THIRD_PARTY}/OOML/lib
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

