include <globals.scad>
use <toolkit.scad>
use <main_closure.scad>
use <lower_part.scad>
//use <hinged_upper_part.scad>
use <levered_upper_part.scad>



module box(top_open, main_open, EXPLODE, math_vis=false) {
    lower_part(EXPLODE);
    open(main_open)
        translate([-2*EXPLODE, 0, 3*EXPLODE])
        translate([0,0,FULL_DIMS.z])
        upper_part(top_open, EXPLODE, math_vis);
    mirror_from(FULL_DIMS/2, [1, 0, 0])
        translate([-2*EXPLODE, 0, 3*EXPLODE])
        open(main_open)
        translate([0,0,FULL_DIMS.z])
        upper_part(top_open, EXPLODE, math_vis);
    closure_levers(main_open, EXPLODE);
}


MAIN_OPEN = min(max($t*1.2-0.1, 0), 1);
TOP_OPEN = min(max($t*1.2-0.1, 0), 1);
EXPLODE = 0;

box(TOP_OPEN, MAIN_OPEN, EXPLODE);
