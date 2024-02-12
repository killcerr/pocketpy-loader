#pragma once
#include <cstdint>
#include <string_view>
#include <variant>
#include <vector>


namespace native_helper {
using TypeVariant = std::variant<
    std::monostate,
    uint8_t,
    uint16_t,
    uint32_t,
    uint64_t,
    int8_t,
    int16_t,
    int32_t,
    int64_t,
    float,
    double,
    void*,
    bool,
    char>;
enum class ArgType : unsigned char { n = -1, v, ui8, ui16, ui32, ui64, i8, i16, i32, i64, f, d, p, b, c };
ArgType form_string(const auto& str) {
    using enum ArgType;
    using namespace std::string_view_literals;
#define RT_IF(T)                                                                                                       \
    if (str == #T##sv) return T;
    RT_IF(v);
    RT_IF(ui8);
    RT_IF(ui16);
    RT_IF(ui32);
    RT_IF(ui64);
    RT_IF(i8);
    RT_IF(i16);
    RT_IF(i32);
    RT_IF(i64);
    RT_IF(f);
    RT_IF(d);
    RT_IF(b);
    RT_IF(c);
#undef RT_IF
    return n;
}
template <ArgType type>
constexpr auto cast_to_int = [] {
    using enum ArgType;
#define RT_IF(T, I)                                                                                                    \
    if constexpr (type == T) return I
    RT_IF(v, 0);
    RT_IF(ui8, 1);
    RT_IF(ui16, 2);
    RT_IF(ui32, 3);
    RT_IF(ui64, 4);
    RT_IF(i8, 5);
    RT_IF(i16, 6);
    RT_IF(i32, 7);
    RT_IF(i64, 8);
    RT_IF(f, 9);
    RT_IF(d, 10);
    RT_IF(p, 11);
    RT_IF(b, 12);
    RT_IF(c, 13);
#undef RT_IF
}();
TypeVariant call(ArgType ret, const std::vector<ArgType>& args, const std::vector<TypeVariant>& vals, void* fp);
} // namespace native_helper
