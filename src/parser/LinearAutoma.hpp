#ifndef LINEAR_AUTOMA_HPP_INCLUDED
#define LINEAR_AUTOMA_HPP_INCLUDED

#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>


class ParseError : public std::runtime_error {
  public:
    ParseError(const std::string& msg)
        : std::runtime_error(msg)
    { }
}; // class ParseError;


template <typename T>
bool write_from_str(T* value, const std::string& inp)
{
    return false;
}

template <>
bool write_from_str(double* value, const std::string& inp)
{
    try {
        *value = std::stod(inp);
        return true;
    } catch (std::invalid_argument e) {
        return false;
    }
}

template <>
bool write_from_str(bool* value, const std::string& inp)
{
    if (inp == "true") {
        *value = true;
        return true;
    } else if (inp == "false") {
        *value = false;
        return true;
    }
    return false;
}

template <int I, typename... Ts>
bool write_from_str(int i, std::tuple<Ts...>& tuple, const std::string& inp)
{
    if (i > I)
        throw std::out_of_range("Index out of tuple range");
    if (i < I)
        return write_from_str<(I - 1 < 0 ? I : I - 1)>(i, tuple, inp);
    auto& value = std::get<I>(tuple);
    return write_from_str(&value, inp);
}

template <typename... Ts>
bool write_from_str(int i, std::tuple<Ts...>& tuple, const std::string& inp)
{
    return write_from_str<sizeof...(Ts) - 1>(i, tuple, inp);
}



template <typename... Ts>
class LinearAutoma {
  public:
    LinearAutoma(const std::vector<std::regex>& expected_seq)
        : _expected_seq { expected_seq }
        , _state { _expected_seq.begin() }
        , _loaded { 0 }
    { }

    LinearAutoma& operator<<(const std::string& inp)
    {
        if (at_endstate())
            throw ParseError("Automa already at its end state");
        if (!std::regex_match(inp, *_state))
            throw ParseError(
                "Given input \"" + inp + "\" does not match expected regex"
            );
        _state++;
        if (_loaded < sizeof...(Ts) && write_from_str(_loaded, _result, inp))
            _loaded++;
        return *this;
    }

    bool at_endstate()
    {
        return _state == _expected_seq.end();
    }

    std::tuple<Ts...> get_result()
    {
        return _result;
    }

  private:
    std::vector<std::regex> _expected_seq;
    std::vector<std::regex>::iterator _state;
    std::tuple<Ts...> _result;
    int _loaded;
}; // class LinearAutoma


#endif // LINEAR_AUTOMA_HPP_INCLUDED