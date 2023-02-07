
module crenels(height, width, depth, offset, length) {
    for(i = [offset: 2*width : length]) {
        // real_width = min(width, length-i);
        if (i + width <= length)
            translate([i, 0, 0])
                cube([width, depth, height]);
    }
}

module crenellated_wall(dims,
                        crn_t = [0,0,0], // [height, width, offset
                        crn_r = [0,0,0],
                        crn_b = [0,0,0],
                        crn_l = [0,0,0]) {
    union() {
        translate([crn_l.x, 0, crn_b.x])
            cube([
                dims.x - crn_l.x - crn_r.x,
                dims.y,
                dims.z - crn_t.x - crn_b.x,
            ]);
        if (crn_t.x > 0) {
            translate([crn_l.x, 0, dims.z - crn_t.x])
                crenels(crn_t.x, crn_t.y, dims.y, crn_t.z, dims.x-crn_l.x);
        }
        if (crn_r.x > 0) {
            translate([dims.x - crn_r.x, 0, dims.z - crn_t.x])
                rotate([0,90,0])
                crenels(crn_r.x, crn_r.y, dims.y, crn_r.z, dims.z-crn_t.x);
        }
        if (crn_b.x > 0) {
            translate([dims.x - crn_r.x, 0, crn_b.x])
                rotate([0,180,0])
                crenels(crn_b.x, crn_b.y, dims.y, crn_b.z, dims.x-crn_r.x);
        }
        if (crn_l.x > 0) {
            translate([crn_l.x, 0, crn_b.x])
                rotate([0,270,0])
                crenels(crn_l.x, crn_l.y, dims.y, crn_l.z, dims.z-crn_b.x);
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
SLOT_SIZES = [50, 50, 25];
ND_SLOT = [20, 2];
FULL_DIMS = [
    OW_THICC*2 + IW_THICC*2 + SLOT_SIZES.x*2,
    OW_THICC*2 + IW_THICC + SLOT_SIZES.y*2,
    OW_THICC + SLOT_SIZES.z
];
CZ = FULL_DIMS.z / 6;
CX = FULL_DIMS.x / 13;
CY = (FULL_DIMS.y - 2*OW_THICC) / 11;

color([1,0,0])
    crenellated_wall([FULL_DIMS.x, OW_THICC, FULL_DIMS.z],
            [0,0,0], [OW_THICC,CZ,CZ], [OW_THICC,CX,-OW_THICC], [OW_THICC,CZ,-OW_THICC]);
color([1,0,0])
    translate([0, FULL_DIMS.y-OW_THICC, 0])
    crenellated_wall([FULL_DIMS.x, OW_THICC, FULL_DIMS.z],
            [0,0,0], [OW_THICC,CZ,CZ], [OW_THICC,CX,-OW_THICC], [OW_THICC,CZ,-OW_THICC]);
color([0,0,1])
    rotate_around([OW_THICC/2, OW_THICC/2, 0], [0, 0, 90])
    crenellated_wall([FULL_DIMS.y, OW_THICC, FULL_DIMS.z],
            [0,0,0], [OW_THICC,CZ,0], [OW_THICC,CY,CY], [OW_THICC,CZ,CZ-OW_THICC]);
color([0,0,1])
    translate([FULL_DIMS.x-OW_THICC, 0, 0])
    rotate_around([OW_THICC/2, OW_THICC/2, 0], [0, 0, 90])
    crenellated_wall([FULL_DIMS.y, OW_THICC, FULL_DIMS.z],
            [0,0,0], [OW_THICC,CZ,0], [OW_THICC,CY,CY], [OW_THICC,CZ,CZ-OW_THICC]);
color([0,1,0])
    rotate_around([0, OW_THICC/2, OW_THICC/2], [-90, 0, 0])
    crenellated_wall([FULL_DIMS.x, OW_THICC, FULL_DIMS.y],
            [OW_THICC,CX,CX-OW_THICC], [OW_THICC,CY,0], [OW_THICC,CX,CX-OW_THICC], [OW_THICC,CY,0]);
