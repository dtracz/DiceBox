use <toolkit.scad>
include <lower_part.scad>

$EPS = 1e-12;
$fn=100;

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

function lhp_calc(uhp, open_size, lev_z) = [
    (uhp.x + uhp.x-open_size) / 2,
    (FULL_DIMS.z + uhp.y - lev_z) % FULL_DIMS.z
];

OPEN_SIZE = SLOT_SIZE.x + IW_THICC;
HOLE_D = 4;
LEV_Z = 33;
UHP1 = [OW_THICC + 33, OW_THICC + SLOT_SIZE.z*1/2]; // UPPER HOLE POSITION
UHP2 = [OW_THICC + 43, OW_THICC + SLOT_SIZE.z*1/2];
LHP1 = lhp_calc(UHP1, OPEN_SIZE, LEV_Z);
LHP2 = lhp_calc(UHP2, OPEN_SIZE, LEV_Z);
SEP = 1;
LEV_THICC = [sin(atan2(LEV_Z, UHP1.x-LHP1.x))*(UHP2.x-UHP1.x), 4];
LEV_LGH = sqrt((UHP1.x - LHP1.x)^2 + (FULL_DIMS.z+UHP1.y - LHP1.y)^2);



module lever_hole(position) {
    translate([position.x, -1, position.y])
    rotate([-90, 0, 0])
    cylinder(OW_THICC+2, HOLE_D/2, HOLE_D/2);
}


module lever_plug() {
    translate([0, -LEV_THICC.y-SEP, 0])
    union() {
        rotate([-90, 0, 0])
            cylinder(OW_THICC+SEP+LEV_THICC.y, HOLE_D/2, HOLE_D/2);
        difference() {
            sphere(HOLE_D);
            translate([-HOLE_D-1, 0, -HOLE_D-1])
            cube(2*HOLE_D+2);
        }
    }
}


module lever_bar(length) {
    translate([0, 0, -LEV_THICC.x/2])
    difference() {
        union() {
            cube([length, LEV_THICC.y, LEV_THICC.x]);
            translate([0, 0, LEV_THICC.x/2])
                rotate([-90, 0, 0])
                cylinder(LEV_THICC.y, LEV_THICC.x/2, LEV_THICC.x/2);
            translate([length, 0, LEV_THICC.x/2])
                rotate([-90, 0, 0])
                cylinder(LEV_THICC.y, LEV_THICC.x/2, LEV_THICC.x/2);
        }
        translate([0, -1, LEV_THICC.x/2])
            rotate([-90, 0, 0])
            cylinder(LEV_THICC.y+2, HOLE_D/2, HOLE_D/2);
        translate([length, -1, LEV_THICC.x/2])
            rotate([-90, 0, 0])
            cylinder(LEV_THICC.y+2, HOLE_D/2, HOLE_D/2);
    }
}


module lever(alpha, lhp, c) {
    color(c)
        translate([lhp.x, -$EXPLODE*3/2, lhp.y])
        rotate([0, -alpha, 0])
        translate([0, -LEV_THICC.y-SEP, 0])
        lever_bar(LEV_LGH);
    translate([lhp.x, -$EXPLODE*5/2, lhp.y])
        lever_plug();
    translate([lhp.x + LEV_LGH*cos(alpha), -$EXPLODE*5/2, lhp.y + LEV_LGH*sin(alpha)])
        lever_plug();
}


module closure_levers(alpha) {
    c1 = [1, 0.5, 0];
    c2 = [1, 0.7, 0];
    lever(alpha, LHP1, c1);
    lever(alpha, LHP2, c2);
    mirror_from(FULL_DIMS/2, [0,1,0]) {
        lever(alpha, LHP1, c1);
        lever(alpha, LHP2, c2);
    }
    mirror_from(FULL_DIMS/2, [1,0,0]) {
        lever(alpha, LHP1, c1);
        lever(alpha, LHP2, c2);
        mirror_from(FULL_DIMS/2, [0,1,0]) {
            lever(alpha, LHP1, c1);
            lever(alpha, LHP2, c2);
        }
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


module generic_hinge(params) {
    offsets = [
        for (i=0, sum=0;
             i < len(params);
             sum = sum + (params[i][2]==undef ? 0 : params[i][2]), i=i+1
        )
            sum
    ];
    for(i = [0: 1 : len(params)-1]) {
        reflection = params[i][0]=="v" ? 1 : 0;
        rotation = params[i][0]=="v" ? 90-$TOP_OPEN : 0;
        color(params[i][1])
            translate([0, offsets[i], 0])
            rotate_around([OW_THICC/2, 0, OW_THICC/2], [0, rotation, 0])
            mirror_from([OW_THICC/2, 0, 0], [reflection, 0, 0])
            hinge_joint(params[i][2], params[i][3]);
    }
}


module hinge() {
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
            generic_hinge(params);
        translate([0, SLOT_SIZE.y, 0])
            mirror([0, 1, 0])
            generic_hinge(params);
        translate([0, $EXPLODE > 0 ? -FULL_DIMS.y : 0, 0])
            translate([OW_THICC/2, -OW_THICC/3, OW_THICC/2])
            color([0.4, 0.4, 0.4])
            rotate([-90, 0, 0])
            cylinder(FULL_DIMS.y-OW_THICC*4/3, HRD/2, HRD/2);
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


module box() {
    alpha = atan2(FULL_DIMS.z+UHP1.y - LHP1.y, UHP1.x - LHP1.x);
    beta = 90-alpha;
    curr_alpha = alpha + 2*$OPEN*beta;
    pos0 = [LEV_LGH*cos(alpha), 0, LEV_LGH*sin(alpha)];
    curr_pos = [LEV_LGH*cos(curr_alpha), 0, LEV_LGH*sin(curr_alpha)];

    lower_part();
    translate(curr_pos-pos0)
        upper_part();
    rotate_around(FULL_DIMS/2, [0, 0, 180])
        translate(curr_pos-pos0)
        upper_part();
    closure_levers(curr_alpha);
}


$OPEN = 1;
$TOP_OPEN = 90; // in degrees
$EXPLODE = 0;

box();
