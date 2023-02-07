EPS = 1e-12;

module crenels(height, width, depth, offset, length) {
    for(i = [offset: 2*width : length]) {
        if (i + width < length + EPS)
            translate([i, 0, 0])
                cube([width, depth, height]);
    }
}

module crenellated_wall(dims,
                        crn_t = [0,0,0], // [height, width, offset
                        crn_b = [0,0,0],
                        crn_r = [0,0,0],
                        crn_l = [0,0,0]) {
    union() {
        translate([crn_l.x, 0, crn_b.x])
            cube([
                dims.x - crn_l.x - crn_r.x,
                dims.y,
                dims.z - crn_t.x - crn_b.x,
            ]);
        if (crn_t.x > 0) {
            translate([0, 0, dims.z - crn_t.x])
                crenels(crn_t.x, crn_t.y, dims.y, crn_t.z, dims.x);
        }
        if (crn_b.x > 0) {
            crenels(crn_b.x, crn_b.y, dims.y, crn_b.z, dims.x);
        }
        if (crn_r.x > 0) {
            translate([dims.x - crn_r.x, 0, dims.z - crn_t.x])
                rotate([0,90,0])
                crenels(crn_r.x, crn_r.y, dims.y, crn_r.z, dims.z-crn_t.x-crn_b.x);
        }
        if (crn_l.x > 0) {
            translate([0, 0, dims.z - crn_t.x])
                rotate([0,90,0])
                crenels(crn_l.x, crn_l.y, dims.y, crn_l.z, dims.z-crn_t.x-crn_b.x);
        }
    }
}


module rotate_around(pt, rotation) {
    translate(pt)
    rotate(rotation)
    translate(-pt)
    children();
}


OW_THICC = 5;
IW_THICC = 3;
SLOT_SIZE = [50, 50, 25];
ND_SLOT = [20, 2];
FULL_DIMS = [
    OW_THICC*2 + IW_THICC*2 + SLOT_SIZE.x*2,
    OW_THICC*2 + IW_THICC + SLOT_SIZE.y*2,
    OW_THICC + SLOT_SIZE.z
];
CZ = (FULL_DIMS.z - OW_THICC) / 5;
CX = (FULL_DIMS.x - 2*OW_THICC) / 11;
CY = FULL_DIMS.y / 13;
SIDE_CUT = 2*CZ;
BOTT_CUT = CY;


module lower_deck() {
    color([0,1,0])
    rotate([90, 0, 0])
    rotate([0, 90, 0])
    difference() {
        crenellated_wall([FULL_DIMS.y, OW_THICC, FULL_DIMS.x],
                [OW_THICC,CY,CY], [OW_THICC,CY,CY], [OW_THICC,CX,0], [OW_THICC,CX,0]);
        translate([OW_THICC-1, -1, OW_THICC + SLOT_SIZE.x])
            cube([BOTT_CUT+1, OW_THICC+2, 2*IW_THICC]);
        translate([FULL_DIMS.y-OW_THICC-BOTT_CUT, -1, OW_THICC + SLOT_SIZE.x])
            cube([BOTT_CUT+1, OW_THICC+2, 2*IW_THICC]);
        translate([OW_THICC + SLOT_SIZE.y, -1, OW_THICC-1])
            cube([IW_THICC, OW_THICC+2, BOTT_CUT+1]);
    }
}

module x_wall() {
    color([1,0,0])
    difference() {
        crenellated_wall([FULL_DIMS.x, OW_THICC, FULL_DIMS.z],
            [0,0,0], [OW_THICC,CX,CX+OW_THICC], [OW_THICC,CZ,0], [OW_THICC,CZ,0]);
        translate([OW_THICC + SLOT_SIZE.x, -1, OW_THICC+SLOT_SIZE.z-SIDE_CUT])
            cube([2*IW_THICC, OW_THICC+2, SIDE_CUT+1]);
    }
}

module ry_wall() {
    color([0,0,1])
    translate([FULL_DIMS.x-OW_THICC, 0, 0])
    rotate_around([OW_THICC/2, OW_THICC/2, 0], [0, 0, 90])
    crenellated_wall([FULL_DIMS.y, OW_THICC, FULL_DIMS.z],
        [0,0,0], [OW_THICC,CY,0], [OW_THICC,CZ,CZ], [OW_THICC,CZ,CZ]);
}

module ly_wall() {
    color([0,0,1])
    translate([-(FULL_DIMS.x-OW_THICC),0,0])
    difference() {
        ry_wall();
        translate([FULL_DIMS.x-OW_THICC-1, OW_THICC+SLOT_SIZE.y, OW_THICC+SLOT_SIZE.z-SIDE_CUT])
            cube([OW_THICC+2, IW_THICC, SIDE_CUT+1]);
    }
}

module ry_sep() {
    color([0,0,1])
    translate([OW_THICC+SLOT_SIZE.x+2*IW_THICC, 0, 0])
    rotate([0,0,90])
    union() {
        translate([OW_THICC, 0, OW_THICC])
            cube([FULL_DIMS.y-2*OW_THICC, IW_THICC, SLOT_SIZE.z]);
        translate([0, 0, FULL_DIMS.z-SIDE_CUT])
            cube([OW_THICC, IW_THICC, SIDE_CUT]);
        translate([FULL_DIMS.y-OW_THICC, 0, FULL_DIMS.z-SIDE_CUT])
            cube([OW_THICC, IW_THICC, SIDE_CUT]);
        translate([OW_THICC, 0, 0])
            cube([BOTT_CUT, IW_THICC, OW_THICC]);
        translate([FULL_DIMS.y-OW_THICC-BOTT_CUT, 0, 0])
            cube([BOTT_CUT, IW_THICC, OW_THICC]);
    }
}

module ly_sep() {
    color([0.2,0.8,1])
    translate([OW_THICC+SLOT_SIZE.x+IW_THICC, 0, 0])
    rotate([0,0,90])
    union() {
        translate([OW_THICC, 0, OW_THICC])
            cube([SLOT_SIZE.y, IW_THICC, SLOT_SIZE.z]);
        translate([0, 0, FULL_DIMS.z-SIDE_CUT])
            cube([OW_THICC, IW_THICC, SIDE_CUT]);
        translate([OW_THICC, 0, 0])
            cube([BOTT_CUT, IW_THICC, OW_THICC]);
    }
}

module x_sep() {
    color([1,0,0])
    translate([0, OW_THICC+SLOT_SIZE.y, 0])
    union() {
        translate([OW_THICC, 0, OW_THICC])
            cube([SLOT_SIZE.x+IW_THICC, IW_THICC, SLOT_SIZE.z]);
        translate([0, 0, FULL_DIMS.z-SIDE_CUT])
            cube([OW_THICC, IW_THICC, SIDE_CUT]);
        translate([OW_THICC, 0, 0])
            cube([BOTT_CUT, IW_THICC, OW_THICC]);
    }
}

module lower_part() {
    lower_deck();
    x_wall();
    translate([0, FULL_DIMS.y-OW_THICC, 0]) x_wall();
    ly_wall();
    ry_wall();
    ry_sep();
    ly_sep();
    x_sep();
    translate([0, FULL_DIMS.y, 0]) mirror([0,1,0]) ly_sep();
}

lower_part();
