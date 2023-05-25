#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "base/geometry.h"


constexpr double OW_THICKNESS = 6;
constexpr double IW_THICKNESS = 4;
constexpr Vec3 SLOT_SIZE { 50, 50, 25 };
constexpr Vec3 FULL_DIMS {
    OW_THICKNESS * 2 + IW_THICKNESS * 2 + SLOT_SIZE.x * 2,
    OW_THICKNESS * 2 + IW_THICKNESS + SLOT_SIZE.y * 2,
    OW_THICKNESS + SLOT_SIZE.z,
};
constexpr double CZ = (FULL_DIMS.z - OW_THICKNESS) / 5;
constexpr double CX = (FULL_DIMS.x - 2 * OW_THICKNESS) / 11;
constexpr double DX = SLOT_SIZE.x / 5;
constexpr double CY = FULL_DIMS.y / 13;
constexpr double SIDE_CUT = 2 * CZ;
constexpr double BOTT_CUT = CY;
constexpr double CENT_CUT = CZ;

constexpr double OPEN_SIZE = FULL_DIMS.z + OW_THICKNESS;
constexpr double UHP_Z = FULL_DIMS.z + OW_THICKNESS + 4;
constexpr double LHP_Z = UHP_Z - 26;
constexpr Vec2 UHP = { OW_THICKNESS + 39, UHP_Z };
constexpr Vec2 LHP = { OW_THICKNESS + 39 - OPEN_SIZE / 2, LHP_Z };
constexpr double SHIFT_TO_2ND = 10;

#endif // GLOBALS_H_INCLUDED
