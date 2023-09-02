#ifndef PARSEABLE_H_INCLUDED
#define PARSEABLE_H_INCLUDED

#include <concepts>
#include <memory>


class Parseable {
  public:
    virtual void print() = 0;
}; // class Parseable

#endif // PARSEABLE_H_INCLUDED
