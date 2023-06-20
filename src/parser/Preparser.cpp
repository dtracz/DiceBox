#include "parser/Preparser.h"
#include "core/IndentWriter.h"
#include "parser/scad_2D_parser.hpp"
#include <set>
#include <sstream>
#include <string>


using namespace parser;


Preparser& Preparser::operator>>(AbstractParser& parser)
{
    AbstractParser* parser_ptr = &parser;
    std::string buff;
    while (_load_next(buff)) {
        auto& new_parser = *parser_ptr << buff;
        parser_ptr = &new_parser;
    }
    return *this;
}

int Preparser::_load_next(std::string& str)
{
    while (_awaiting == '\0' || std::isspace(_awaiting))
        _awaiting = _buff.get();
    if (_awaiting == std::char_traits<char>::eof())
        return 0;
    std::stringstream buff;
    bool done = _delims.contains(_awaiting);
    do {
        buff << _awaiting;
        _awaiting = _buff.get();
        done += (std::isspace(_awaiting) || _delims.contains(_awaiting));
    } while (!done);

    str = buff.str();
    return str.length();
}
