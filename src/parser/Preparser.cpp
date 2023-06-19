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
    if (_awaiting == std::char_traits<char>::eof())
        return 0;
    if (_awaiting == '\0')
        _awaiting = _next_char();
    std::stringstream buff;
    bool done = false;
    while (!done) {
        buff << _awaiting;
        done = _awaiting != '\0' && _delims.contains(_awaiting);
        _awaiting = _next_char();
        done += _delims.contains(_awaiting);
    }
    str = buff.str();
    return str.length();
}
