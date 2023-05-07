#include <iostream>
#include <cmath>
#include <numbers>
#include "core/IndentWriter.h"
#include "base.h"
#include "toolkit.h"
#include "globals.h"
#include "lower_part.h"
#include "upper_part.h"
#include "levers.h"


Module3D get_levers(
        std::pair<double, double> lower_mp,
        std::pair<double, double> upper_mp,
        double shift_to_2nd, double open,
        double width, double thicc, double pin_r,
        const IColorGenerator& colors
) {
    double length = std::sqrt(
            sqr(upper_mp.first - lower_mp.first) +
            sqr(upper_mp.second - lower_mp.second)
    );
    double angle0 = angle2D(
            std::abs(upper_mp.first - lower_mp.first),
            std::abs(upper_mp.second - lower_mp.second)
    );
    double angle1 = std::numbers::pi - angle0;
    double angle = angle0 + open*(angle1 - angle0);
    double pin_length = OW_THICC;
    auto lever1 = levers::simple(
            length, thicc, width/2,
            pin_r, pin_length, colors
    );
    lever1.rotate({0, 0, -angle * 180 / std::numbers::pi});
    lever1.translate({lower_mp.first, lower_mp.second, 0});
    lever1.rotate({-90, 0, 0});
    auto lever2 = lever1;
    lever2.mirror({0, 1, 0}, FULL_DIMS/2);
    auto lever3 = lever1;
    lever3.translate({shift_to_2nd, 0, 0});
    auto lever4 = lever3;
    lever4.mirror({0, 1, 0}, FULL_DIMS/2);
    auto lever_set1 = lever1 + lever2 + lever3 + lever4;
    auto lever_set2 = lever_set1;
    lever_set2.mirror({1, 0, 0}, FULL_DIMS/2);
    return lever_set1 + lever_set2;
}


Module3D get_box(double open, double cover_open) {
    DistinguishableColorGenerator gen;
    auto lp = LowerPart::get(gen);
    gen.set_offset(6);
    auto up1 = UpperPart::get(gen, cover_open);
    up1.translate({-open*OPEN_SIZE, 0, FULL_DIMS.z});
    auto up2 = up1;
    up2.mirror({1,0,0}, FULL_DIMS/2);
    auto levers = get_levers(
            LHP, UHP,
            SHIFT_TO_2ND, open, OW_THICC, 4, 1,
            gen
    );
    return Module3D::Union(
            lp,
            up1,
            up2,
            levers
    );
}


int main() {
    DistinguishableColorGenerator gen;
    // auto box = UpperPart::get(gen, 0.5);
    auto box = get_box(1, 1);

    IndentWriter writer;
    box.render3D(writer);
    std::cout << writer << std::flush;
    return 0;
}

