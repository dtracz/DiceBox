#ifndef PARSEABLE_H_INCLUDED
#define PARSEABLE_H_INCLUDED

#include <concepts>
#include <memory>

namespace parser {

class Parseable {
  public:
    // template <typename ParsedT>
    // requires std::derived_from<ParsedT, Parseable>
    // std::shared_ptr<ParsedT> as()
    // {
    //     return dynamic_cast<ParsedT>(*this);
    // }
    //
    // template <typename ParsedT>
    // requires std::derived_from<ParsedT, Parseable>
    // std::shared_ptr<ParsedT> as_shared()
    // {
    //     return std::dynamic_pointer_cast<ParsedT>(
    //         std::make_shared<Parseable>(*this)
    //     );
    // }

    // template <typename ParsedT>
    // requires std::derived_from<ParsedT, Parseable>
    // std::shared_ptr<ParsedT> as_shared()
    // {
    //     std::shared_ptr<Parseable> shp = shared_from_this();
    //     return std::dynamic_pointer_cast<ParsedT>(shp);
    // }

    virtual void print() = 0;
}; // class Parseable


} // namespace parser

#endif // PARSEABLE_H_INCLUDED
