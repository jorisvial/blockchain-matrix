/* Example of Outcome
(C) 2017-2020 Niall Douglas <http://www.nedproductions.biz/> (3 commits), Krzemienski <sg0897542@global.ad.sabre.com> (3 commits) and Andrzej Krzemienski <akrzemi1@gmail.com> (3 commits)


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "../../../include/boost/outcome.hpp"
#include <iostream>

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

//! [decl_f_g]
auto f() -> int;  // throws on failure
auto g() noexcept -> outcome::result<int>;
//! [decl_f_g]

auto f() -> int
{
  throw std::runtime_error{"failure"};
}

auto g() noexcept -> outcome::result<int>
{
  return 1;
}

namespace old
{
    auto h() noexcept -> outcome::outcome<int>;
}

//! [def_h]
auto old::h() noexcept -> outcome::outcome<int>
{
  BOOST_OUTCOME_TRY(i, (g()));             // #1
    
  try {
    return i + f();
  }
  catch (...) {
    return std::current_exception(); // #2
  }
}
//! [def_h]

//! [def_z]
auto z() -> int // throws on failure
{
  if (outcome::outcome<int> o = old::h())
    return o.value();
  else if (o.has_exception())
    std::rethrow_exception(o.exception());
  else
    throw std::system_error{o.error()};
}
//! [def_z]

int main()
try {
  return z();
}
catch(std::runtime_error const& e) {
  std::cerr << e.what() << std::endl;
  return 1;
}