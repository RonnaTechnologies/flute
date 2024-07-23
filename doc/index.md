# About flute

## What is flute?

Flute is a C++20 fixed-point library. It aims to provide a
 modern interface that allows to deal with fixed-point arithmetic. It is
 meant to be used on systems that don't have an FPU (floating-point
unit), such as micro-controllers.

Flute is a bit different from other fixed-point libraries. It includes utilities for look-up tables (LUT, hence the name "flute").

```cpp linenums="1"
#include "flute.hpp"

int main()
{
  using namespace flute;

  constexpr ufixed<16,16> pi{3.1415926};
  constexpr auto two_pi = 2 * pi;

  static_assert((two_pi - pi).as<double>() == pi.as<double>());
}
```
