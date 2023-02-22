include <globals.scad>
use <toolkit.scad>
use <main_closure.scad>
use <lower_part.scad>
use <upper_part.scad>



module box() {
    lower_part();
    open()
        translate([0,0,FULL_DIMS.z])
        upper_part();
    rotate_around(FULL_DIMS/2, [0, 0, 180])
        open()
        translate([0,0,FULL_DIMS.z])
        upper_part();
    closure_levers();
}


$fn=100;

$OPEN = 1;
$TOP_OPEN = 90; // in degrees
$EXPLODE = 0;


box();
