#include "parser/scad_2D_parser.hpp"
#include <regex>
#include <stdexcept>
#include <vector>

using namespace parser;


class NullBuffer : public AbstractParser {
  public:
    NullBuffer(std::shared_ptr<AbstractParser> parent_ptr, int inps_to_skip)
        : _parent_ptr { std::move(parent_ptr) }
        , _inps_to_skip { inps_to_skip }
    {
        if (inps_to_skip < 0)
            throw std::invalid_argument("Cannot skip negative number of inputs"
            );
    }

    NullBuffer(
        std::shared_ptr<AbstractParser> parent_ptr,
        const std::string& skip_until
    )
        : _parent_ptr { parent_ptr }
        , _inps_to_skip { -1 }
        , _border { skip_until }
    { }

    AbstractParser& operator<<(const std::string& inp) override
    {
        if (_inps_to_skip < 0) {
            if (std::regex_match(inp, _border))
                return *_parent_ptr;
            return *this;
        }
        if (0 < --_inps_to_skip)
            return *this;
        return *_parent_ptr;
    }

    std::shared_ptr<Parseable> get_parsed() override
    {
        return _parent_ptr->get_parsed();
    }

  private:
    std::shared_ptr<AbstractParser> _parent_ptr;
    int _inps_to_skip;
    std::regex _border;
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


template <class ParseableT, typename... Ts>
std::unique_ptr<ShapeParser<ParseableT, Ts...>> get_parser_ptr(
    std::shared_ptr<AbstractParser> parent, const std::string& regexes
)
{
    auto regex_vector = split_by(regexes);
    std::shared_ptr<IAutoma<Ts...>> automa_ptr
        = std::make_shared<LinearAutoma<Ts...>>(regex_vector);
    return std::make_unique<ShapeParser<ParseableT, Ts...>>(
        std::move(parent), automa_ptr
    );
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
        _current_child = std::make_unique<NullBuffer>(shared_from_this(), 3);
        return *_current_child;
    } else if (str.starts_with("union")) {
        _current_child = get_parser_ptr<Union>(shared_from_this(), "\\( \\)");
        _last_indent.push(tShape);
        return *_current_child;
    } else if (str.starts_with("difference")) {
        _current_child = get_parser_ptr<Difference>(
            shared_from_this(), "\\( \\)"
        );
        _last_indent.push(tShape);
        return *_current_child;
    } else if (str.starts_with("translate")) {
        _current_child = get_parser_ptr<Translate, double, double, double>(
            shared_from_this(),
            "\\( v = \\[ -?\\d+(\\.\\d+)? , -?\\d+(\\.\\d+)? , -?\\d+(\\.\\d+)? "
            "\\] \\)"
        );
        _last_indent.push(tOperation);
        return *_current_child;
    } else if (str.starts_with("rotate")) {
        _current_child = get_parser_ptr<Rotate, double, double, double>(
            shared_from_this(),
            "\\( \\[ -?\\d+(\\.\\d+)? , -?\\d+(\\.\\d+)? , -?\\d+(\\.\\d+)? "
            "\\] \\)"
        );
        _last_indent.push(tOperation);
        return *_current_child;
    } else if (str.starts_with("mirror")) {
        _current_child = get_parser_ptr<Mirror, double, double, double>(
            shared_from_this(),
            "\\( v = \\[ -?\\d+(\\.\\d+)? , -?\\d+(\\.\\d+)? , -?\\d+(\\.\\d+)? "
            "\\] \\)"
        );
        _last_indent.push(tOperation);
        return *_current_child;
    } else if (str.starts_with("square")) {
        _current_child = get_parser_ptr<Rectangle, double, double, bool>(
            shared_from_this(),
            "\\( size = \\[ -?\\d+(\\.\\d+)? , -?\\d+(\\.\\d+)? \\] , center = "
            "(true)|(false) \\)"
        );
        return *_current_child;
    } else if (str.starts_with("circle")) {
        _current_child = get_parser_ptr<Circle, double>(
            shared_from_this(), "\\( r = -?\\d+(\\.\\d+)? , \\$fn = \\d+ \\)"
        );
        return *_current_child;
    } else if (str.starts_with("polygon")) {
        auto skip_till_the_end = std::make_shared<NullBuffer>(
            shared_from_this(), "\\)"
        );
        auto regex_vector = split_by(
            "\\[ -?\\d+(\\.\\d+)? , -?\\d+(\\.\\d+)? \\] [,\\]]"
        );
        std::shared_ptr<IAutoma<std::vector<double>>> automa_ptr
            = std::make_shared<PrimitiveCyclicAutoma<double>>(
                regex_vector, ","
            );
        std::shared_ptr<ShapeParser<Polygon, std::vector<double>>> parse_polygon
            = std::make_shared<ShapeParser<Polygon, std::vector<double>>>(
                skip_till_the_end, automa_ptr
            );
        _current_child = std::make_unique<NullBuffer>(parse_polygon, "\\[");
        return *_current_child;
    } else if (str.starts_with("color")) {
        _last_indent.push(tOther);
        _current_child = std::make_unique<NullBuffer>(shared_from_this(), "\\)");
        return *_current_child;
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
