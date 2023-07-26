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

    AbstractParser& operator<<(const std::string&) override
    {
        if (0 < --_inps_to_skip)
            return *this;
        return *_parent;
    }

    std::shared_ptr<Parseable> get_parsed() override
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


template <class Parseable, typename... Ts>
std::unique_ptr<ShapeParser<Parseable, Ts...>>
get_parser_ptr(AbstractParser* parent, const std::string& regexes)
{
    auto regex_vector = split_by(regexes);
    auto automa = LinearAutoma<Ts...> { regex_vector };
    return std::make_unique<ShapeParser<Parseable, Ts...>>(parent, automa);
}


AbstractParser& Scad2DParser::operator<<(const std::string& str)
{
    if (str.starts_with(";")) {
        auto shp = _current_child->get_parsed();
        add(std::dynamic_pointer_cast<Shape>(shp));
        _current_child.release();
        return *this;
    } else if (str.starts_with("{")) {
        switch (_last_indent.top()) {
        case tShape: {
            auto shp = _current_child->get_parsed();
            _shape_stack.push(std::dynamic_pointer_cast<ShapeContainer>(shp));
            break;
        }
        case tOperation: {
            auto o = _current_child->get_parsed();
            _operation_stack.push_back(std::dynamic_pointer_cast<Operation>(o));
            break;
        }
        case tOther: {
            break;
        }
        default:
            throw std::runtime_error("Bad last indent type");
        }
    } else if (str.starts_with("}")) {
        switch (_last_indent.top()) {
        case tShape: {
            auto shape_container_ptr = _shape_stack.top();
            _shape_stack.pop();
            add(shape_container_ptr);
            break;
        }
        case tOperation: {
            _operation_stack.pop_back();
            break;
        }
        case tOther:
            break;
        default:
            throw std::runtime_error("Bad last indent type");
        }
        _last_indent.pop();
        _current_child = std::make_unique<NullBuffer>(this, 3);
        return *_current_child;
    } else if (str.starts_with("union")) {
        _current_child = get_parser_ptr<Union>(this, "\\( \\)");
        _last_indent.push(tShape);
        return *_current_child;
    } else if (str.starts_with("difference")) {
        _current_child = get_parser_ptr<Difference>(this, "\\( \\)");
        _last_indent.push(tShape);
        return *_current_child;
    } else if (str.starts_with("translate")) {
        _current_child = get_parser_ptr<Translate, double, double, double>(
            this,
            "\\( v = \\[ -?\\d+(\\.\\d+)? , \\d+(\\.\\d+)? , \\d+(\\.\\d+)? "
            "\\] \\)"
        );
        _last_indent.push(tOperation);
        return *_current_child;
    } else if (str.starts_with("rotate")) {
        _current_child = get_parser_ptr<Rotate, double, double, double>(
            this,
            "\\( \\[ -?\\d+(\\.\\d+)? , -?\\d+(\\.\\d+)? , -?\\d+(\\.\\d+)? "
            "\\] \\)"
        );
        _last_indent.push(tOperation);
        return *_current_child;
    } else if (str.starts_with("mirror")) {
        _current_child = get_parser_ptr<Mirror, double, double, double>(
            this,
            "\\( v = \\[ -?\\d+(\\.\\d+)? , \\d+(\\.\\d+)? , \\d+(\\.\\d+)? "
            "\\] \\)"
        );
        _last_indent.push(tOperation);
        return *_current_child;
    } else if (str.starts_with("square")) {
        _current_child = get_parser_ptr<Rectangle, double, double, bool>(
            this,
            "\\( size = \\[ \\d+(\\.\\d+)? , \\d+(\\.\\d+)? \\] , center = "
            "(true)|(false) \\)"
        );
        return *_current_child;
    } else if (str.starts_with("circle")) {
        _current_child = get_parser_ptr<Circle, double>(
            this, "\\( r = \\d+(\\.\\d+)? , \\$fn = \\d+ \\)"
        );
        return *_current_child;
    } else if (str.starts_with("polygon")) {
        // TODO
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
        _shape_stack.top()->children.push_back(shape);
}
