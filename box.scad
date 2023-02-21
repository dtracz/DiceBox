use <toolkit.scad>
include <lower_part.scad>
include <upper_part.scad>



$EPS = 1e-12;
$fn=100;

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



module lever_hole(position) {
    translate([position.x, -1, position.y])
    rotate([-90, 0, 0])
    cylinder(OW_THICC+2, HOLE_D/2, HOLE_D/2);
}


module lever_plug() {
    translate([0, -LEV_THICC.y-SEP, 0])
    union() {
        rotate([-90, 0, 0])
            cylinder(OW_THICC+SEP+LEV_THICC.y, HOLE_D/2, HOLE_D/2);
        difference() {
            sphere(HOLE_D);
            translate([-HOLE_D-1, 0, -HOLE_D-1])
            cube(2*HOLE_D+2);
        }
    }
}


module lever_bar(length) {
    translate([0, 0, -LEV_THICC.x/2])
    difference() {
        union() {
            cube([length, LEV_THICC.y, LEV_THICC.x]);
            translate([0, 0, LEV_THICC.x/2])
                rotate([-90, 0, 0])
                cylinder(LEV_THICC.y, LEV_THICC.x/2, LEV_THICC.x/2);
            translate([length, 0, LEV_THICC.x/2])
                rotate([-90, 0, 0])
                cylinder(LEV_THICC.y, LEV_THICC.x/2, LEV_THICC.x/2);
        }
        translate([0, -1, LEV_THICC.x/2])
            rotate([-90, 0, 0])
            cylinder(LEV_THICC.y+2, HOLE_D/2, HOLE_D/2);
        translate([length, -1, LEV_THICC.x/2])
            rotate([-90, 0, 0])
            cylinder(LEV_THICC.y+2, HOLE_D/2, HOLE_D/2);
    }
}


module lever(alpha, lhp, c) {
    color(c)
        translate([lhp.x, -$EXPLODE*3/2, lhp.y])
        rotate([0, -alpha, 0])
        translate([0, -LEV_THICC.y-SEP, 0])
        lever_bar(LEV_LGH);
    translate([lhp.x, -$EXPLODE*5/2, lhp.y])
        lever_plug();
    translate([lhp.x + LEV_LGH*cos(alpha), -$EXPLODE*5/2, lhp.y + LEV_LGH*sin(alpha)])
        lever_plug();
}


module closure_levers(alpha) {
    c1 = [1, 0.5, 0];
    c2 = [1, 0.7, 0];
    lever(alpha, LHP1, c1);
    lever(alpha, LHP2, c2);
    mirror_from(FULL_DIMS/2, [0,1,0]) {
        lever(alpha, LHP1, c1);
        lever(alpha, LHP2, c2);
    }
    mirror_from(FULL_DIMS/2, [1,0,0]) {
        lever(alpha, LHP1, c1);
        lever(alpha, LHP2, c2);
        mirror_from(FULL_DIMS/2, [0,1,0]) {
            lever(alpha, LHP1, c1);
            lever(alpha, LHP2, c2);
        }
    }
}


module box() {
    alpha = atan2(FULL_DIMS.z+UHP1.y - LHP1.y, UHP1.x - LHP1.x);
    beta = 90-alpha;
    curr_alpha = alpha + 2*$OPEN*beta;
    pos0 = [LEV_LGH*cos(alpha), 0, LEV_LGH*sin(alpha)];
    curr_pos = [LEV_LGH*cos(curr_alpha), 0, LEV_LGH*sin(curr_alpha)];

    lower_part();
    translate(curr_pos-pos0)
        upper_part();
    rotate_around(FULL_DIMS/2, [0, 0, 180])
        translate(curr_pos-pos0)
        upper_part();
    closure_levers(curr_alpha);
}


$OPEN = 1;
$TOP_OPEN = 90; // in degrees
$EXPLODE = 0;

box();
