include <globals.scad>
use <toolkit.scad>
use <main_closure.scad>



module lower_deck() {
    color([0,1,0])
    rotate([90, 0, 0])
    rotate([0, 90, 0])
    difference() {
        crenellated_wall([FULL_DIMS.y, OW_THICKNESS, FULL_DIMS.x],
                [OW_THICKNESS,CY,CY], [OW_THICKNESS,CY,CY], [OW_THICKNESS,CX,0], [OW_THICKNESS,CX,0]);
        translate([OW_THICKNESS-1, -1, OW_THICKNESS + SLOT_SIZE.x])
            cube([BOTT_CUT+1, OW_THICKNESS+2, 2*IW_THICKNESS]);
        translate([FULL_DIMS.y-OW_THICKNESS-BOTT_CUT, -1, OW_THICKNESS + SLOT_SIZE.x])
            cube([BOTT_CUT+1, OW_THICKNESS+2, 2*IW_THICKNESS]);
        translate([OW_THICKNESS + SLOT_SIZE.y, -1, OW_THICKNESS-1])
            cube([IW_THICKNESS, OW_THICKNESS+2, BOTT_CUT+1]);
    }
}


module x_wall() {
    color([1,0,0])
    difference() {
        rotate_around([FULL_DIMS.z/2, 0, FULL_DIMS.z/2], [0, 90, 0])
            crenellated_wall([FULL_DIMS.z, OW_THICKNESS, FULL_DIMS.x], [OW_THICKNESS,CZ,0], [OW_THICKNESS,CZ,0], [OW_THICKNESS,CX,CX], [0,0,0]);
        translate([OW_THICKNESS + SLOT_SIZE.x, -1, OW_THICKNESS+SLOT_SIZE.z-SIDE_CUT])
            cube([2*IW_THICKNESS, OW_THICKNESS+2, SIDE_CUT+1]);
        translate([-1, -1, -CZ/2])
            cube([OW_THICKNESS+2, OW_THICKNESS+2, CZ]);
        translate([FULL_DIMS.x-OW_THICKNESS-1, -1, -CZ/2])
            cube([OW_THICKNESS+2, OW_THICKNESS+2, CZ]);
    }
}


module ry_wall(thickness) {
    color([0,0,1])
    translate([FULL_DIMS.x-OW_THICKNESS, 0, 0])
    rotate_around([thickness/2, thickness/2, 0], [0, 0, 90])
    crenellated_wall([FULL_DIMS.y, thickness, FULL_DIMS.z],
        [0,0,0], [OW_THICKNESS,CY,0], [OW_THICKNESS,CZ,CZ], [OW_THICKNESS,CZ,CZ]);
}


module ly_wall(thickness) {
    color([0,0,1])
    translate([-(FULL_DIMS.x-OW_THICKNESS),0,0])
    difference() {
        ry_wall(thickness);
        translate([FULL_DIMS.x-OW_THICKNESS-1, OW_THICKNESS+SLOT_SIZE.y, OW_THICKNESS+SLOT_SIZE.z-SIDE_CUT])
            cube([OW_THICKNESS+2, IW_THICKNESS, SIDE_CUT+1]);
    }
}


module y_sep() {
    module half() {
        union() {
            translate([OW_THICKNESS, 0, OW_THICKNESS])
                cube([SLOT_SIZE.y, IW_THICKNESS, SLOT_SIZE.z]);
            translate([0, 0, FULL_DIMS.z-SIDE_CUT])
                cube([OW_THICKNESS, IW_THICKNESS, SIDE_CUT]);
            translate([OW_THICKNESS, 0, 0])
                cube([BOTT_CUT, IW_THICKNESS, OW_THICKNESS]);
        }
    }
    color([0,0,1])
    translate([OW_THICKNESS+SLOT_SIZE.x+IW_THICKNESS, 0, 0])
    rotate([0,0,90])
    union() {
        half();
        translate([FULL_DIMS.y, 0, 0])
            mirror([1,0,0])
            half();
        translate([OW_THICKNESS+SLOT_SIZE.y, 0, OW_THICKNESS+CENT_CUT])
            cube([IW_THICKNESS, IW_THICKNESS, SLOT_SIZE.z-2*CENT_CUT]);
    }
}


module x_sep() {
    color([1,0,0])
    translate([0, OW_THICKNESS+SLOT_SIZE.y, 0])
    union() {
        translate([OW_THICKNESS, 0, OW_THICKNESS])
            cube([SLOT_SIZE.x, IW_THICKNESS, SLOT_SIZE.z]);
        translate([0, 0, FULL_DIMS.z-SIDE_CUT])
            cube([OW_THICKNESS, IW_THICKNESS, SIDE_CUT]);
        translate([OW_THICKNESS, 0, 0])
            cube([BOTT_CUT, IW_THICKNESS, OW_THICKNESS]);
        translate([OW_THICKNESS+SLOT_SIZE.x, 0, FULL_DIMS.z-CENT_CUT])
            cube([2*IW_THICKNESS, IW_THICKNESS, CENT_CUT]);
        translate([OW_THICKNESS+SLOT_SIZE.x, 0, OW_THICKNESS])
            cube([2*IW_THICKNESS, IW_THICKNESS, CENT_CUT]);
    }
}


module lower_part(EXPLODE) {
    translate([0, 0, -EXPLODE])
        lower_deck();
    translate([0, -EXPLODE, 0])
        cut_lever_hole(LHP1)
        cut_lever_hole(LHP2)
        cut_lever_hole([FULL_DIMS.x - LHP1.x, LHP1.y])
        cut_lever_hole([FULL_DIMS.x - LHP2.x, LHP2.y])
        x_wall();
    translate([0, EXPLODE, 0])
        translate([0, FULL_DIMS.y-OW_THICKNESS, 0])
        cut_lever_hole(LHP1)
        cut_lever_hole(LHP2)
        cut_lever_hole([FULL_DIMS.x - LHP1.x, LHP1.y])
        cut_lever_hole([FULL_DIMS.x - LHP2.x, LHP2.y])
        x_wall();
    translate([-EXPLODE, 0, 0])
        ly_wall(OW_THICKNESS);
    translate([EXPLODE, 0, 0])
        ry_wall(OW_THICKNESS);
    translate([EXPLODE, 0, EXPLODE]) {
        color([0.5,0,1]) y_sep();
        translate([IW_THICKNESS, 0, 0]) y_sep();
    }
    translate([-EXPLODE/2, 0, EXPLODE])
        x_sep();
}


EXPLODE = 0;

lower_part(EXPLODE);
