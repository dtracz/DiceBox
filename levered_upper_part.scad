include <globals.scad>
use <toolkit.scad>
use <lower_part.scad>
use <main_closure.scad>


RS = 15;
DZ = (SLOT_SIZE.z-RS)/2;
TLHD = 3; // top lever hole diameter;

MP_X = RS*4/5;
MP_Y = RS*1/2;

//--CALCULATIONS------------------------------------------------
// Lower lever schme
// S---R-A
//      \|
//       E
// SR = RE
SA = FULL_DIMS.z;
AE = FULL_DIMS.x/2 - FULL_DIMS.z - IW_THICC;
RE = (AE^2 + SA^2) / (2*SA);
S = [OW_THICC/2, OW_THICC/2];
R = S + [RE, 0];
E = S + [SA, -AE];

// Upper lever schme (possibly rotated)
// C       D
//   \   /
//     T
C = [OW_THICC + MP_Y, FULL_DIMS.z-RS + MP_X];
D = [OW_THICC+SLOT_SIZE.x-RS + MP_X, FULL_DIMS.z - MP_Y];
// returns point basing on it's y position
function line_T(y) = let(T_ = (C+D)/2) T_ + [-((T_-C).y/(T_-C).x)*y, y];
T = line_T(-2.25);
echo(D.y);
echo(T.y);
// CTD angle
UP_LEV_OFFSET = atan2(D.y - T.y, D.x - T.x);
UP_LEV_RANGE = (atan2(C.y - T.y, C.x - T.x) - UP_LEV_OFFSET + 360) % 360;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

function len2D(v) = sqrt((v.x)^2 + (v.y)^2);

function rot2D(O, X, theta) = [(X-O).x*cos(theta) - (X-O).y*sin(theta),
        (X-O).x*sin(theta) + (X-O).y*cos(theta)]+O;

function get_current_D(open) = rot2D(T, D, open*UP_LEV_RANGE);

function get_current_S(open) =
 let( mov_D = get_current_D(open)                         )
 let( center_dist = len2D(R - mov_D)                      )
 let( cos_theta = (center_dist^2 + RE^2 - len2D(D-S)^2) /
                     (2*center_dist*RE)                   )
 let( sin_theta = sqrt(1 - cos_theta^2)                   )
 let( vp = RE * (mov_D - R) / center_dist                 )
      [ vp.x*cos_theta - vp.y*sin_theta + R.x,
        vp.x*sin_theta + vp.y*cos_theta + R.y,
      ];

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

module nail(pos, c=[1,0,0]) {
    translate([pos.x, 2, pos.y])
    rotate([90, 0, 0])
    % color([c.x,c.y,c.z,0.8])
    cylinder(10, 1, 1);
}


module arc(center, r, a0, a, c) {
    translate([center.x, 0, center.y]) {
        translate([0, -4, 0])
        rotate([90, 0, 0])
        % color([c.x,c.y,c.z,0.2])
        cylinder(2, r, r);
        rotate([90, -a0, 0])
        % color([c.x,c.y,c.z,0.5])
        rotate_extrude(angle=a, $fn=1000)
        polygon([[0,4], [r,4], [r,6], [0,6]]);
    }
}


module math_visualisation(open) {
    mov_D = get_current_D(open);
    mov_S = get_current_S(open);
    nail(C);
    nail(D);
    nail(T);
    translate([0,1,0])
        arc(T, len2D(D-T), UP_LEV_OFFSET, UP_LEV_RANGE, [1,1,1]);
    nail(S);
    nail(R);
    nail(E);
    max_gamma = 180 - asin(AE/RE);
    arc(R, RE, 180, max_gamma, [1,0,1]);
    translate([0,-0.5,0])
        arc(mov_D, len2D(D-S) , 0, 0, [0,0,1]);
    nail(mov_D, [0,0,1]);
    nail(mov_S, [0,1,0]);
}

//--COVER-------------------------------------------------------

module top_cover() {
    color([0,0.6,0])
    translate([FULL_DIMS.x/2, FULL_DIMS.y, FULL_DIMS.z])
    rotate([90, 0, 0])
    rotate([0, -90, 0])
    union() {
        translate([0,0,FULL_DIMS.x/2-(OW_THICC+RS)])
        difference() {
            crenellated_wall([FULL_DIMS.y, OW_THICC, OW_THICC+RS],
                [OW_THICC,CY,CY], [0,0,0], [OW_THICC,RS/3,RS/3], [OW_THICC,RS/3,RS/3]);
            translate([OW_THICC+SLOT_SIZE.y, -1, RS/2])
                cube([IW_THICC, OW_THICC+2, RS/2+1]);
        }
        translate([0,0,RS+IW_THICC])
            cube([FULL_DIMS.y, OW_THICC, SLOT_SIZE.x-2*RS]);
        difference() {
            crenellated_wall([FULL_DIMS.y, OW_THICC, RS+IW_THICC],
                [0,0,0], [IW_THICC,CY,CY], [OW_THICC,RS/3,RS/3], [OW_THICC,RS/3,RS/3]);
            translate([OW_THICC+SLOT_SIZE.y, -1, IW_THICC-1])
                cube([IW_THICC, OW_THICC+2, RS/2+1]);
        }
    }
}


module cover_wall() {
    color([0,1,1])
    cut_lever_hole(S, TLHD)
    cut_lever_hole(S, TLHD, in_depth=FULL_DIMS.y-OW_THICC)
    rotate_around([OW_THICC/2, OW_THICC/2, 0], [0, 0, 90])
    union() {
        translate([0, 0, FULL_DIMS.z-RS])
        difference() {
            crenellated_wall([FULL_DIMS.y, OW_THICC, RS+OW_THICC],
                [OW_THICC,CY,0], [0,0,0], [OW_THICC,RS/3,RS/3], [OW_THICC,RS/3,RS/3]);
            translate([OW_THICC+SLOT_SIZE.y, -1, -1])
                cube([IW_THICC, OW_THICC+2, RS/2+1]);
        }
        translate([0, 0, OW_THICC/2])
            cube([FULL_DIMS.y, OW_THICC, FULL_DIMS.z-RS-OW_THICC/2]);
        translate([0, OW_THICC/2, 0])
            cube([FULL_DIMS.y, OW_THICC/2, OW_THICC/2]);
        translate([0, OW_THICC/2, OW_THICC/2])
            rotate([0, 90, 0])
            cylinder(FULL_DIMS.y, OW_THICC/2, OW_THICC/2);
    }
}


module cover_sep() {
    color([0,0.8,0.8])
    translate([OW_THICC+SLOT_SIZE.x, 0, FULL_DIMS.z-RS])
    rotate_around([IW_THICC/2, IW_THICC/2, 0], [0, 0, 90])
    difference() {
        crenellated_wall([FULL_DIMS.y, IW_THICC, RS+OW_THICC], [OW_THICC,CY,0], [0,0,0], [OW_THICC,RS/3,RS/3], [OW_THICC,RS/3,RS/3]);
        translate([OW_THICC+SLOT_SIZE.x, -1, -1])
            cube([IW_THICC, IW_THICC+2, RS/2+1]);
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
        translate([0, OW_THICC, 0])
            rotate([90, 0, 0])
            linear_extrude(OW_THICC)
            polygon([[rs,0], [rs, rs], [0,rs]]);
        translate([RS, 0, rs])
            rotate([0,90,0])
            crenels(IW_THICC, rs/3, OW_THICC, 0, rs);
        translate([0, 0, rs])
            crenels(OW_THICC, rs/3, OW_THICC, 0, rs);
    }
}


module inner_central_reinf(rs) {
    color([0.8,0.8,0])
    translate([OW_THICC+SLOT_SIZE.x,OW_THICC+SLOT_SIZE.y,FULL_DIMS.z-RS])
    rotate_around([0, IW_THICC/2, IW_THICC/2], [90, 0, 0])
    linear_extrude(IW_THICC)
    polygon([[0, 0],
             [IW_THICC, 0],
             [IW_THICC, RS/2],
             [0, RS/2],
             [0, RS+OW_THICC],
             [-RS/2, RS+OW_THICC],
             [-RS/2, RS],
             [-RS, RS]
    ]);
}


module outer_central_reinf(rs) {
    color([0.8,0.8,0])
    translate([0,OW_THICC+SLOT_SIZE.y,FULL_DIMS.z-RS])
    rotate_around([0, IW_THICC/2, IW_THICC/2], [90, 0, 0])
    linear_extrude(IW_THICC)
    polygon([[0, 0],
             [OW_THICC, 0],
             [OW_THICC+RS, RS],
             [OW_THICC+RS/2, RS],
             [OW_THICC+RS/2, OW_THICC+RS],
             [RS-IW_THICC-RS/3, RS+OW_THICC],
             [RS-IW_THICC-RS/3, RS/2],
             [0, RS/2],
    ]);
}


module cover(EXPLODE) {
    translate([-EXPLODE, 0, 0])
        cover_wall();
    translate([0, -EXPLODE, 0])
        cut_lever_hole(D, TLHD)
        translate([0, 0, FULL_DIMS.z-RS]) {
            outer_reinforcement(RS);
            translate([OW_THICC+SLOT_SIZE.x-RS, 0, 0])
            inner_reinforcement(RS);
        }
    translate([0, EXPLODE, 0])
        translate([0, FULL_DIMS.y-OW_THICC, 0])
        cut_lever_hole(D, TLHD)
        translate([0, 0, FULL_DIMS.z-RS]) {
            outer_reinforcement(RS);
            translate([OW_THICC+SLOT_SIZE.x-RS, 0, 0])
            inner_reinforcement(RS);
        }
    translate([0, 0, EXPLODE])
        top_cover();
    translate([EXPLODE, 0, 0])
        cover_sep();
    outer_central_reinf(RS);
    inner_central_reinf(RS);
}

//--BASE--------------------------------------------------------

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
        polygon([[SLOT_SIZE.x, FULL_DIMS.z-RS],
                 [SLOT_SIZE.x-RS, FULL_DIMS.z],
                 [RS, FULL_DIMS.z],
                 [0, FULL_DIMS.z-RS],
        ]);
        crenellated_wall([SLOT_SIZE.x+IW_THICC, OW_THICC, FULL_DIMS.z-RS],
                [0,0,0],[OW_THICC,DX,0],[IW_THICC,DZ,DZ],[0,0,0]);
    }
}

module yu_wall() {
    color([0,0,1])
    rotate_around([IW_THICC/2, IW_THICC/2, 0], [0, 0, 90])
    difference() {
        crenellated_wall([FULL_DIMS.y, IW_THICC, FULL_DIMS.z-RS],
            [0,0,0], [OW_THICC,CY,0], [OW_THICC,DZ,0], [OW_THICC,DZ,0]);
        translate([OW_THICC+SLOT_SIZE.y, -1 ,OW_THICC+DZ])
            cube([IW_THICC, IW_THICC+2, DZ+1]);
    }
}


module xu_sep() {
    color([1,1,0])
    translate([OW_THICC, OW_THICC+SLOT_SIZE.y, 0])
    union() {
        rotate_around([0, IW_THICC/2, IW_THICC/2], [90, 0, 0])
        linear_extrude(IW_THICC)
        polygon([[SLOT_SIZE.x+IW_THICC, FULL_DIMS.z-RS],
                 [SLOT_SIZE.x, FULL_DIMS.z-RS],
                 [SLOT_SIZE.x-RS, FULL_DIMS.z],
                 [RS, FULL_DIMS.z],
                 [0, FULL_DIMS.z-RS],
                 [0, 0],
                 [BOTT_CUT, 0],
                 [BOTT_CUT, OW_THICC],
                 [SLOT_SIZE.x-BOTT_CUT, OW_THICC],
                 [SLOT_SIZE.x-BOTT_CUT, 0],
                 [SLOT_SIZE.x, 0],
                 [SLOT_SIZE.x, OW_THICC+DZ],
                 [SLOT_SIZE.x+IW_THICC, OW_THICC+DZ],
        ]);
    }
}


module upper_base(EXPLODE) {
    translate([EXPLODE, 0, 0])
        translate([OW_THICC+SLOT_SIZE.x, 0, 0])
        color([0,1,1])
        yu_wall();
    translate([0, -EXPLODE, 0])
        cut_lever_hole(UHP1)
        cut_lever_hole(UHP2)
        cut_lever_hole(R, TLHD)
        cut_lever_hole(T, TLHD)
        xu_wall();
    translate([0, EXPLODE, 0])
        mirror_from([0, FULL_DIMS.y/2, 0], [0,1,0])
        cut_lever_hole(UHP1)
        cut_lever_hole(UHP2)
        cut_lever_hole(R, TLHD)
        cut_lever_hole(T, TLHD)
        xu_wall();
    translate([0, 0, -EXPLODE/2])
        upper_deck();
    xu_sep();
}

//--LEVERS------------------------------------------------------

module one_side_levers(open, EXPLODE) {
    beta = UP_LEV_OFFSET + open*UP_LEV_RANGE;
    tll = len2D(D-T);
    lever(beta, T, tll, TLHD, [0.55,0.8,0], EXPLODE);
    mov_S = get_current_S(open);
    gamma = atan2((mov_S-R).y, (mov_S-R).x);
    bll = len2D(S-R);
    lever(gamma, R, bll, TLHD, [0.55,0.8,0.7], EXPLODE);
}


module levers(open, EXPLODE) {
    one_side_levers(open, EXPLODE);
    mirror_from(FULL_DIMS/2, [0,1,0])
        one_side_levers(open, EXPLODE);
}

//--MAIN-PART---------------------------------------------------

module upper_part(open, EXPLODE) {
    mov_D = get_current_D(open);
    mov_S = get_current_S(open);
    cov_angle_0 = atan2((D-S).y, (D-S).x);
    cov_angle_curr = atan2((mov_D-mov_S).y, (mov_D-mov_S).x);
    cov_angle = cov_angle_curr - cov_angle_0;

    upper_base(EXPLODE);
    translate([-EXPLODE, 0, EXPLODE])
        translate([mov_S.x-S.x, 0, mov_S.y-S.y])
        rotate_around([OW_THICC/2, 0, OW_THICC/2], [0, -cov_angle, 0])
        cover(EXPLODE);
    levers(open, EXPLODE);
}


TOP_OPEN = min(max($t*1.2-0.1, 0), 1);
EXPLODE = 0;

math_visualisation(TOP_OPEN);
upper_part(TOP_OPEN, EXPLODE);
