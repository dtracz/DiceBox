#ifndef SCAD_2D_PARSER_HPP_INCLUDED
#define SCAD_2D_PARSER_HPP_INCLUDED

#include "core/IndentWriter.h"
#include "parser/LinearAutoma.hpp"
#include "parser/Parseable.h"
#include "parser/operations.h"
#include "parser/shapes.h"
#include <cctype>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>


namespace parser {

class AbstractParser {
  public:
    virtual AbstractParser& operator<<(const std::string&) = 0;

    virtual std::shared_ptr<Parseable> get_parsed() = 0;
};

class Scad2DParser : public AbstractParser,
                     public std::enable_shared_from_this<AbstractParser> {
  public:
    Scad2DParser()
        : _current_child { nullptr }
        , _main_shape { nullptr }
        , _shape_stack {}
        , _last_indent {}
    { }

    ~Scad2DParser() = default;

    AbstractParser& operator<<(const std::string&);

    std::shared_ptr<Parseable> get_parsed()
    {
        if (_shape_stack.empty() && _operation_stack.empty()
            && _main_shape.get() != nullptr)
            return _main_shape;
        throw std::runtime_error("bad parse");
    }

    void add(std::shared_ptr<Shape>);
    void add(std::shared_ptr<Operation>) { }

  private:
    enum _LastIndentT { tShape, tOperation, tOther };

    std::unique_ptr<AbstractParser> _current_child;

    std::shared_ptr<Shape> _main_shape;
    std::vector<std::shared_ptr<Operation>> _operation_stack;
    std::stack<std::shared_ptr<ShapeContainer>> _shape_stack;
    std::stack<_LastIndentT> _last_indent;
}; // class Scad2DParser


template <class ParsedT, typename... Ts>
class ShapeParser : public AbstractParser {
  public:
    ShapeParser(std::shared_ptr<AbstractParser> parser_ptr, std::shared_ptr<IAutoma<Ts...>> automa_ptr)
        : _parent { std::move(parser_ptr) }
        , _automa_ptr { automa_ptr }
    { }

    AbstractParser& operator<<(const std::string& inp)  override
    {
        *_automa_ptr << inp;
        if (_automa_ptr->at_endstate())
            return *_parent;
        return *this;
    }

    std::shared_ptr<Parseable> get_parsed() override
    {
        ParsedT shape = std::make_from_tuple<ParsedT>(_automa_ptr->get_result());
        return std::make_shared<ParsedT>(shape);
    }

  private:
    std::shared_ptr<AbstractParser> _parent;
    std::shared_ptr<IAutoma<Ts...>> _automa_ptr;
}; // class RectangleParser


} // namespace packer

#endif // SCAD_2D_PARSER_HPP_INCLUDED
