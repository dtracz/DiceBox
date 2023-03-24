#include <iostream>
#include "core/Component2D.h"
#include "components.h"
#include "base.h"
#include "toolkit.h"



int main() {
    FlatPart fp {Circle::create(4)};
    fp + Circle::create(3).translate(2,1,0)
       - Circle::create(2)
       + Square::create(1, 10);

    auto fp2 = fp;

    fp.translate({1,0,3});
    fp.translate({0,2,0});
    fp.set_thickness(3);

    fp2.translate({0,0,-2});
    fp2.set_thickness(2);


    FlatPart fp3 {crenels({3,2}, 13, -1)};
    fp3 + crenels({3,2}, 13, 2).translate(0, -2, 0);

    FlatPart fp4 {crenellated_wall(
            {20, 10}, {4,2,-2},  {4,2,-2},  {2,2,0},  {2,2,0}
    )};
    fp4.set_thickness(2);
    fp4.translate({0, -15, 0});
    auto fp5 = fp4;
    fp4.rotate({0,0,90});
    fp4.rotate({0,90,0});

    Color c = {1,0,1};
    c += {0,1,0};

    auto fp6 = fp4;
    fp6.mirror({0,1,0}, {0,-1,0});

    fp4.set_color({1,0,0});
    fp5.set_color({0,1,0});
    fp6.set_color({0,0,1});

    HelperPart hp0 = Cube::create(2,3,4);

    IndentWriter writer;
    // fp.render3D(writer);
    // fp2.render3D(writer);
    // fp3.render2D(writer);
    fp4.render3D(writer);
    fp5.render3D(writer);
    fp6.render3D(writer);
    hp0.render3D(writer);
    std::cout << writer;

    return 0;
}

