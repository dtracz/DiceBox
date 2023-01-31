
module crenels(height, width, depth, offset, length) {
    for(i = [offset: 2*width : length]) {
        if (i + width <= length)
            translate([i, 0, 0])
                cube([width, depth, height]);
    }
}

module crenellated_wall(dims,
                        crn_t = [0,0,0], // [height, width, offset
                        crn_r = [0,0,0],
                        crn_b = [0,0,0],
                        crn_l = [0,0,0]) {
    union() {
        translate([crn_b.x, 0, crn_l.x])
            cube([
                dims.x - crn_l.x - crn_r.x,
                dims.y,
                dims.z - crn_t.x - crn_b.x,
            ]);
        if (crn_t.x > 0) {
            translate([crn_l.x, 0, dims.z - crn_t.x])
                crenels(crn_t.x, crn_t.y, dims.y, crn_t.z, dims.x-crn_l.x);
        }
        if (crn_r.x > 0) {
            translate([dims.x - crn_r.x, 0, dims.z - crn_t.x])
                rotate([0,90,0])
                crenels(crn_r.x, crn_r.y, dims.y, crn_r.z, dims.z-crn_t.x);
        }
        if (crn_b.x > 0) {
            translate([dims.x - crn_r.x, 0, crn_b.x])
                rotate([0,180,0])
                crenels(crn_b.x, crn_b.y, dims.y, crn_b.z, dims.x-crn_r.x);
        }
        if (crn_l.x > 0) {
            translate([crn_l.x, 0, crn_b.x])
                rotate([0,270,0])
                crenels(crn_l.x, crn_l.y, dims.y, crn_l.z, dims.z-crn_b.x);
        }
    }
}


module rotate_around(pt, rotation) {
    translate(pt)
    rotate(rotation)
    translate(-pt)
    children();
}

color([1,0,0])
    crenellated_wall([19, 1, 13], [0,0,0], [1,2,0], [1,2,0], [1,2,0]);
color([0,1,0])
    rotate_around([1/2, 1/2, 10/2], [0, 0, 90])
    rotate_around([19/2, 1/2, 10/2], [0, 0, 180])
    crenellated_wall([19, 1, 13], [0,0,0], [1,2,0], [1,2,0], [1,2,0]);
color([0,0,1])
    rotate_around([19-1/2, 1/2, 10/2], [0, 0, -90])
    rotate_around([19/2, 1/2, 10/2], [0, 0, 180])
    crenellated_wall([19, 1, 13], [0,0,0], [1,2,0], [1,2,0], [1,2,0]);
color([1,1,0])
    translate([0, 19-1, 0])
    rotate_around([19/2, 1/2, 10/2], [0, 0, 180])
    crenellated_wall([19, 1, 13], [0,0,0], [1,2,0], [1,2,0], [1,2,0]);

color([1,1,1])
    rotate_around([19/2, 1/2, 1/2], [-90, 0, 0])
    crenellated_wall([19, 1, 19], [1,2,2], [1,2,2], [1,2,2], [1,2,2]);

