include <globals.scad>
use <toolkit.scad>
use <lower_part.scad>
use <main_closure.scad>


RS = 15;

//--COVER-------------------------------------------------------

module top_cover() {
    color([0,0.6,0])
    translate([FULL_DIMS.x/2, FULL_DIMS.y, FULL_DIMS.z])
    rotate([90, 0, 0])
    rotate([0, -90, 0])
    union() {
        translate([0,0,FULL_DIMS.x/2-(OW_THICC+RS)])
            crenellated_wall([FULL_DIMS.y, OW_THICC, OW_THICC+RS],
                [OW_THICC,CY,CY], [0,0,0], [OW_THICC,RS/3,RS/3], [OW_THICC,RS/3,RS/3]);
        translate([0,0,RS])
            cube([FULL_DIMS.y, OW_THICC, FULL_DIMS.x/2-OW_THICC-2*RS]);
        crenellated_wall([FULL_DIMS.y, OW_THICC, RS],
            [0,0,0], [IW_THICC,CY,CY], [OW_THICC,RS/3,RS/3], [OW_THICC,RS/3,RS/3]);
    }
}


module cover_wall() {
    color([0,1,1])
    rotate_around([OW_THICC/2, OW_THICC/2, 0], [0, 0, 90])
    union() {
        translate([0, 0, FULL_DIMS.z-RS])
            crenellated_wall([FULL_DIMS.y, OW_THICC, RS+OW_THICC],
                [OW_THICC,CY,0], [0,0,0], [OW_THICC,RS/3,RS/3], [OW_THICC,RS/3,RS/3]);
        cube([FULL_DIMS.y, OW_THICC, FULL_DIMS.z-RS]);
    }
}


module cover_sep() {
    color([0,1,1])
    translate([OW_THICC+SLOT_SIZE.x, OW_THICC, FULL_DIMS.z-RS])
    union() {
        rotate_around([IW_THICC/2, IW_THICC/2, 0], [0, 0, 90])
            crenellated_wall([FULL_DIMS.y-2*OW_THICC, IW_THICC, RS+OW_THICC], [OW_THICC,CY,-OW_THICC], [0,RS/3,RS/2], [0,RS/3,RS/2], [0,0,0]);
        translate([0, -OW_THICC, RS/2])
            cube([IW_THICC, OW_THICC, RS/3]);
        translate([0, FULL_DIMS.y-2*OW_THICC, RS/2])
            cube([IW_THICC, OW_THICC, RS/3]);
    }
}


module outer_reinforcement(rs) {
    color([0.8,0.8,0])
    translate([OW_THICC, 0, 0])
    union() {
        translate([0, OW_THICC, 0])
            rotate([90, 0, 0])
            linear_extrude(OW_THICC)
            polygon([[0,0], [rs, rs], [0,rs]]);
        translate([-OW_THICC, 0, rs])
            rotate([0,90,0])
            crenels(OW_THICC, rs/3, OW_THICC, 0, rs);
        translate([0, 0, rs])
            crenels(OW_THICC, rs/3, OW_THICC, 0, rs);
    }
}


module inner_reinforcement(rs) {
    color([0.8,0.8,0])
    union() {
        difference() {
            translate([0, OW_THICC, 0])
                rotate([90, 0, 0])
                linear_extrude(OW_THICC)
                polygon([[rs,0], [rs, rs], [0,rs]]);
            translate([rs-IW_THICC, -1, rs/2])
                cube([IW_THICC+1, OW_THICC+2, rs/3]);
        }
        translate([0, 0, rs])
            crenels(OW_THICC, rs/3, OW_THICC, 0, rs);
    }
}


module cover(EXPLODE) {
    translate([-EXPLODE, 0, 0])
        cover_wall();
    translate([0, -EXPLODE, 0])
        translate([0, 0, FULL_DIMS.z-RS]) {
            outer_reinforcement(RS);
            translate([FULL_DIMS.x/2-RS, 0, 0])
            inner_reinforcement(RS);
        }
    translate([0, EXPLODE, 0])
        translate([0, FULL_DIMS.y-OW_THICC, FULL_DIMS.z-RS]) {
            outer_reinforcement(RS);
            translate([FULL_DIMS.x/2-RS, 0, 0])
            inner_reinforcement(RS);
        }
    translate([0, 0, EXPLODE])
        top_cover();
    translate([EXPLODE, 0, 0])
        cover_sep();
}


//--MAIN-PART---------------------------------------------------


module upper_base(EXPLODE) {

}


module upper_part(top_open, EXPLODE) {
    upper_base(EXPLODE);
    translate([0, 0, EXPLODE])
        rotate_around([OW_THICC/2, 0, OW_THICC/2], [0, -top_open, 0])
        cover(EXPLODE);
}


TOP_OPEN = 0; // in degrees
EXPLODE = 0;

upper_part(TOP_OPEN, EXPLODE);
