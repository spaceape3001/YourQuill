#pragma once

#include <util/preamble.hpp>

template <typename T> T nan();

#define YQ_NAN( type, ... ) template <> constexpr type nan<type>() { return __VA_ARGS__; } 

YQ_NAN(float, yq::NaNf)
YQ_NAN(double, yq::NaN)

