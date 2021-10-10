#pragma once

#include "ListFwd.hpp"
#include "Result.hpp"
#include "ResultFwd.hpp"
#include "SetFwd.hpp"
#include "VectorFwd.hpp"

#include <cwchar>
#include <cuchar>
#include <string>
#include <string_view>

class QByteArray;
class QString;

class Char8;
class Char32;

using u32string_r   = Result<std::u32string>;
using wstring_r     = Result<std::wstring>;

class String;
class StringView;
