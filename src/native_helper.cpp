#include "logger.h"

#include <cstdint>
#include <dyncall/dyncall.h>
#include <dyncall/dyncall_callback.h>
#include <ll/api/memory/Hook.h>
#include <magic_enum.hpp>
#include <variant>
#include <vector>


FILE_LOGGER();


namespace native_helper {
enum class ArgType : signed char { n = -1, v, ui8, ui16, ui32, ui64, i8, i16, i32, i64, f, d, p, b, c };
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
        case b:
            dcArgBool(vm, std::get<cast_to_int<b>>(vals[i]));
            break;
        case c:
            dcArgChar(vm, std::get<cast_to_int<c>>(vals[i]));
            break;
        case p:
            dcArgPointer(vm, std::get<cast_to_int<p>>(vals[i]));
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
    case b:
        res.emplace<cast_to_int<b>>(dcCallBool(vm, fp));
        break;
    case c:
        res.emplace<cast_to_int<c>>(dcCallChar(vm, fp));
        break;
    case p:
        res.emplace<cast_to_int<p>>(dcCallPointer(vm, fp));
        break;
    default:
        std::unreachable();
    }
    return res;
}

auto tr(ArgType t) {
    using enum ArgType;
    switch (t) {
    case v:
        return DC_SIGCHAR_VOID;
        break;
    case ui8:
        return DC_SIGCHAR_UCHAR;
    case i8:
        return DC_SIGCHAR_CHAR;
    case ui16:
        return DC_SIGCHAR_USHORT;
    case i16:
        return DC_SIGCHAR_SHORT;
    case ui32:
        return DC_SIGCHAR_UINT;
    case i32:
        return DC_SIGCHAR_INT;
    case ui64:
        return DC_SIGCHAR_ULONGLONG;
    case i64:
        return DC_SIGCHAR_LONGLONG;
    case f:
        return DC_SIGCHAR_FLOAT;
    case d:
        return DC_SIGCHAR_DOUBLE;
    case b:
        return DC_SIGCHAR_DOUBLE;
    case c:
        return DC_SIGCHAR_CHAR;
    case p:
        return DC_SIGCHAR_POINTER;
    default:
        std::unreachable();
        break;
    }
}

struct CallBackData {
    std::function<TypeVariant(std::vector<TypeVariant>& args)> scb;
    std::vector<ArgType>                                       argtypes;
    ArgType                                                    rettype;
    void*                                                      ori;
};

DCsigchar handler(DCCallback* pcb, DCArgs* args, DCValue* result, void* userdata) {
    auto                     cb = (CallBackData*)userdata;
    std::vector<TypeVariant> _args;
    for (auto argtype : cb->argtypes) {
        TypeVariant va;
        using enum ArgType;
        switch (argtype) {
        case i8:
            va.emplace<cast_to_int<i8>>(dcbArgChar(args));
            break;
        case ui8:
            va.emplace<cast_to_int<ui8>>(dcbArgUChar(args));
            break;
        case i16:
            va.emplace<cast_to_int<i16>>(dcbArgShort(args));
            break;
        case ui16:
            va.emplace<cast_to_int<ui16>>(dcbArgUShort(args));
            break;
        case i32:
            va.emplace<cast_to_int<i32>>(dcbArgInt(args));
            break;
        case ui32:
            va.emplace<cast_to_int<ui32>>(dcbArgUInt(args));
            break;
        case i64:
            va.emplace<cast_to_int<i64>>(dcbArgLongLong(args));
            break;
        case ui64:
            va.emplace<cast_to_int<ui64>>(dcbArgULongLong(args));
            break;
        case f:
            va.emplace<cast_to_int<f>>(dcbArgFloat(args));
            break;
        case d:
            va.emplace<cast_to_int<d>>(dcbArgDouble(args));
            break;
        case b:
            va.emplace<cast_to_int<b>>(dcbArgBool(args));
            break;
        case c:
            va.emplace<cast_to_int<c>>(dcbArgChar(args));
            break;
        case p:
            va.emplace<cast_to_int<p>>(dcbArgPointer(args));
            break;
        default:
            break;
        }
    }
    auto res = cb->scb(_args);
    using enum ArgType;
    switch (cb->rettype) {
    case v:
        break;
    case ui8:
        result->C = std::get<cast_to_int<ui8>>(res);
        break;
    case i8:
        result->c = std::get<cast_to_int<i8>>(res);
        break;
    case ui16:
        result->S = std::get<cast_to_int<ui16>>(res);
        break;
    case i16:
        result->s = std::get<cast_to_int<i16>>(res);
        break;
    case ui32:
        result->I = std::get<cast_to_int<ui32>>(res);
        break;
    case i32:
        result->i = std::get<cast_to_int<i32>>(res);
        break;
    case ui64:
        result->L = std::get<cast_to_int<ui64>>(res);
        break;
    case i64:
        result->l = std::get<cast_to_int<i64>>(res);
        break;
    case f:
        result->f = std::get<cast_to_int<f>>(res);
        break;
    case d:
        result->d = std::get<cast_to_int<d>>(res);
        break;
    case b:
        result->B = std::get<cast_to_int<b>>(res);
        break;
    case c:
        result->c = std::get<cast_to_int<c>>(res);
        break;
    case p:
        result->p = std::get<cast_to_int<p>>(res);
        break;
    default:
        std::unreachable();
    }
    return tr(cb->rettype);
}

void hook(
    std::string_view                                           sym,
    ll::memory::HookPriority                                   pri,
    ArgType                                                    ret,
    const std::vector<ArgType>&                                args,
    std::function<TypeVariant(std::vector<TypeVariant>& args)> cb
) {
    std::string sig;
    for (auto arg : args) {
        sig += tr(arg);
    }
    sig           += DC_SIGCHAR_ENDARG;
    sig           += tr(ret);
    auto cbd       = new CallBackData();
    cbd->argtypes  = args;
    cbd->rettype   = ret;
    cbd->scb       = cb;
    auto ncb       = dcbNewCallback(sig.c_str(), handler, cbd);
    ll::memory::hook(ll::memory::resolveSymbol(sym, false), (ll::memory::FuncPtr)ncb, &(cbd->ori), pri);
}
} // namespace native_helper
