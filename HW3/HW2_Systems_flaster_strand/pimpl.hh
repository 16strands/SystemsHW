#include <memory>

class foo {
 private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;

 public:
  foo();
  ~foo();
  int dosomething();
};


