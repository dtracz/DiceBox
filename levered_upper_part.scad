include <globals.scad>
use <toolkit.scad>
use <lower_part.scad>
use <main_closure.scad>


RS = 15;
DZ = (SLOT_SIZE.z-RS)/2;

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
    color([0,0.8,0.8])
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


module upper_deck() {
    color([0,0.7,0])
    rotate([90, 0, 0])
    rotate([0, 90, 0])
    difference() {
        translate([0, 0, OW_THICC])
        crenellated_wall([FULL_DIMS.y, OW_THICC, SLOT_SIZE.x+IW_THICC],
                [IW_THICC,CY,CY], [0,CY,CY], [OW_THICC,DX,DX], [OW_THICC,DX,DX]);
        translate([OW_THICC+SLOT_SIZE.y, -1, OW_THICC-1])
            cube([IW_THICC, OW_THICC+2, BOTT_CUT+1]);
        translate([OW_THICC + SLOT_SIZE.y, -1, OW_THICC+SLOT_SIZE.x-BOTT_CUT])
            cube([IW_THICC, OW_THICC+2, BOTT_CUT+1]);
    }
}


module xu_wall() {
    color([1,1,0])
    translate([OW_THICC, 0, 0])
    union() {
        rotate_around([0, OW_THICC/2, OW_THICC/2], [90, 0, 0])
        linear_extrude(OW_THICC)
        polygon([[SLOT_SIZE.x+IW_THICC, FULL_DIMS.z-RS],
                 [SLOT_SIZE.x+IW_THICC-RS, FULL_DIMS.z],
                 [RS, FULL_DIMS.z],
                 [0, FULL_DIMS.z-RS],
                 // [0,0],
                 // [SLOT_SIZE.x+IW_THICC, 0],
        ]);
        crenellated_wall([SLOT_SIZE.x+IW_THICC, OW_THICC, FULL_DIMS.z-RS],
                [0,0,0],[OW_THICC,DX,0],[IW_THICC,DZ,DZ],[0,0,0]);
    }
}

module yu_wall() {
    color([0,0,1])
    rotate_around([IW_THICC/2, IW_THICC/2, 0], [0, 0, 90])
    crenellated_wall([FULL_DIMS.y, IW_THICC, FULL_DIMS.z-RS],
        [0,0,0], [OW_THICC,CY,0], [OW_THICC,DZ,0], [OW_THICC,DZ,0]);
}


module xu_sep() {
    color([1,1,0])
    translate([OW_THICC, OW_THICC+SLOT_SIZE.y, 0])
    union() {
        translate([0, 0, OW_THICC])
            cube([SLOT_SIZE.x, IW_THICC, SLOT_SIZE.z]);
        cube([BOTT_CUT, IW_THICC, OW_THICC]);
//        translate([SLOT_SIZE.x, 0, FULL_DIMS.z-SIDE_CUT])
//            cube([IW_THICC, IW_THICC, SIDE_CUT]);
        translate([SLOT_SIZE.x-BOTT_CUT, 0, 0])
            cube([BOTT_CUT, IW_THICC, OW_THICC]);
    }
}


module upper_base(EXPLODE) {
    translate([0, 0, -EXPLODE])
        upper_deck();
    translate([EXPLODE, 0, 0])
        translate([OW_THICC+SLOT_SIZE.x, 0, 0])
        color([0,1,1])
        yu_wall();
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
        rotate_around([OW_THICC/2, 0, OW_THICC/2], [0, -top_open, 0])
        cover(EXPLODE);
}


TOP_OPEN = 0; // in degrees
EXPLODE = 0;

upper_part(TOP_OPEN, EXPLODE);
