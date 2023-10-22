#include "base/parts.hpp"
#include "components/box.h"
#include "core/IndentWriter.h"
#include "cxxopts.hpp"
#include "packer/NaivePacker.h"
#include "utils/color_generators.h"
#include <cstdio>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>


cxxopts::ParseResult parse_args(int argc, char** argv)
{
    cxxopts::Options options("test", "A brief description");
    options.add_options()
        ("o,output",
         "Redirect main output to file specified by `arg`.",
          cxxopts::value<std::string>()->default_value("stdout"))
        ("sheet-dims", "Decompose project into flat sheets with dimensions "
         "specified by `arg`. Format should be \"\\d+,\\d+\" [mm].",
         cxxopts::value<std::string>())
        ("sheet-output", "Template name of file to store decomposed output. "
         "Sheets will be saved as \"`arg`_\\d+_\\d+mm.scad\" files, "
         "with the first number being the index and the second specifying "
         "thickness of given sheet. Requires: \"--sheet-dims\".",
         cxxopts::value<std::string>()->default_value("stdout"))
        ("h,help", "Print this help")
    ;
    cxxopts::ParseResult result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    if (result.count("sheet-output") > 0 && result.count("sheet-dims") == 0)
        throw std::invalid_argument(
            "'--sheet-output' option requires '--sheet-dims' option"
        );
    return result;
}

Vec2 parse_sheet_dims(const std::string& arg)
{
    Vec2 dims;
    std::sscanf(arg.c_str(), "%lf,%lf", &dims.x, &dims.y);
    return dims;
}


template <typename T>
void print_to(std::string output, T part, std::string name = "")
{
    if (name != "")
        name = "\n// " + name + "\n\n";
    IndentWriter writer;
    part.render(writer);
    if (output == "stdout") {
        std::cout << name << writer;
    } else if (output == "stdout") {
        std::cerr << name << writer;
    } else {
        if (!output.ends_with(".scad"))
            output.append(".scad");
        std::ofstream out_file;
        out_file.open(output);
        out_file << name << writer;
        out_file.close();
    }
}


int main(int argc, char** argv)
{
    auto args = parse_args(argc, argv);

    DistinguishableColorGenerator gen;
    Module3D box = box::get(0.7, 0.6);
    print_to(args["output"].as<std::string>(), box, "Box");

    if (args.count("sheet-dims")) {
        Vec2 dims = parse_sheet_dims(args["sheet-dims"].as<std::string>());
        packer::NaivePacker packer { dims };
        packer.add_part(box);
        auto sheets = packer.pack();
        std::string name_fmt { "Sheet_{}mm_{}_{}x{}mm" };
        std::string file_fmt { args["sheet-output"].as<std::string>() };
        if (file_fmt != "stdout" && file_fmt != "stderr")
            file_fmt += "_{}";
        for (int i = 0; i < sheets.size(); i++) {
            auto name = std::vformat(
                name_fmt,
                std::make_format_args(sheets[i].thickness, i, dims.x, dims.y)
            );
            auto file = std::vformat(file_fmt, std::make_format_args(i));
            print_to(file, sheets[i], name);
        }
    }

    return 0;
}
