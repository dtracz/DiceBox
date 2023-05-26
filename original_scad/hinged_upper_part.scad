include <globals.scad>
use <toolkit.scad>
use <lower_part.scad>
use <hinge.scad>
use <main_closure.scad>


//--COVER-------------------------------------------------------

module half_top_cover() {
    translate([FULL_DIMS.x/2, FULL_DIMS.y, FULL_DIMS.z])
    rotate([90, 0, 0])
    rotate([0, -90, 0])
    union() {
        translate([0, 0, SLOT_SIZE.x-SLOT_SIZE.z+IW_THICKNESS])
            crenellated_wall([FULL_DIMS.y, OW_THICKNESS/2, SLOT_SIZE.z+OW_THICKNESS],
                [OW_THICKNESS,CY,CY], [0,0,0], [OW_THICKNESS,CZ,CZ], [OW_THICKNESS,CZ,CZ]);
        translate([0, 0, COV_OV/2])
            cube([FULL_DIMS.y, OW_THICKNESS/2, SLOT_SIZE.x-SLOT_SIZE.z+IW_THICKNESS-COV_OV/2]);
        translate([0, 0, -COV_OV/2])
            cube([FULL_DIMS.y/2, OW_THICKNESS/2, COV_OV]);
    }
}


module top_cover() {
    color([0,0.8,0])
        half_top_cover();
    color([0,0.6,0])
        rotate_around([0, FULL_DIMS.y/2, FULL_DIMS.z+OW_THICKNESS/2], [180, 0, 0])
        half_top_cover();
}


module cover_wall() {
    rotate_around([OW_THICKNESS/2, OW_THICKNESS/2, 0], [0, 0, 90])
        translate([0, 0, OW_THICKNESS])
        crenellated_wall([FULL_DIMS.y, OW_THICKNESS, FULL_DIMS.z],
            [OW_THICKNESS,CY,0], [0,0,0], [OW_THICKNESS,CZ,CZ], [OW_THICKNESS,CZ,CZ]);
}


module diag_reinforcement() {
    color([0.8,0.8,0])
    translate([OW_THICKNESS, 0, 0])
    union() {
        translate([0, OW_THICKNESS, OW_THICKNESS])
            rotate([90, 0, 0])
            linear_extrude(OW_THICKNESS)
            polygon([[0,0], [SLOT_SIZE.z, SLOT_SIZE.z], [0,SLOT_SIZE.z]]);
        translate([-OW_THICKNESS, 0, FULL_DIMS.z])
            rotate([0,90,0])
            crenels(OW_THICKNESS, CZ, OW_THICKNESS, 0, FULL_DIMS.z-OW_THICKNESS);
        translate([0, 0, FULL_DIMS.z])
            crenels(OW_THICKNESS, CZ, OW_THICKNESS, 0, SLOT_SIZE.z);
    }
}


module cover(EXPLODE) {
    translate([-EXPLODE, 0, 0])
        color([0,1,1])
        cut_hinge_slots2()
        cover_wall();
    translate([0, -EXPLODE, 0])
        diag_reinforcement();
    translate([0, EXPLODE, 0])
        translate([0, FULL_DIMS.y-OW_THICKNESS, 0])
        diag_reinforcement();
    translate([0, 0, EXPLODE])
        top_cover();
}


//--MAIN-PART---------------------------------------------------

module upper_deck() {
    color([0,0.7,0])
    rotate([90, 0, 0])
    rotate([0, 90, 0])
    difference() {
        translate([0, 0, OW_THICKNESS])
        crenellated_wall([FULL_DIMS.y, OW_THICKNESS, SLOT_SIZE.x+IW_THICKNESS],
                [IW_THICKNESS,CY,CY], [0,CY,CY], [OW_THICKNESS,DX,DX], [OW_THICKNESS,DX,DX]);
        translate([OW_THICKNESS+SLOT_SIZE.y, -1, OW_THICKNESS-1])
            cube([IW_THICKNESS, OW_THICKNESS+2, BOTT_CUT+1]);
        translate([OW_THICKNESS + SLOT_SIZE.y, -1, OW_THICKNESS+SLOT_SIZE.x-BOTT_CUT])
            cube([IW_THICKNESS, OW_THICKNESS+2, BOTT_CUT+1]);
    }
}


module xu_wall() {
    color([1,1,0])
    translate([OW_THICKNESS, 0, 0])
    union() {
        translate([0, OW_THICKNESS, OW_THICKNESS])
            rotate([90, 0, 0])
            linear_extrude(OW_THICKNESS)
            polygon([[0,0], [SLOT_SIZE.x,0], [SLOT_SIZE.x,SLOT_SIZE.z], [SLOT_SIZE.z, SLOT_SIZE.z]]);
        translate([SLOT_SIZE.x, 0, FULL_DIMS.z])
            rotate([0,90,0])
            crenels(IW_THICKNESS, CZ, OW_THICKNESS, 0, FULL_DIMS.z-OW_THICKNESS);
        crenels(OW_THICKNESS, DX, OW_THICKNESS, 0, SLOT_SIZE.x);
        translate([-OW_THICKNESS, 0, 0])
            difference() {
                union() {
                    translate([OW_THICKNESS/2, 0, 0])
                        cube([OW_THICKNESS/2, OW_THICKNESS, OW_THICKNESS]);
                    translate([OW_THICKNESS/2, 0, OW_THICKNESS/2])
                        rotate([-90, 0, 0])
                        cylinder(OW_THICKNESS, OW_THICKNESS/2, OW_THICKNESS/2);
                    cube([OW_THICKNESS/2, OW_THICKNESS, OW_THICKNESS/2]);
                }
                translate([OW_THICKNESS/2, OW_THICKNESS*3/5, OW_THICKNESS/2])
                    rotate([-90, 0, 0])
                    cylinder(OW_THICKNESS*2/5+1, HRD/2, HRD/2);
            }
    }
}


module xu_sep() {
    color([1,1,0])
    translate([0, OW_THICKNESS+SLOT_SIZE.y, 0])
    union() {
        translate([OW_THICKNESS, 0, OW_THICKNESS])
            cube([SLOT_SIZE.x, IW_THICKNESS, SLOT_SIZE.z]);
        hinge_joint(IW_THICKNESS, BOTT_CUT);
        translate([OW_THICKNESS+SLOT_SIZE.x, 0, FULL_DIMS.z-SIDE_CUT])
            cube([IW_THICKNESS, IW_THICKNESS, SIDE_CUT]);
        translate([OW_THICKNESS+SLOT_SIZE.x-BOTT_CUT, 0, 0])
            cube([BOTT_CUT, IW_THICKNESS, OW_THICKNESS]);
    }
}


module upper_base(EXPLODE) {
    translate([0, 0, -EXPLODE])
        cut_hinge_slots1()
        upper_deck();
    translate([EXPLODE, 0, 0])
        translate([OW_THICKNESS+SLOT_SIZE.x, 0, 0])
        color([0,1,1]) ly_wall(IW_THICKNESS);
    translate([0, -EXPLODE, 0])
        cut_lever_hole(UHP1)
        cut_lever_hole(UHP2)
        xu_wall();
    translate([0, EXPLODE, 0])
        mirror_from([0, FULL_DIMS.y/2, 0], [0,1,0])
        cut_lever_hole(UHP1)
        cut_lever_hole(UHP2)
        xu_wall();
    translate([0, 0, EXPLODE])
        xu_sep();
}


module upper_part(top_open, EXPLODE) {
    upper_base(EXPLODE);
    translate([0, 0, EXPLODE])
        rotate_around([OW_THICKNESS/2, 0, OW_THICKNESS/2], [0, -top_open, 0])
        cover(EXPLODE);
    translate([-EXPLODE, 0, -EXPLODE/2])
        hinge(top_open, EXPLODE);
}


TOP_OPEN = 90; // in degrees
EXPLODE = 0;

upper_part(TOP_OPEN, EXPLODE);
