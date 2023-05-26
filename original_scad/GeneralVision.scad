OW_THICKNESS = 8;
IW_THICKNESS = 5;
SLOT_SIZES = [50, 50, 25];
ND_SLOT = [20, 2];


module rotate_around(pt, rotation) {
    translate(pt)
    rotate(rotation)
    translate(-pt)
    children();
}


module top_part() {
    difference() {
        cube([1*SLOT_SIZES.x + (3/2)*OW_THICKNESS,
              2*SLOT_SIZES.y + 2*OW_THICKNESS + IW_THICKNESS,
              SLOT_SIZES.z + OW_THICKNESS]);
        translate([OW_THICKNESS, OW_THICKNESS, OW_THICKNESS])
            cube([SLOT_SIZES.x, SLOT_SIZES.y, SLOT_SIZES.z+1]);
        translate([OW_THICKNESS, OW_THICKNESS + IW_THICKNESS + SLOT_SIZES.y, OW_THICKNESS])
            cube([SLOT_SIZES.x, SLOT_SIZES.y, SLOT_SIZES.z+1]);
        translate([OW_THICKNESS + SLOT_SIZES.x -1,
                   OW_THICKNESS + SLOT_SIZES.y/2 + IW_THICKNESS + SLOT_SIZES.y,
                   OW_THICKNESS + SLOT_SIZES.z/2])
            rotate([0, 90, 0])
            cylinder(ND_SLOT.y+1, ND_SLOT.x/2, ND_SLOT.x/2);
        translate([OW_THICKNESS + SLOT_SIZES.x -1,
                   OW_THICKNESS + SLOT_SIZES.y/2,
                   OW_THICKNESS + SLOT_SIZES.z/2])
            rotate([0, 90, 0])
            cylinder(ND_SLOT.y+1, ND_SLOT.x/2, ND_SLOT.x/2);
    }
}


difference() {
    cube([2*SLOT_SIZES.x + 3*OW_THICKNESS,
          2*SLOT_SIZES.y + 2*OW_THICKNESS + IW_THICKNESS,
          SLOT_SIZES.z + OW_THICKNESS]);
    translate([OW_THICKNESS, OW_THICKNESS, OW_THICKNESS])
        cube([SLOT_SIZES.x, SLOT_SIZES.y, SLOT_SIZES.z+1]);
    translate([OW_THICKNESS, OW_THICKNESS + IW_THICKNESS + SLOT_SIZES.y, OW_THICKNESS])
        cube([SLOT_SIZES.x, SLOT_SIZES.y, SLOT_SIZES.z+1]);
    translate([2*OW_THICKNESS + SLOT_SIZES.x, OW_THICKNESS, OW_THICKNESS])
        cube([SLOT_SIZES.x, 2*SLOT_SIZES.y + IW_THICKNESS, SLOT_SIZES.z+1]);
}
translate([-((3/2)*OW_THICKNESS + SLOT_SIZES.x), 0, SLOT_SIZES.z + OW_THICKNESS])
    top_part();
rotate_around([(2*SLOT_SIZES.x + 3*OW_THICKNESS)/2,
               (2*SLOT_SIZES.y + 2*OW_THICKNESS + IW_THICKNESS)/2,
               0],
              [0, 0, 180])
    translate([-((3/2)*OW_THICKNESS + SLOT_SIZES.x), 0, SLOT_SIZES.z + OW_THICKNESS])
    top_part();

