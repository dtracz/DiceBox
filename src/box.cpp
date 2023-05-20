#include <cmath>
#include <numbers>
#include "box.h"
#include "base.h"
#include "toolkit.h"
#include "globals.h"
#include "lower_part.h"
#include "upper_part.h"
#include "levers.h"


namespace box {

Module3D get_levers(
        const LinearLeverCalculator& calc, double open,
        double width, double thicc, double pin_r,
        const IColorGenerator& colors
) {
    double pin_length = OW_THICC;
    auto get_lev = [&](Vec2 pos) {
        auto lever = levers::simple(
                calc.lever_length(), thicc, width/2,
                pin_r, pin_length, colors
        );
        lever.rotate({0, 0, -calc.get_lever_angle(open)});
        lever.translate(Vec3::fromXY(pos));
        lever.rotate({-90, 0, 0});
        return lever;
    };
    auto lever_set1 = get_lev(calc.lever1_position())
                    + get_lev(calc.lever2_position());
    auto lever_set2 = lever_set1;
    lever_set2.mirror({0, 1, 0}, FULL_DIMS/2);
    auto lever_set3 = lever_set1;
    lever_set3.mirror({1, 0, 0}, FULL_DIMS/2);
    auto lever_set4 = lever_set1;
    lever_set4.mirror({0, 1, 0}, FULL_DIMS/2);
    lever_set4.mirror({1, 0, 0}, FULL_DIMS/2);
    return lever_set1 + lever_set2 + lever_set3 + lever_set4;
}


Module3D get(double open, double cover_open) {
    DistinguishableColorGenerator gen;
    auto lp = LowerPart::get(gen);
    gen.set_offset(6);
    LinearLeverCalculator calc { LHP, UHP, SHIFT_TO_2ND };
    auto up1 = UpperPart::get(gen, cover_open);
    up1.translate({0, 0, FULL_DIMS.z});
    up1.translate(Vec3::fromXZ(calc.cover_position(open)));
    auto up2 = up1;
    up2.mirror({1,0,0}, FULL_DIMS/2);
    auto levers = get_levers(calc, open, OW_THICC, 4, 1, gen);
    return Module3D::Union(
            lp,
            up1,
            up2,
            levers
    );
}

}  // namespace box

