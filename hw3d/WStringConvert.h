#pragma once
#include <string>

namespace wstring_convert {
    std::string to_string(std::wstring_view);
    std::wstring to_wstring(std::string_view);
}