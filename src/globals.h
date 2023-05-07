#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "base.h"


const double OW_THICC = 6;
const double IW_THICC = 4;
const Vec3 SLOT_SIZE {50, 50, 25};
const Vec3 FULL_DIMS {
    OW_THICC*2 + IW_THICC*2 + SLOT_SIZE.x*2,
    OW_THICC*2 + IW_THICC + SLOT_SIZE.y*2,
    OW_THICC + SLOT_SIZE.z
};
const double CZ = (FULL_DIMS.z - OW_THICC) / 5;
const double CX = (FULL_DIMS.x - 2*OW_THICC) / 11;
const double DX = SLOT_SIZE.x / 5;
const double CY = FULL_DIMS.y / 13;
const double SIDE_CUT = 2*CZ;
const double BOTT_CUT = CY;
const double CENT_CUT = CZ;

const double OPEN_SIZE = FULL_DIMS.z + OW_THICC;
const double UHP_Z = FULL_DIMS.z + OW_THICC + 4;
const double LHP_Z = UHP_Z - 26;
const std::pair<double, double> UHP = {OW_THICC + 39, UHP_Z};
const std::pair<double, double> LHP = {OW_THICC + 39 - OPEN_SIZE/2, LHP_Z};
const double SHIFT_TO_2ND = 10;

#endif  // GLOBALS_H_INCLUDED
