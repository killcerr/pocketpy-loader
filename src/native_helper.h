#pragma once
#include <cstdint>
#include <ll/api/memory/Hook.h>
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
    bool,
    char,
    void*>;
enum class ArgType : signed char { n = -1, v, ui8, ui16, ui32, ui64, i8, i16, i32, i64, f, d, b, c, p };
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
    RT_IF(p);
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
    RT_IF(b, 11);
    RT_IF(c, 12);
    RT_IF(p, 13);
#undef RT_IF
}();
TypeVariant call(ArgType ret, const std::vector<ArgType>& args, const std::vector<TypeVariant>& vals, void* fp);

void hook(
    std::string_view                                           sym,
    ll::memory::HookPriority                                   pri,
    ArgType                                                    ret,
    const std::vector<ArgType>&                                args,
    std::function<TypeVariant(std::vector<TypeVariant>& args)> cb
);
} // namespace native_helper
