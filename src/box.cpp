#include <iostream>
#include "core/IndentWriter.h"
#include "base.h"
#include "toolkit.h"
#include "globals.h"
#include "lower_part.h"
#include "upper_part.h"


Module3D get_box() {
    DistinguishableColorGenerator gen;
    auto lp = LowerPart::get(gen);
    gen.set_offset(6);
    auto up1 = UpperPart::get(gen);
    up1.translate({-FULL_DIMS.x/2, 0, FULL_DIMS.z});
    auto up2 = up1;
    up2.mirror({1,0,0}, {FULL_DIMS.x/2, 0, 0});
    return Module3D::Union(
            lp,
            up1,
            up2
    );
}


int main() {
    auto box = get_box();

    IndentWriter writer;
    box.render3D(writer);
    std::cout << writer << std::flush;
    return 0;
}

