include <globals.scad>
use <toolkit.scad>



module hinge_joint(thickness, ov_lgh) {
    difference() {
        union() {
            translate([OW_THICKNESS/2, 0, 0])
                cube([ov_lgh+OW_THICKNESS/2, thickness, OW_THICKNESS]);
            translate([OW_THICKNESS/2, 0, OW_THICKNESS/2])
                rotate([-90, 0, 0])
                cylinder(thickness, OW_THICKNESS/2, OW_THICKNESS/2);
            cube([OW_THICKNESS/2, thickness, OW_THICKNESS/2]);
        }
        translate([OW_THICKNESS/2, -1, OW_THICKNESS/2])
            rotate([-90, 0, 0])
            cylinder(thickness+2, HRD/2, HRD/2);
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
            rotate_around([OW_THICKNESS/2, 0, OW_THICKNESS/2], [0, rotation, 0])
            mirror_from([OW_THICKNESS/2, 0, 0], [reflection, 0, 0])
            hinge_joint(params[i][2], params[i][3]);
    }
}


module hinge(top_open, EXPLODE) {
    c1 = [0.6, 0.9, 1];
    c2 = [0.55, 0.8, 0.95];
    c3 = [0.7, 1, 0.8];
    c4 = [0.60, 0.95, 0.70];
    params = [["h", c3, IW_THICKNESS, BOTT_CUT],
              ["v", c1, IW_THICKNESS, SIDE_CUT*2/3],
              ["v", c2, OW_THICKNESS, SIDE_CUT],
              ["v", c1, IW_THICKNESS, SIDE_CUT*2/3],
              ["h", c3, OW_THICKNESS, BOTT_CUT],
              ["h", c4, OW_THICKNESS, BOTT_CUT],
              ["v", c1, IW_THICKNESS, SIDE_CUT*2/3],
              ["v", c2, OW_THICKNESS, SIDE_CUT],
              ["v", c1, IW_THICKNESS, SIDE_CUT*2/3],
              ["h", c3, OW_THICKNESS, BOTT_CUT]];
    translate([0, OW_THICKNESS, 0]) {
        translate([0, SLOT_SIZE.y+IW_THICKNESS, 0])
            generic_hinge(params, top_open);
        translate([0, SLOT_SIZE.y, 0])
            mirror([0, 1, 0])
            generic_hinge(params, top_open);
        translate([0, EXPLODE > 0 ? -FULL_DIMS.y : 0, 0])
            translate([OW_THICKNESS/2, -OW_THICKNESS/3, OW_THICKNESS/2])
            color([0.4, 0.4, 0.4])
            rotate([-90, 0, 0])
            cylinder(FULL_DIMS.y-OW_THICKNESS*4/3, HRD/2, HRD/2);
    }
}


module cut_hinge_slots1() {
    difference() {
        children();
        union() {
            translate([OW_THICKNESS-1, OW_THICKNESS+SLOT_SIZE.y+6*IW_THICKNESS++4*OW_THICKNESS, -1])
                cube([BOTT_CUT+1, OW_THICKNESS+1, OW_THICKNESS+2]);
            translate([OW_THICKNESS-1, OW_THICKNESS+SLOT_SIZE.y+4*IW_THICKNESS+OW_THICKNESS, -1])
                cube([BOTT_CUT+1, OW_THICKNESS*2, OW_THICKNESS+2]);
            translate([OW_THICKNESS-1, OW_THICKNESS+SLOT_SIZE.y+IW_THICKNESS-1, -1])
                cube([BOTT_CUT+1, IW_THICKNESS+1, OW_THICKNESS+2]);

            mirror_from([0, FULL_DIMS.y/2, 0], [0,1,0]) {
                translate([OW_THICKNESS-1, OW_THICKNESS+SLOT_SIZE.y+6*IW_THICKNESS++4*OW_THICKNESS, -1])
                    cube([BOTT_CUT+1, OW_THICKNESS+1, OW_THICKNESS+2]);
                translate([OW_THICKNESS-1, OW_THICKNESS+SLOT_SIZE.y+4*IW_THICKNESS+OW_THICKNESS, -1])
                    cube([BOTT_CUT+1, OW_THICKNESS*2, OW_THICKNESS+2]);
                translate([OW_THICKNESS-1, OW_THICKNESS+SLOT_SIZE.y+IW_THICKNESS-1, -1])
                    cube([BOTT_CUT+1, IW_THICKNESS+1, OW_THICKNESS+2]);
            }
        }
    }
}


module cut_hinge_slots2() {
    module hinge_slot_cut() {
        translate([-1, 0, -1])
            cube([OW_THICKNESS+2, IW_THICKNESS+1, SIDE_CUT*2/3+1]);
        translate([-1, IW_THICKNESS, -1])
            cube([OW_THICKNESS+2, OW_THICKNESS, SIDE_CUT+1]);
        translate([-1, IW_THICKNESS+OW_THICKNESS-1, -1])
            cube([OW_THICKNESS+2, IW_THICKNESS+1, SIDE_CUT*2/3+1]);
    }
    difference() {
        children();
        translate([0, 2*OW_THICKNESS, OW_THICKNESS])
            hinge_slot_cut();
        translate([0, 5*OW_THICKNESS+2*IW_THICKNESS, OW_THICKNESS])
            hinge_slot_cut();
        translate([0, 6*OW_THICKNESS+7*IW_THICKNESS, OW_THICKNESS])
            hinge_slot_cut();
        translate([0, 9*OW_THICKNESS+9*IW_THICKNESS, OW_THICKNESS])
            hinge_slot_cut();
    }
}


TOP_OPEN = 0; // in degrees
EXPLODE = 0;

hinge(TOP_OPEN, EXPLODE);
