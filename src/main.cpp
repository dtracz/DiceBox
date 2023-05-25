#include "base/parts.hpp"
#include "components/box.h"
#include "core/IndentWriter.h"
#include "utils/color_generators.h"
#include <iostream>

int main()
{
    DistinguishableColorGenerator gen;
    auto box = box::get(0.7, 0.6);

    IndentWriter writer;
    box.render3D(writer);
    std::cout << writer << std::flush;
    return 0;
}
