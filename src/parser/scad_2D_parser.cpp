#include "parser/scad_2D_parser.hpp"
#include <regex>
#include <vector>

using namespace parser;


class NullBuffer : public AbstractParser {
  public:
    NullBuffer(Scad2DParser* parent, size_t inps_to_skip)
        : _parent { parent }
        , _inps_to_skip { inps_to_skip }
    { }

    AbstractParser& operator<<(const std::string&)
    {
        if (0 < _inps_to_skip--)
            return *this;
        return *_parent;
    }

    virtual std::shared_ptr<Shape> get_parsed()
    {
        return nullptr;
    }

  private:
    Scad2DParser* _parent;
    size_t _inps_to_skip;
}; // NullBuffer


std::vector<std::string>
split_by(std::string str, const std::string& delim = " ")
{
    auto next = [&delim](std::string& str) -> std::string {
        size_t pos = str.find(delim);
        if (pos == std::string::npos)
            pos = str.length();
        auto prefix = str.substr(0, pos);
        str = str.substr(std::min(pos + 1, str.length()), str.length());
        return prefix;
    };
    std::vector<std::string> splitted;
    while (str.length())
        splitted.push_back(next(str));
    return splitted;
}


AbstractParser& Scad2DParser::operator<<(const std::string& str)
{
    if (str.starts_with(";")) {
        add(_current_child->get_parsed());
        _current_child.release();
        return *this;
    } else if (str.starts_with("}")) {
        auto shape_container_ptr = _shape_stack.top();
        _shape_stack.pop();
        add(shape_container_ptr);
        _current_child = std::make_unique<NullBuffer>(this, 3);
        return *(_current_child.get());
    } else if (str.starts_with("union")) {
        auto automa = LinearAutoma<> { { "\\(", "\\)", "\\{" } };
        _current_child = std::make_unique<ShapeParser<Union>>(this, automa);
        return *(_current_child.get());
    } else if (str.starts_with("difference")) {
    } else if (str.starts_with("translate")) {
    } else if (str.starts_with("rotate")) {
    } else if (str.starts_with("mirror")) {
    } else if (str.starts_with("square")) {
        auto rgxs = split_by(
            "\\( size = \\[ \\d+(\\.\\d+)? , \\d+(\\.\\d+)? \\] , center = "
            "(true)|(false) \\)"
        );
        auto automa = LinearAutoma<double, double, bool> { rgxs };
        _current_child
            = std::make_unique<ShapeParser<Rectangle, double, double, bool>>(
                this, automa
            );
        return *(_current_child.get());
    } else if (str.starts_with("circle")) {
    } else if (str.starts_with("polygon")) {
    } else {
        throw std::runtime_error("Unknown symbol: <" + str + ">");
    }
    return *this;
}

void Scad2DParser::add(std::shared_ptr<Shape> shape)
{
    for (auto back_it = _operation_stack.rbegin();
         back_it != _operation_stack.rend();
         back_it++) {
        auto& operation_ptr = *back_it;
        operation_ptr->apply(shape);
    }
    if (_shape_stack.empty())
        _main_shape = shape;
    else
        _shape_stack.top()->children.push_back(std::move(shape));
}

