#include <iostream>
#include "core/IndentWriter.h"
#include "box.h"
#include "base.h"
#include "toolkit.h"

int main() {
    DistinguishableColorGenerator gen;
    // auto box = UpperPart::get(gen, 0.5);
    auto box = box::get(1, 1);

    IndentWriter writer;
    box.render3D(writer);
    std::cout << writer << std::flush;
    return 0;
}

