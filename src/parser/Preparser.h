#ifndef PREPARSER_H_INCLUDED
#define PREPARSER_H_INCLUDED

#include "core/IndentWriter.h"
#include "parser/scad_2D_parser.hpp"
#include <set>
#include <sstream>
#include <string>

namespace parser {

class Preparser {
  public:
    inline Preparser& operator<<(IndentWriter& writer)
    {
        _buff << writer;
        return *this;
    }

    Preparser& operator>>(AbstractParser& parser);

  private:
    int _load_next(std::string& str);

    std::stringstream _buff;
    char _awaiting = '\0';
    const std::set<char> _delims = { '(', ')', '[', ']', '{', '}',
                                     ',', ';', '=', EOF, '\0' };
}; // class Preparser

} // namespace parser

#endif // PREPARSER_H_INCLUDED
