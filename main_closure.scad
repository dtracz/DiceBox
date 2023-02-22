include <globals.scad>
use <toolkit.scad>



module cut_lever_hole(position) {
    difference() {
        children();
        translate([position.x, -1, position.y])
            rotate([-90, 0, 0])
            cylinder(OW_THICC+2, HOLE_D/2, HOLE_D/2);
    }
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


module lever(alpha, lhp, c, explode) {
    color(c)
        translate([lhp.x, -explode*3/2, lhp.y])
        rotate([0, -alpha, 0])
        translate([0, -LEV_THICC.y-SEP, 0])
        lever_bar(LEV_LGH);
    translate([0, -explode*5/2, 0]) {
        translate([lhp.x, 0, lhp.y])
            lever_plug();
        translate([lhp.x+LEV_LGH*cos(alpha), 0, lhp.y+LEV_LGH*sin(alpha)])
            lever_plug();
    }
}


function get_curr_alpha(main_open) =
    ALPHA + 2*main_open*(90-ALPHA);


module closure_levers(main_open, EXPLODE) {
    alpha = get_curr_alpha(main_open);
    c1 = [1, 0.5, 0];
    c2 = [1, 0.7, 0];
    lever(alpha, LHP1, c1, EXPLODE);
    lever(alpha, LHP2, c2, EXPLODE);
    mirror_from(FULL_DIMS/2, [0,1,0]) {
        lever(alpha, LHP1, c1, EXPLODE);
        lever(alpha, LHP2, c2, EXPLODE);
    }
    mirror_from(FULL_DIMS/2, [1,0,0]) {
        lever(alpha, LHP1, c1, EXPLODE);
        lever(alpha, LHP2, c2, EXPLODE);
        mirror_from(FULL_DIMS/2, [0,1,0]) {
            lever(alpha, LHP1, c1, EXPLODE);
            lever(alpha, LHP2, c2, EXPLODE);
        }
    }
}


module open(main_open) {
    alpha = get_curr_alpha(main_open);
    pos0 = [LEV_LGH*cos(ALPHA), 0, LEV_LGH*sin(ALPHA)];
    curr_pos = [LEV_LGH*cos(alpha), 0, LEV_LGH*sin(alpha)];
    translate(curr_pos-pos0)
        children();
}


EXPLODE = 0;
MAIN_OPEN = 1;

closure_levers(MAIN_OPEN, EXPLODE);
