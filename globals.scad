
OW_THICC = 6;
IW_THICC = 4;
SLOT_SIZE = [50, 50, 25];
ND_SLOT = [20, 2];
FULL_DIMS = [
    OW_THICC*2 + IW_THICC*2 + SLOT_SIZE.x*2,
    OW_THICC*2 + IW_THICC + SLOT_SIZE.y*2,
    OW_THICC + SLOT_SIZE.z
];
CZ = (FULL_DIMS.z - OW_THICC) / 5;
CX = (FULL_DIMS.x - 2*OW_THICC) / 11;
DX = SLOT_SIZE.x / 5;
CY = FULL_DIMS.y / 13;
SIDE_CUT = 2*CZ;
BOTT_CUT = CY;
CENT_CUT = CZ;
HRD = 2; // HINGE RHOD DIAMETER
COV_OV = 4*IW_THICC; // COVER OVERLAP


function lhp_calc(uhp, open_size, lev_z) = [
    (uhp.x + uhp.x-open_size) / 2,
    (FULL_DIMS.z + uhp.y - lev_z) % FULL_DIMS.z
];


OPEN_SIZE = SLOT_SIZE.x + IW_THICC;
HOLE_D = 4;
LEV_Z = 33;
UHP1 = [OW_THICC + 33, OW_THICC + SLOT_SIZE.z*1/2]; // UPPER HOLE POSITION
UHP2 = [OW_THICC + 43, OW_THICC + SLOT_SIZE.z*1/2];
LHP1 = lhp_calc(UHP1, OPEN_SIZE, LEV_Z);
LHP2 = lhp_calc(UHP2, OPEN_SIZE, LEV_Z);
SEP = 1;
LEV_THICC = [sin(atan2(LEV_Z, UHP1.x-LHP1.x))*(UHP2.x-UHP1.x), 4];
LEV_LGH = sqrt((UHP1.x - LHP1.x)^2 + (FULL_DIMS.z+UHP1.y - LHP1.y)^2);
ALPHA = atan2(FULL_DIMS.z+UHP1.y - LHP1.y, UHP1.x - LHP1.x);
