#include "logger.h"

#include <cstdint>
#include <dyncall/dyncall.h>
#include <magic_enum.hpp>
#include <variant>
#include <vector>

FILE_LOGGER();


namespace native_helper {
enum class ArgType : unsigned char { n = -1, v, ui8, ui16, ui32, ui64, i8, i16, i32, i64, f, d, p, b, c };
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

TypeVariant call(ArgType ret, const std::vector<ArgType>& args, const std::vector<TypeVariant>& vals, void* fp) {
    auto vm = dcNewCallVM(1024);
    using enum ArgType;
    // logger.info("call:{}", __LINE__);
    for (int i = 0; i < std::min(args.size(), vals.size()); i++) {
        switch (args[i]) {
        case ui8:
            dcArgChar(vm, std::get<cast_to_int<ui8>>(vals[i]));
            break;
        case i8:
            dcArgChar(vm, std::get<cast_to_int<i8>>(vals[i]));
            break;
        case ui16:
            dcArgShort(vm, std::get<cast_to_int<ui16>>(vals[i]));
            break;
        case i16:
            dcArgShort(vm, std::get<cast_to_int<ui16>>(vals[i]));
            break;
        case ui32:
            dcArgInt(vm, std::get<cast_to_int<ui32>>(vals[i]));
            break;
        case i32:
            dcArgInt(vm, std::get<cast_to_int<i32>>(vals[i]));
            break;
        case ui64:
            dcArgLongLong(vm, std::get<cast_to_int<ui64>>(vals[i]));
            break;
        case i64:
            dcArgLongLong(vm, std::get<cast_to_int<i64>>(vals[i]));
            break;
        case f:
            dcArgFloat(vm, std::get<cast_to_int<f>>(vals[i]));
            break;
        case d:
            dcArgDouble(vm, std::get<cast_to_int<d>>(vals[i]));
            break;
        case p:
            dcArgPointer(vm, std::get<cast_to_int<p>>(vals[i]));
            break;
        case b:
            dcArgBool(vm, std::get<cast_to_int<b>>(vals[i]));
            break;
        case c:
            dcArgChar(vm, std::get<cast_to_int<c>>(vals[i]));
            break;
        default:
            std::unreachable();
        }
    }
    // logger.info("call:{}", __LINE__);
    TypeVariant res{};
    switch (ret) {
    case v:
        dcCallVoid(vm, fp);
        break;
    case ui8:
        res.emplace<cast_to_int<ui8>>(dcCallChar(vm, fp));
        break;
    case i8:
        res.emplace<cast_to_int<i8>>(dcCallChar(vm, fp));
        break;
    case ui16:
        res.emplace<cast_to_int<ui16>>(dcCallShort(vm, fp));
        break;
    case i16:
        res.emplace<cast_to_int<i16>>(dcCallShort(vm, fp));
        break;
    case ui32:
        res.emplace<cast_to_int<ui32>>(dcCallInt(vm, fp));
        break;
    case i32:
        res.emplace<cast_to_int<i32>>(dcCallInt(vm, fp));
        break;
    case ui64:
        res.emplace<cast_to_int<ui64>>(dcCallLongLong(vm, fp));
        break;
    case i64:
        res.emplace<cast_to_int<i64>>(dcCallLongLong(vm, fp));
        break;
    case f:
        res.emplace<cast_to_int<f>>(dcCallFloat(vm, fp));
        break;
    case d:
        res.emplace<cast_to_int<d>>(dcCallDouble(vm, fp));
        break;
    case p:
        res.emplace<cast_to_int<p>>(dcCallPointer(vm, fp));
        break;
    case b:
        res.emplace<cast_to_int<b>>(dcCallBool(vm, fp));
        break;
    case c:
        res.emplace<cast_to_int<c>>(dcCallChar(vm, fp));
        break;
    default:
        std::unreachable();
    }
    return res;
}
} // namespace native_helper
