EPS = 1e-12;


module crenels(height, width, depth, offset, length) {
    for(i = [offset: 2*width : length]) {
        if (i + width < length + EPS)
            translate([i, 0, 0])
                cube([width, depth, height]);
    }
}


module crenellated_wall(dims,
                        crn_t = [0,0,0], // [height, width, offset
                        crn_b = [0,0,0],
                        crn_r = [0,0,0],
                        crn_l = [0,0,0]) {
    union() {
        translate([crn_l.x, 0, crn_b.x])
            cube([
                dims.x - crn_l.x - crn_r.x,
                dims.y,
                dims.z - crn_t.x - crn_b.x,
            ]);
        if (crn_t.x > 0) {
            translate([0, 0, dims.z - crn_t.x])
                crenels(crn_t.x, crn_t.y, dims.y, crn_t.z, dims.x);
        }
        if (crn_b.x > 0) {
            crenels(crn_b.x, crn_b.y, dims.y, crn_b.z, dims.x);
        }
        if (crn_r.x > 0) {
            translate([dims.x - crn_r.x, 0, dims.z - crn_t.x])
                rotate([0,90,0])
                crenels(crn_r.x, crn_r.y, dims.y, crn_r.z, dims.z-crn_t.x-crn_b.x);
        }
        if (crn_l.x > 0) {
            translate([0, 0, dims.z - crn_t.x])
                rotate([0,90,0])
                crenels(crn_l.x, crn_l.y, dims.y, crn_l.z, dims.z-crn_t.x-crn_b.x);
        }
    }
}


module rotate_around(pt, rotation) {
    translate(pt)
    rotate(rotation)
    translate(-pt)
    children();
}

module mirror_from(translation, plane) {
    translate(translation)
    mirror(plane)
    translate(-translation)
    children();
}


OW_THICC = 6;
IW_THICC = 4;
SLOT_SIZE = [50, 50, 25];
ND_SLOT = [20, 2];
FULL_DIMS = [
    OW_THICC*2 + IW_THICC*2 + SLOT_SIZE.x*2,
    OW_THICC*2 + IW_THICC + SLOT_SIZE.y*2,
    OW_THICC + SLOT_SIZE.z
];
CZ = (FULL_DIMS.z - OW_THICC) / 5;
CX = (FULL_DIMS.x - 2*OW_THICC) / 11;
DX = SLOT_SIZE.x / 5;
CY = FULL_DIMS.y / 13;
SIDE_CUT = 2*CZ;
BOTT_CUT = CY;
CENT_CUT = CZ;
HRD = 2; // HINGE RHOD DIAMETER
COV_OV = 4*IW_THICC; // COVER OVERLAP

function lhp_calc(uhp, open_size, handle_size_z) = [
    (uhp.x + uhp.x-open_size) / 2,
    (FULL_DIMS.z + uhp.y - handle_size_z) % FULL_DIMS.z
];

OPEN_SIZE = SLOT_SIZE.x + IW_THICC;
HOLE_D = 4;
UHP1 = [OW_THICC + SLOT_SIZE.x*4/6, OW_THICC + SLOT_SIZE.z*1/3]; // UPPER HOLE POSITION
UHP2 = [OW_THICC + SLOT_SIZE.x*5/6, OW_THICC + SLOT_SIZE.z*1/3];
LHP1 = lhp_calc(UHP1, OPEN_SIZE, 25);
LHP2 = lhp_calc(UHP2, OPEN_SIZE, 25);



module handle_hole(position) {
    translate([position.x, -1, position.y])
    rotate([-90, 0, 0])
    cylinder(OW_THICC+2, HOLE_D/2, HOLE_D/2, $fn=100);
}


// DEPERCATED
//module quarter_cutter(r, h, $fn=40) {
//    difference() {
//        translate([0, -1, 0])
//            cube([r+1, h+2, r+1]);
//        translate([0, -2, 0])
//            rotate([-90, 0, 0])
//            cylinder(h+4, r, r, $fn=$fn);
//    }
//}


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
        handle_hole(LHP1);
        handle_hole(LHP2);
        handle_hole([FULL_DIMS.x - LHP1.x, LHP1.y]);
        handle_hole([FULL_DIMS.x - LHP2.x, LHP2.y]);
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
                            cylinder(OW_THICC, OW_THICC/2, OW_THICC/2, $fn=100);
                        cube([OW_THICC/2, OW_THICC, OW_THICC/2]);
                    }
                    translate([OW_THICC/2, OW_THICC*3/5, OW_THICC/2])
                        rotate([-90, 0, 0])
                        cylinder(OW_THICC*2/5+1, HRD/2, HRD/2, $fn=100);
                }
        }
        handle_hole(UHP1);
        handle_hole(UHP2);
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


module hinge_joint(thicc, ov_lgh) {
    difference() {
        union() {
            translate([OW_THICC/2, 0, 0])
                cube([ov_lgh+OW_THICC/2, thicc, OW_THICC]);
            translate([OW_THICC/2, 0, OW_THICC/2])
                rotate([-90, 0, 0])
                cylinder(thicc, OW_THICC/2, OW_THICC/2, $fn=100);
            cube([OW_THICC/2, thicc, OW_THICC/2]);
        }
        translate([OW_THICC/2, -1, OW_THICC/2])
            rotate([-90, 0, 0])
            cylinder(thicc+2, HRD/2, HRD/2, $fn=100);
    }
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


module generic_hinge(params, top_open=0) {
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


module hinge(top_open=0, decompose=0) {
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
        translate([0, decompose > 0 ? -FULL_DIMS.y : 0, 0])
            translate([OW_THICC/2, -OW_THICC/3, OW_THICC/2])
            color([0.4, 0.4, 0.4])
            rotate([-90, 0, 0])
            cylinder(FULL_DIMS.y-OW_THICC*4/3, HRD/2, HRD/2, $fn=100);
    }
}


module lower_part(decompose=0) {
    translate([0, 0, -decompose])
        lower_deck();
    translate([0, -decompose, 0])
        x_wall();
    translate([0, decompose, 0])
        translate([0, FULL_DIMS.y-OW_THICC, 0]) x_wall();
    translate([-decompose, 0, 0])
        ly_wall(OW_THICC);
    translate([decompose, 0, 0])
        ry_wall(OW_THICC);
    translate([decompose, 0, decompose]) {
        color([0.5,0,1]) y_sep();
        translate([IW_THICC, 0, 0]) y_sep();
    }
    translate([-decompose/2, 0, decompose])
        x_sep();
}


module upper_base(decompose=0) {
    translate([0, 0, -decompose])
        upper_deck();
    translate([decompose, 0, 0])
        translate([OW_THICC+SLOT_SIZE.x, 0, 0])
        color([0,1,1]) ly_wall(IW_THICC);
    translate([0, -decompose, 0])
        xu_wall();
    translate([0, decompose, 0])
        mirror_from([0, FULL_DIMS.y/2, 0], [0,1,0]) xu_wall();
    translate([0, 0, decompose])
        xu_sep();
}


module cover(decompose=0) {
    translate([-decompose, 0, 0])
        color([0,1,1]) cover_wall();
        diag_reinforcement();
    translate([0, decompose, 0])
        translate([0, FULL_DIMS.y-OW_THICC, 0]) diag_reinforcement();
    translate([0, 0, decompose])
        top_cover();
}


module upper_part(open=0, top_open=0, decompose=0) {
    x_trans = -open * OPEN_SIZE;
    translate([-2*decompose, 0, 3*decompose])
    translate([x_trans,0,FULL_DIMS.z]) {
        upper_base(decompose);
        translate([0, 0, decompose])
            rotate_around([OW_THICC/2, 0, OW_THICC/2], [0, -top_open, 0])
            cover(decompose);
        translate([-decompose, 0, -decompose/2])
            hinge(top_open, decompose);
    }
}

OPEN = 1;
TOP_OPEN = 90; // in degrees
DECOMPOSE = 0;

lower_part(DECOMPOSE);
upper_part(OPEN, TOP_OPEN, DECOMPOSE);
rotate_around(FULL_DIMS/2, [0, 0, 180])
    upper_part(OPEN, TOP_OPEN, DECOMPOSE);
