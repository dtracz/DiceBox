#ifndef SIMPLE_PARSER_H_INCLUDED
#define SIMPLE_PARSER_H_INCLUDED

#include "parser/Parseable.h"
#include "parser/operations.h"
#include "parser/shapes.h"
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace parser {

class SimpleParser {
  public:
    SimpleParser()
        // : _current_child { nullptr }
        : _main_shape { nullptr }
        , _shape_stack {}
        , _last_indent {}
    { }

    ~SimpleParser() = default;

    void parse_line(const std::string& line);

    // AbstractParser& operator<<(const std::string&);

    std::shared_ptr<Parseable> get_parsed()
    {
        if (_shape_stack.empty() && _operation_stack.empty()
            && _main_shape.get() != nullptr)
            return _main_shape;
        throw std::runtime_error("bad parse");
    }

    // void add(std::shared_ptr<Shape>);
    // void add(std::shared_ptr<Operation>) { }

  private:
    enum _LastIndentT { tShape, tOperation, tOther };

    // std::unique_ptr<AbstractParser> _current_child;

    std::shared_ptr<Shape> _main_shape;
    std::vector<std::shared_ptr<Operation>> _operation_stack;
    std::stack<std::shared_ptr<ShapeContainer>> _shape_stack;
    std::stack<_LastIndentT> _last_indent;
}; // class SimpleParser


} // namespace parser

#endif // SIMPLE_PARSER_H_INCLUDED
