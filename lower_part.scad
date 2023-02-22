include <globals.scad>
use <toolkit.scad>
use <main_closure.scad>



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
        lever_hole(LHP1);
        lever_hole(LHP2);
        lever_hole([FULL_DIMS.x - LHP1.x, LHP1.y]);
        lever_hole([FULL_DIMS.x - LHP2.x, LHP2.y]);
    }
}


module ry_wall(thicc) {
    color([0,0,1])
    translate([FULL_DIMS.x-OW_THICC, 0, 0])
    rotate_around([thicc/2, thicc/2, 0], [0, 0, 90])
    crenellated_wall([FULL_DIMS.y, thicc, FULL_DIMS.z],
        [0,0,0], [OW_THICC,CY,0], [OW_THICC,CZ,CZ], [OW_THICC,CZ,CZ]);
}


module ly_wall(thicc) {
    color([0,0,1])
    translate([-(FULL_DIMS.x-OW_THICC),0,0])
    difference() {
        ry_wall(thicc);
        translate([FULL_DIMS.x-OW_THICC-1, OW_THICC+SLOT_SIZE.y, OW_THICC+SLOT_SIZE.z-SIDE_CUT])
            cube([OW_THICC+2, IW_THICC, SIDE_CUT+1]);
    }
}


module y_sep() {
    module half() {
        union() {
            translate([OW_THICC, 0, OW_THICC])
                cube([SLOT_SIZE.y, IW_THICC, SLOT_SIZE.z]);
            translate([0, 0, FULL_DIMS.z-SIDE_CUT])
                cube([OW_THICC, IW_THICC, SIDE_CUT]);
            translate([OW_THICC, 0, 0])
                cube([BOTT_CUT, IW_THICC, OW_THICC]);
        }
    }
    color([0,0,1])
    translate([OW_THICC+SLOT_SIZE.x+IW_THICC, 0, 0])
    rotate([0,0,90])
    union() {
        half();
        translate([FULL_DIMS.y, 0, 0])
            mirror([1,0,0])
            half();
        translate([OW_THICC+SLOT_SIZE.y, 0, OW_THICC+CENT_CUT])
            cube([IW_THICC, IW_THICC, SLOT_SIZE.z-2*CENT_CUT]);
    }
}


module x_sep() {
    color([1,0,0])
    translate([0, OW_THICC+SLOT_SIZE.y, 0])
    union() {
        translate([OW_THICC, 0, OW_THICC])
            cube([SLOT_SIZE.x, IW_THICC, SLOT_SIZE.z]);
        translate([0, 0, FULL_DIMS.z-SIDE_CUT])
            cube([OW_THICC, IW_THICC, SIDE_CUT]);
        translate([OW_THICC, 0, 0])
            cube([BOTT_CUT, IW_THICC, OW_THICC]);
        translate([OW_THICC+SLOT_SIZE.x, 0, FULL_DIMS.z-CENT_CUT])
            cube([2*IW_THICC, IW_THICC, CENT_CUT]);
        translate([OW_THICC+SLOT_SIZE.x, 0, OW_THICC])
            cube([2*IW_THICC, IW_THICC, CENT_CUT]);
    }
}


module lower_part() {
    translate([0, 0, -$EXPLODE])
        lower_deck();
    translate([0, -$EXPLODE, 0])
        x_wall();
    translate([0, $EXPLODE, 0])
        translate([0, FULL_DIMS.y-OW_THICC, 0]) x_wall();
    translate([-$EXPLODE, 0, 0])
        ly_wall(OW_THICC);
    translate([$EXPLODE, 0, 0])
        ry_wall(OW_THICC);
    translate([$EXPLODE, 0, $EXPLODE]) {
        color([0.5,0,1]) y_sep();
        translate([IW_THICC, 0, 0]) y_sep();
    }
    translate([-$EXPLODE/2, 0, $EXPLODE])
        x_sep();
}
