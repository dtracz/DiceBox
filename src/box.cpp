#include <iostream>
#include "core/Component2D.h"
#include "components.h"
#include "base.h"



int main() {
    FlatPart fp{Circle::create(4)};
    fp + Circle::create(3).translate(2,1,0)
       - Circle::create(2)
       + Square::create(1, 10);

    auto fp2 = fp;

    fp.translate({1,0,3});
    fp.translate({0,2,0});
    fp.set_thickness(3);

    fp2.translate({0,0,-2});
    fp2.set_thickness(2);

    IndentWriter writer;
    fp.render3D(writer);
    fp2.render3D(writer);
    std::cout << writer;

    return 0;
}

