// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

// This file is generated by generate_for_each.py. DO NOT EDIT BY HAND!
#pragma once

#include "count.h"
#include "misc.h"

#define NX_PP_FOR_EACH(Macro, Data, ...) \
    NX_PP_EXPAND(NX_PP_CAT( \
        _NX_PP_FOR_EACH_, NX_PP_ARGS_COUNT(__VA_ARGS__))(Macro, Data, __VA_ARGS__))

#define _NX_PP_FOR_EACH_1(Macro, Data, Item) Macro(Data, Item)
#define _NX_PP_FOR_EACH_2(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_1(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_3(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_2(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_4(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_3(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_5(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_4(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_6(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_5(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_7(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_6(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_8(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_7(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_9(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_8(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_10(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_9(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_11(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_10(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_12(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_11(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_13(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_12(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_14(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_13(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_15(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_14(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_16(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_15(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_17(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_16(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_18(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_17(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_19(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_18(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_20(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_19(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_21(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_20(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_22(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_21(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_23(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_22(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_24(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_23(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_25(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_24(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_26(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_25(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_27(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_26(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_28(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_27(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_29(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_28(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_30(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_29(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_31(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_30(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_32(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_31(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_33(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_32(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_34(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_33(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_35(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_34(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_36(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_35(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_37(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_36(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_38(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_37(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_39(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_38(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_40(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_39(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_41(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_40(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_42(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_41(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_43(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_42(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_44(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_43(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_45(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_44(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_46(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_45(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_47(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_46(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_48(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_47(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_49(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_48(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_50(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_49(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_51(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_50(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_52(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_51(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_53(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_52(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_54(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_53(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_55(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_54(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_56(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_55(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_57(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_56(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_58(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_57(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_59(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_58(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_60(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_59(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_61(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_60(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_62(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_61(Macro, Data, __VA_ARGS__))
#define _NX_PP_FOR_EACH_63(Macro, Data, Item, ...) \
    Macro(Data, Item) NX_PP_EXPAND(_NX_PP_FOR_EACH_62(Macro, Data, __VA_ARGS__))
