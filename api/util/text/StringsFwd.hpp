#pragma once

#include <util/collection/AllFwd.hpp>
#include <util/type/Result.hpp>
#include <util/type/ResultFwd.hpp>

#include <cwchar>
#include <cuchar>
#include <string>
#include <string_view>

class QByteArray;
class QString;

namespace yq {

    class Char8;
    class Char32;

    using u32string_r   = Result<std::u32string>;
    using wstring_r     = Result<std::wstring>;

    class String;
    class StringView;
}
