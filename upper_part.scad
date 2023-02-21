include <lower_part.scad>
include <hinge.scad>



module upper_deck_hinge_slots() {
    translate([OW_THICC+SLOT_SIZE.y+4*IW_THICC+OW_THICC, -1, OW_THICC-1])
        cube([OW_THICC*2, OW_THICC+2, BOTT_CUT+1]);
    translate([OW_THICC+SLOT_SIZE.y+6*IW_THICC++4*OW_THICC, -1, OW_THICC-1])
        cube([OW_THICC+1, OW_THICC+2, BOTT_CUT+1]);
    translate([OW_THICC+SLOT_SIZE.y+IW_THICC-1, -1, OW_THICC-1])
        cube([IW_THICC+1, OW_THICC+2, BOTT_CUT+1]);
}


module upper_deck() {
    color([0,0.7,0])
    rotate([90, 0, 0])
    rotate([0, 90, 0])
    difference() {
        translate([0, 0, OW_THICC])
        crenellated_wall([FULL_DIMS.y, OW_THICC, SLOT_SIZE.x+IW_THICC],
                [IW_THICC,CY,CY], [0,CY,CY], [OW_THICC,DX,DX], [OW_THICC,DX,DX]);
        upper_deck_hinge_slots();
        mirror_from([FULL_DIMS.y/2, 0, 0], [1,0,0])
            upper_deck_hinge_slots();
        translate([OW_THICC+SLOT_SIZE.y, -1, OW_THICC-1])
            cube([IW_THICC, OW_THICC+2, BOTT_CUT+1]);
        translate([OW_THICC + SLOT_SIZE.y, -1, OW_THICC+SLOT_SIZE.x-BOTT_CUT])
            cube([IW_THICC, OW_THICC+2, BOTT_CUT+1]);
    }
}


module xu_wall() {
    color([1,1,0])
    difference() {
        translate([OW_THICC, 0, 0])
        union() {
            translate([0, OW_THICC, OW_THICC])
                rotate([90, 0, 0])
                linear_extrude(OW_THICC)
                polygon([[0,0], [SLOT_SIZE.x,0], [SLOT_SIZE.x,SLOT_SIZE.z], [SLOT_SIZE.z, SLOT_SIZE.z]]);
            translate([SLOT_SIZE.x, 0, FULL_DIMS.z])
                rotate([0,90,0])
                crenels(IW_THICC, CZ, OW_THICC, 0, FULL_DIMS.z-OW_THICC);
            crenels(OW_THICC, DX, OW_THICC, 0, SLOT_SIZE.x);
            translate([-OW_THICC, 0, 0])
                difference() {
                    union() {
                        translate([OW_THICC/2, 0, 0])
                            cube([OW_THICC/2, OW_THICC, OW_THICC]);
                        translate([OW_THICC/2, 0, OW_THICC/2])
                            rotate([-90, 0, 0])
                            cylinder(OW_THICC, OW_THICC/2, OW_THICC/2);
                        cube([OW_THICC/2, OW_THICC, OW_THICC/2]);
                    }
                    translate([OW_THICC/2, OW_THICC*3/5, OW_THICC/2])
                        rotate([-90, 0, 0])
                        cylinder(OW_THICC*2/5+1, HRD/2, HRD/2);
                }
        }
        lever_hole(UHP1);
        lever_hole(UHP2);
    }
}


module cover_wall() {
    module hinge_slot_cut() {
        translate([-1, 0, -1])
            cube([OW_THICC+2, IW_THICC+1, SIDE_CUT*2/3+1]);
        translate([-1, IW_THICC, -1])
            cube([OW_THICC+2, OW_THICC, SIDE_CUT+1]);
        translate([-1, IW_THICC+OW_THICC-1, -1])
            cube([OW_THICC+2, IW_THICC+1, SIDE_CUT*2/3+1]);
    }
    difference() {
        rotate_around([OW_THICC/2, OW_THICC/2, 0], [0, 0, 90])
            translate([0, 0, OW_THICC])
            crenellated_wall([FULL_DIMS.y, OW_THICC, FULL_DIMS.z],
                [OW_THICC,CY,0], [0,0,0], [OW_THICC,CZ,CZ], [OW_THICC,CZ,CZ]);
        translate([0, 2*OW_THICC, OW_THICC])
            hinge_slot_cut();
        translate([0, 5*OW_THICC+2*IW_THICC, OW_THICC])
            hinge_slot_cut();
        translate([0, 6*OW_THICC+7*IW_THICC, OW_THICC])
            hinge_slot_cut();
        translate([0, 9*OW_THICC+9*IW_THICC, OW_THICC])
            hinge_slot_cut();
    }
}


module diag_reinforcement() {
    color([0.8,0.8,0])
    translate([OW_THICC, 0, 0])
    union() {
        translate([0, OW_THICC, OW_THICC])
            rotate([90, 0, 0])
            linear_extrude(OW_THICC)
            polygon([[0,0], [SLOT_SIZE.z, SLOT_SIZE.z], [0,SLOT_SIZE.z]]);
        translate([-OW_THICC, 0, FULL_DIMS.z])
            rotate([0,90,0])
            crenels(OW_THICC, CZ, OW_THICC, 0, FULL_DIMS.z-OW_THICC);
        translate([0, 0, FULL_DIMS.z])
            crenels(OW_THICC, CZ, OW_THICC, 0, SLOT_SIZE.z);
    }
}


module half_top_cover() {
    translate([FULL_DIMS.x/2, FULL_DIMS.y, FULL_DIMS.z])
    rotate([90, 0, 0])
    rotate([0, -90, 0])
    union() {
        translate([0, 0, SLOT_SIZE.x-SLOT_SIZE.z+IW_THICC])
            crenellated_wall([FULL_DIMS.y, OW_THICC/2, SLOT_SIZE.z+OW_THICC],
                [OW_THICC,CY,CY], [0,0,0], [OW_THICC,CZ,CZ], [OW_THICC,CZ,CZ]);
        translate([0, 0, COV_OV/2])
            cube([FULL_DIMS.y, OW_THICC/2, SLOT_SIZE.x-SLOT_SIZE.z+IW_THICC-COV_OV/2]);
        translate([0, 0, -COV_OV/2])
            cube([FULL_DIMS.y/2, OW_THICC/2, COV_OV]);
    }
}


module top_cover() {
    color([0,0.8,0])
        half_top_cover();
    color([0,0.6,0])
        rotate_around([0, FULL_DIMS.y/2, FULL_DIMS.z+OW_THICC/2], [180, 0, 0])
        half_top_cover();
}


module xu_sep() {
    color([1,1,0])
    translate([0, OW_THICC+SLOT_SIZE.y, 0])
    union() {
        translate([OW_THICC, 0, OW_THICC])
            cube([SLOT_SIZE.x, IW_THICC, SLOT_SIZE.z]);
        hinge_joint(IW_THICC, BOTT_CUT);
        translate([OW_THICC+SLOT_SIZE.x, 0, FULL_DIMS.z-SIDE_CUT])
            cube([IW_THICC, IW_THICC, SIDE_CUT]);
        translate([OW_THICC+SLOT_SIZE.x-BOTT_CUT, 0, 0])
            cube([BOTT_CUT, IW_THICC, OW_THICC]);
    }
}


module upper_base() {
    translate([0, 0, -$EXPLODE])
        upper_deck();
    translate([$EXPLODE, 0, 0])
        translate([OW_THICC+SLOT_SIZE.x, 0, 0])
        color([0,1,1]) ly_wall(IW_THICC);
    translate([0, -$EXPLODE, 0])
        xu_wall();
    translate([0, $EXPLODE, 0])
        mirror_from([0, FULL_DIMS.y/2, 0], [0,1,0]) xu_wall();
    translate([0, 0, $EXPLODE])
        xu_sep();
}


module cover() {
    translate([-$EXPLODE, 0, 0])
        color([0,1,1]) cover_wall();
        diag_reinforcement();
    translate([0, $EXPLODE, 0])
        translate([0, FULL_DIMS.y-OW_THICC, 0]) diag_reinforcement();
    translate([0, 0, $EXPLODE])
        top_cover();
}


module upper_part() {
    translate([-2*$EXPLODE, 0, 3*$EXPLODE])
    translate([0,0,FULL_DIMS.z]) {
        upper_base();
        translate([0, 0, $EXPLODE])
            rotate_around([OW_THICC/2, 0, OW_THICC/2], [0, -$TOP_OPEN, 0])
            cover();
        translate([-$EXPLODE, 0, -$EXPLODE/2])
            hinge();
    }
}
