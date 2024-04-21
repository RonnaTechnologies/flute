# Flute

A C++20 fixed-point single header library, for embedded devices.

⚠️ This project is a work in progress, use at your own risk.

## Description

Flute is a C++20 fixed-point library. It aims to provide a modern interface that allows to deal with fixed-point arithmetic. It is meant to be used on systems that don't have an FPU (floating-point unit), such as micro-controllers.

## Features

* **Zero-overhead:** You only pay for what you need. For instance (real-world example), compiling for an ARM Cortex-M3 using GCC 13.2, *flute* allows to multiply an integer and a `ufixed<12, 20>` as fast as a regular 32-bit integer multiplication. Compared to multiplying an integer with a 32-bit float, this allows to save about 1277 bytes in the compiled firmware.
* **Embedded-friendly:** The STL is not required. *Flute* only needs a few C++ headers: `array`, `concepts`, `limits`, and `type_traits`.
* **Modern C++ standards:** Requires **C++20**. *Concepts* make the code more reliable and provide readable error messages. Moreover, modern C++ is key to achieve optimal performance.
* **Compile-time evaluation:** Everything is **constexpr**! This has a very positive impact on performance too.
* **Header-only library:** No need to link the library, simply include `flute.hpp` into your *.cpp* files.
* **Policy-based design:** Some fixed-point operations require to make choices, especially regarding overflow.

## Usage

### Prerequisites

Since this is a header only library, all you need is a C++20 compiler. This library has only been tested with GCC 13.2 (x86-64 and arm-none-eabi). It should work with GCC 10 and Clang 16.

### Installing

It's a single header library, so there's nothing to install. All you need is to include `flute.hpp` in your C++ files.

### Example

```cpp
#include "flute.hpp"

int main()
{
  using namespace flute;

  constexpr ufixed<16,16> pi{3.1415926};
  constexpr auto two_pi = 2 * pi;

  static_assert((two_pi - pi).as<double>() == pi.as<double>());
}
```

## Building the project

It's a header-only library, include `flute.hpp` and build your source files as usual.

## Documentation

No documentation yet, but there will be one for sure.

## Running the tests

## Contributing

## Versioning

This project will make use of [SemVer](http://semver.org/) for versioning.

## Authors

* **Ronna Technologies** - [@RonnaTechnologies](https://github.com/RonnaTechnologies)
* **Jeremy Oden** - [@SpintroniK](https://github.com/SpintroniK)

## License

This project is licensed under the [MIT](LICENSE) license.
