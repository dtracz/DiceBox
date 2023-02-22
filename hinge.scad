include <globals.scad>
use <toolkit.scad>



module hinge_joint(thicc, ov_lgh) {
    difference() {
        union() {
            translate([OW_THICC/2, 0, 0])
                cube([ov_lgh+OW_THICC/2, thicc, OW_THICC]);
            translate([OW_THICC/2, 0, OW_THICC/2])
                rotate([-90, 0, 0])
                cylinder(thicc, OW_THICC/2, OW_THICC/2);
            cube([OW_THICC/2, thicc, OW_THICC/2]);
        }
        translate([OW_THICC/2, -1, OW_THICC/2])
            rotate([-90, 0, 0])
            cylinder(thicc+2, HRD/2, HRD/2);
    }
}


module generic_hinge(params, top_open) {
    offsets = [
        for (i=0, sum=0;
             i < len(params);
             sum = sum + (params[i][2]==undef ? 0 : params[i][2]), i=i+1
        )
            sum
    ];
    for(i = [0: 1 : len(params)-1]) {
        reflection = params[i][0]=="v" ? 1 : 0;
        rotation = params[i][0]=="v" ? 90-top_open : 0;
        color(params[i][1])
            translate([0, offsets[i], 0])
            rotate_around([OW_THICC/2, 0, OW_THICC/2], [0, rotation, 0])
            mirror_from([OW_THICC/2, 0, 0], [reflection, 0, 0])
            hinge_joint(params[i][2], params[i][3]);
    }
}


module hinge(top_open, EXPLODE) {
    c1 = [0.6, 0.9, 1];
    c2 = [0.55, 0.8, 0.95];
    c3 = [0.7, 1, 0.8];
    c4 = [0.60, 0.95, 0.70];
    params = [["h", c3, IW_THICC, BOTT_CUT],
              ["v", c1, IW_THICC, SIDE_CUT*2/3],
              ["v", c2, OW_THICC, SIDE_CUT],
              ["v", c1, IW_THICC, SIDE_CUT*2/3],
              ["h", c3, OW_THICC, BOTT_CUT],
              ["h", c4, OW_THICC, BOTT_CUT],
              ["v", c1, IW_THICC, SIDE_CUT*2/3],
              ["v", c2, OW_THICC, SIDE_CUT],
              ["v", c1, IW_THICC, SIDE_CUT*2/3],
              ["h", c3, OW_THICC, BOTT_CUT]];
    translate([0, OW_THICC, 0]) {
        translate([0, SLOT_SIZE.y+IW_THICC, 0])
            generic_hinge(params, top_open);
        translate([0, SLOT_SIZE.y, 0])
            mirror([0, 1, 0])
            generic_hinge(params, top_open);
        translate([0, EXPLODE > 0 ? -FULL_DIMS.y : 0, 0])
            translate([OW_THICC/2, -OW_THICC/3, OW_THICC/2])
            color([0.4, 0.4, 0.4])
            rotate([-90, 0, 0])
            cylinder(FULL_DIMS.y-OW_THICC*4/3, HRD/2, HRD/2);
    }
}


module cut_hinge_slots1() {
    difference() {
        children();
        union() {
            translate([OW_THICC-1, OW_THICC+SLOT_SIZE.y+6*IW_THICC++4*OW_THICC, -1])
                cube([BOTT_CUT+1, OW_THICC+1, OW_THICC+2]);
            translate([OW_THICC-1, OW_THICC+SLOT_SIZE.y+4*IW_THICC+OW_THICC, -1])
                cube([BOTT_CUT+1, OW_THICC*2, OW_THICC+2]);
            translate([OW_THICC-1, OW_THICC+SLOT_SIZE.y+IW_THICC-1, -1])
                cube([BOTT_CUT+1, IW_THICC+1, OW_THICC+2]);

            mirror_from([0, FULL_DIMS.y/2, 0], [0,1,0]) {
                translate([OW_THICC-1, OW_THICC+SLOT_SIZE.y+6*IW_THICC++4*OW_THICC, -1])
                    cube([BOTT_CUT+1, OW_THICC+1, OW_THICC+2]);
                translate([OW_THICC-1, OW_THICC+SLOT_SIZE.y+4*IW_THICC+OW_THICC, -1])
                    cube([BOTT_CUT+1, OW_THICC*2, OW_THICC+2]);
                translate([OW_THICC-1, OW_THICC+SLOT_SIZE.y+IW_THICC-1, -1])
                    cube([BOTT_CUT+1, IW_THICC+1, OW_THICC+2]);
            }
        }
    }
}


module cut_hinge_slots2() {
    module hinge_slot_cut() {
        translate([-1, 0, -1])
            cube([OW_THICC+2, IW_THICC+1, SIDE_CUT*2/3+1]);
        translate([-1, IW_THICC, -1])
            cube([OW_THICC+2, OW_THICC, SIDE_CUT+1]);
        translate([-1, IW_THICC+OW_THICC-1, -1])
            cube([OW_THICC+2, IW_THICC+1, SIDE_CUT*2/3+1]);
    }
    difference() {
        children();
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


TOP_OPEN = 0; // in degrees
EXPLODE = 0;

hinge(TOP_OPEN, EXPLODE);
