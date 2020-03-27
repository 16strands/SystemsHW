#include "pimpl.hh"

class foo::Impl {
 public:
  int value_;
};

foo::foo()
 : pimpl_(new Impl())
{
  pimpl_->value_ = 56;
}


foo::~foo()
{
}

int foo::dosomething()
{
  return pimpl_->value_;
}
