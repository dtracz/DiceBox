#ifndef SCAD_2D_PARSER_H_INCLUDED
#define SCAD_2D_PARSER_H_INCLUDED

#include "core/IndentWriter.h"
#include "parser/LinearAutoma.hpp"
#include "parser/operations.h"
#include "parser/shapes.h"
#include <cctype>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <vector>


namespace parser {

class AbstractParser {
  public:
    virtual AbstractParser& operator<<(const std::string&) = 0;

    virtual std::shared_ptr<Shape> get_parsed() = 0;
};


class Scad2DParser : public AbstractParser {
  public:
    Scad2DParser()
        : _current_child { nullptr }
        , _main_shape { nullptr }
        , _shape_stack { { std::make_shared<Union>() } }
        , _last_indent { { tShape } }
    { }

    ~Scad2DParser() = default;

    AbstractParser& operator<<(const std::string&);

    std::shared_ptr<Shape> get_parsed();

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


template <class ShapeT, typename... Ts>
class ShapeParser : public AbstractParser {
  public:
    ShapeParser(Scad2DParser* parser_ptr, const LinearAutoma<Ts...>& automa)
        : _parent { parser_ptr }
        , _automa { automa }
    { }

    AbstractParser& operator<<(const std::string& inp)
    {
        _automa << inp;
        if (_automa.at_endstate())
            return *_parent;
        return *this;
    }

    std::shared_ptr<Shape> get_parsed()
    {
        ShapeT shape = std::make_from_tuple<ShapeT>(_automa.get_result());
        return std::make_shared<ShapeT>(shape);
    }

  private:
    Scad2DParser* _parent;
    LinearAutoma<Ts...> _automa;
}; // class RectangleParser


} // namespace packer

#endif // SCAD_2D_PARSER_H_INCLUDED
