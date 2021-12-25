#pragma once

#include <limits>

static constexpr double   NaN     = std::numeric_limits<double>::quiet_NaN();
static constexpr float    NaNf    = std::numeric_limits<float>::quiet_NaN();

template <typename T> T nan();

#define YQ_NAN( type, ... ) template <> constexpr type nan<type>() { return __VA_ARGS__; } 

YQ_NAN(float, NaNf)
YQ_NAN(double, NaN)

