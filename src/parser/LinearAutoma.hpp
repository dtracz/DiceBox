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
inline bool load_from_str(T* value, const std::string& inp)
{
    return false;
}

template <>
inline bool load_from_str(double* value, const std::string& inp)
{
    try {
        *value = std::stod(inp);
        return true;
    } catch (std::invalid_argument e) {
        return false;
    }
}

template <>
inline bool load_from_str(bool* value, const std::string& inp)
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
inline bool
load_from_str(int i, std::tuple<Ts...>& tuple, const std::string& inp)
{
    if (i > I)
        throw std::out_of_range("Index out of tuple range");
    if (i < I)
        return load_from_str<(I - 1 < 0 ? I : I - 1)>(i, tuple, inp);
    auto& value = std::get<I>(tuple);
    return load_from_str(&value, inp);
}

template <typename... Ts>
inline bool
load_from_str(int i, std::tuple<Ts...>& tuple, const std::string& inp)
{
    return load_from_str<sizeof...(Ts) - 1>(i, tuple, inp);
}



template <typename... Ts>
class IAutoma {
  public:
    virtual IAutoma& operator<<(const std::string&) = 0;

    /**
     * endstate of automa cannot have any output paths!
     */
    virtual bool at_endstate() = 0;

    virtual std::tuple<Ts...> get_result() = 0;
};



template <typename... Ts>
class LinearAutoma : public IAutoma<Ts...> {
  public:
    LinearAutoma(const std::vector<std::regex>& expected_seq)
        : _expected_seq { expected_seq }
        , _state { 0 }
        , _loaded { 0 }
    { }

    LinearAutoma(const std::vector<std::string>& regex_strings)
        : _expected_seq { regex_strings.size() }
        , _state { 0 }
        , _loaded { 0 }
    {
        for (size_t i = 0; i < regex_strings.size(); i++)
            _expected_seq[i] = std::regex { regex_strings[i] };
    }

    LinearAutoma& operator<<(const std::string& inp)
    {
        if (at_endstate())
            throw ParseError("Automa already at its end state");
        if (!std::regex_match(inp, _expected_seq[_state]))
            throw ParseError(
                "Given input \"" + inp + "\" does not match expected regex"
            );
        _state++;
        if (_loaded < sizeof...(Ts) && load_from_str(_loaded, _result, inp))
            _loaded++;
        return *this;
    }

    bool at_endstate()
    {
        return _state == _expected_seq.size();
    }

    std::tuple<Ts...> get_result()
    {
        if (!at_endstate())
            std::runtime_error("Parsing not finished");
        return _result;
    }

  private:
    std::vector<std::regex> _expected_seq;
    int _state;
    std::tuple<Ts...> _result;
    int _loaded;
}; // class LinearAutoma


template <>
class LinearAutoma<> : public IAutoma<> {
  public:
    LinearAutoma(const std::vector<std::regex>& expected_seq)
        : _expected_seq { expected_seq }
        , _state { 0 }
    { }

    LinearAutoma(const std::vector<std::string>& regex_strings)
        : _expected_seq { regex_strings.size() }
        , _state { 0 }
    {
        for (size_t i = 0; i < regex_strings.size(); i++)
            _expected_seq[i] = std::regex { regex_strings[i] };
    }

    LinearAutoma& operator<<(const std::string& inp)
    {
        if (at_endstate())
            throw ParseError("Automa already at its end state");
        if (!std::regex_match(inp, _expected_seq[_state]))
            throw ParseError(
                "Given input \"" + inp + "\" does not match expected regex"
            );
        _state++;
        return *this;
    }

    bool at_endstate()
    {
        return _state == _expected_seq.size();
    }

    std::tuple<> get_result()
    {
        if (!at_endstate())
            std::runtime_error("Parsing not finished");
        return std::make_tuple();
    }


  private:
    std::vector<std::regex> _expected_seq;
    int _state;
}; // class LinearAutoma



template <typename T>
class PrimitiveCyclicAutoma : public IAutoma<std::vector<T>> {
  public:
    PrimitiveCyclicAutoma(
        const std::vector<std::regex>& cyclic_seq, const std::regex& final_token
    )
        : _cyclic_seq { cyclic_seq }
        , _final_token { final_token }
        , _state { 0 }
    { }

    PrimitiveCyclicAutoma(
        const std::vector<std::string>& regex_strings,
        const std::string& final_token_str
    )
        : _cyclic_seq { regex_strings.size() }
        , _final_token { final_token_str }
        , _state { 0 }
    {
        for (size_t i = 0; i < regex_strings.size(); i++)
            _cyclic_seq[i] = std::regex { regex_strings[i] };
    }

    PrimitiveCyclicAutoma<T>& operator<<(const std::string& inp)
    {
        if (at_endstate())
            throw ParseError("Automa already at its end state");
        if (std::regex_match(inp, _cyclic_seq[_state])) {
            _state = (_state + 1) % _cyclic_seq.size();
            T value;
            if (load_from_str(&value, inp))
                _result.push_back(value);
        } else if (_state == 0 && std::regex_match(inp, _final_token)) {
            _state = _cyclic_seq.size();
        } else {
            throw ParseError(
                "Given input \"" + inp + "\" does not match expected regex"
            );
        }
        return *this;
    }

    bool at_endstate()
    {
        return _state == _cyclic_seq.size();
    }

    std::tuple<std::vector<T>> get_result()
    {
        if (!at_endstate())
            std::runtime_error("Parsing not finished");
        return std::make_tuple(_result);
    }

  private:
    std::vector<std::regex> _cyclic_seq;
    std::regex _final_token;
    int _state;
    std::vector<T> _result;
}; // class PrimitiveCyclicAutoma



#endif // LINEAR_AUTOMA_HPP_INCLUDED
