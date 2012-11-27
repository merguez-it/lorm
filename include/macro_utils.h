/*
 *  macro_utils.h
 *  LORM
 *
 *  Created by Grégoire Lejeune.
 *
 */

#ifndef __MACRO_UTILS_H
#define __MACRO_UTILS_H

#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2

#define COUNT_ARGS(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__) 
#define FOR_EACH_ARG_N( \
 _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
_21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
_31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
_41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
_51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
_61, _62, _63, _64, _65, _66, _67, _68, _69, _70, \
_71, _72, _73, _74, _75, _76, _77, _78, _79, _80, \
_81, _82, _83, _84, _85, _86, _87, _88, _89, _90, \
_91, _92, _93, _94, _95, _96, _97, _98, _99,   N, ...) N 
#define FOR_EACH_RSEQ_N() \
99, 98, 97, 96, 95, 94, 93, 92, 91, 90, \
89, 88, 87, 86, 85, 84, 83, 82, 81, 80, \
79, 78, 77, 76, 75, 74, 73, 72, 71, 70, \
69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define FOR_EACH_1(what, o, x, ...) what(o, x)
#define FOR_EACH_2(what, o, x, ...) what(o, x) FOR_EACH_1(what, o, __VA_ARGS__)
#define FOR_EACH_3(what, o, x, ...) what(o, x) FOR_EACH_2(what, o, __VA_ARGS__)
#define FOR_EACH_4(what, o, x, ...) what(o, x) FOR_EACH_3(what, o, __VA_ARGS__)
#define FOR_EACH_5(what, o, x, ...) what(o, x) FOR_EACH_4(what, o, __VA_ARGS__)
#define FOR_EACH_6(what, o, x, ...) what(o, x) FOR_EACH_5(what, o, __VA_ARGS__)
#define FOR_EACH_7(what, o, x, ...) what(o, x) FOR_EACH_6(what, o, __VA_ARGS__)
#define FOR_EACH_8(what, o, x, ...) what(o, x) FOR_EACH_7(what, o, __VA_ARGS__)
#define FOR_EACH_9(what, o, x, ...) what(o, x) FOR_EACH_8(what, o, __VA_ARGS__)
#define FOR_EACH_10(what, o, x, ...) what(o, x) FOR_EACH_9(what, o, __VA_ARGS__)
#define FOR_EACH_11(what, o, x, ...) what(o, x) FOR_EACH_10(what, o, __VA_ARGS__)
#define FOR_EACH_12(what, o, x, ...) what(o, x) FOR_EACH_11(what, o, __VA_ARGS__)
#define FOR_EACH_13(what, o, x, ...) what(o, x) FOR_EACH_12(what, o, __VA_ARGS__)
#define FOR_EACH_14(what, o, x, ...) what(o, x) FOR_EACH_13(what, o, __VA_ARGS__)
#define FOR_EACH_15(what, o, x, ...) what(o, x) FOR_EACH_14(what, o, __VA_ARGS__)
#define FOR_EACH_16(what, o, x, ...) what(o, x) FOR_EACH_15(what, o, __VA_ARGS__)
#define FOR_EACH_17(what, o, x, ...) what(o, x) FOR_EACH_16(what, o, __VA_ARGS__)
#define FOR_EACH_18(what, o, x, ...) what(o, x) FOR_EACH_17(what, o, __VA_ARGS__)
#define FOR_EACH_19(what, o, x, ...) what(o, x) FOR_EACH_18(what, o, __VA_ARGS__)
#define FOR_EACH_20(what, o, x, ...) what(o, x) FOR_EACH_19(what, o, __VA_ARGS__)
#define FOR_EACH_21(what, o, x, ...) what(o, x) FOR_EACH_20(what, o, __VA_ARGS__)
#define FOR_EACH_22(what, o, x, ...) what(o, x) FOR_EACH_21(what, o, __VA_ARGS__)
#define FOR_EACH_23(what, o, x, ...) what(o, x) FOR_EACH_22(what, o, __VA_ARGS__)
#define FOR_EACH_24(what, o, x, ...) what(o, x) FOR_EACH_23(what, o, __VA_ARGS__)
#define FOR_EACH_25(what, o, x, ...) what(o, x) FOR_EACH_24(what, o, __VA_ARGS__)
#define FOR_EACH_26(what, o, x, ...) what(o, x) FOR_EACH_25(what, o, __VA_ARGS__)
#define FOR_EACH_27(what, o, x, ...) what(o, x) FOR_EACH_26(what, o, __VA_ARGS__)
#define FOR_EACH_28(what, o, x, ...) what(o, x) FOR_EACH_27(what, o, __VA_ARGS__)
#define FOR_EACH_29(what, o, x, ...) what(o, x) FOR_EACH_28(what, o, __VA_ARGS__)
#define FOR_EACH_30(what, o, x, ...) what(o, x) FOR_EACH_29(what, o, __VA_ARGS__)
#define FOR_EACH_31(what, o, x, ...) what(o, x) FOR_EACH_30(what, o, __VA_ARGS__)
#define FOR_EACH_32(what, o, x, ...) what(o, x) FOR_EACH_31(what, o, __VA_ARGS__)
#define FOR_EACH_33(what, o, x, ...) what(o, x) FOR_EACH_32(what, o, __VA_ARGS__)
#define FOR_EACH_34(what, o, x, ...) what(o, x) FOR_EACH_33(what, o, __VA_ARGS__)
#define FOR_EACH_35(what, o, x, ...) what(o, x) FOR_EACH_34(what, o, __VA_ARGS__)
#define FOR_EACH_36(what, o, x, ...) what(o, x) FOR_EACH_35(what, o, __VA_ARGS__)
#define FOR_EACH_37(what, o, x, ...) what(o, x) FOR_EACH_36(what, o, __VA_ARGS__)
#define FOR_EACH_38(what, o, x, ...) what(o, x) FOR_EACH_37(what, o, __VA_ARGS__)
#define FOR_EACH_39(what, o, x, ...) what(o, x) FOR_EACH_38(what, o, __VA_ARGS__)
#define FOR_EACH_40(what, o, x, ...) what(o, x) FOR_EACH_39(what, o, __VA_ARGS__)
#define FOR_EACH_41(what, o, x, ...) what(o, x) FOR_EACH_40(what, o, __VA_ARGS__)
#define FOR_EACH_42(what, o, x, ...) what(o, x) FOR_EACH_41(what, o, __VA_ARGS__)
#define FOR_EACH_43(what, o, x, ...) what(o, x) FOR_EACH_42(what, o, __VA_ARGS__)
#define FOR_EACH_44(what, o, x, ...) what(o, x) FOR_EACH_43(what, o, __VA_ARGS__)
#define FOR_EACH_45(what, o, x, ...) what(o, x) FOR_EACH_44(what, o, __VA_ARGS__)
#define FOR_EACH_46(what, o, x, ...) what(o, x) FOR_EACH_45(what, o, __VA_ARGS__)
#define FOR_EACH_47(what, o, x, ...) what(o, x) FOR_EACH_46(what, o, __VA_ARGS__)
#define FOR_EACH_48(what, o, x, ...) what(o, x) FOR_EACH_47(what, o, __VA_ARGS__)
#define FOR_EACH_49(what, o, x, ...) what(o, x) FOR_EACH_48(what, o, __VA_ARGS__)
#define FOR_EACH_50(what, o, x, ...) what(o, x) FOR_EACH_49(what, o, __VA_ARGS__)
#define FOR_EACH_51(what, o, x, ...) what(o, x) FOR_EACH_50(what, o, __VA_ARGS__)
#define FOR_EACH_52(what, o, x, ...) what(o, x) FOR_EACH_51(what, o, __VA_ARGS__)
#define FOR_EACH_53(what, o, x, ...) what(o, x) FOR_EACH_52(what, o, __VA_ARGS__)
#define FOR_EACH_54(what, o, x, ...) what(o, x) FOR_EACH_53(what, o, __VA_ARGS__)
#define FOR_EACH_55(what, o, x, ...) what(o, x) FOR_EACH_54(what, o, __VA_ARGS__)
#define FOR_EACH_56(what, o, x, ...) what(o, x) FOR_EACH_55(what, o, __VA_ARGS__)
#define FOR_EACH_57(what, o, x, ...) what(o, x) FOR_EACH_56(what, o, __VA_ARGS__)
#define FOR_EACH_58(what, o, x, ...) what(o, x) FOR_EACH_57(what, o, __VA_ARGS__)
#define FOR_EACH_59(what, o, x, ...) what(o, x) FOR_EACH_58(what, o, __VA_ARGS__)
#define FOR_EACH_60(what, o, x, ...) what(o, x) FOR_EACH_59(what, o, __VA_ARGS__)
#define FOR_EACH_61(what, o, x, ...) what(o, x) FOR_EACH_60(what, o, __VA_ARGS__)
#define FOR_EACH_62(what, o, x, ...) what(o, x) FOR_EACH_61(what, o, __VA_ARGS__)
#define FOR_EACH_63(what, o, x, ...) what(o, x) FOR_EACH_62(what, o, __VA_ARGS__)
#define FOR_EACH_64(what, o, x, ...) what(o, x) FOR_EACH_63(what, o, __VA_ARGS__)
#define FOR_EACH_65(what, o, x, ...) what(o, x) FOR_EACH_64(what, o, __VA_ARGS__)
#define FOR_EACH_66(what, o, x, ...) what(o, x) FOR_EACH_65(what, o, __VA_ARGS__)
#define FOR_EACH_67(what, o, x, ...) what(o, x) FOR_EACH_66(what, o, __VA_ARGS__)
#define FOR_EACH_68(what, o, x, ...) what(o, x) FOR_EACH_67(what, o, __VA_ARGS__)
#define FOR_EACH_69(what, o, x, ...) what(o, x) FOR_EACH_68(what, o, __VA_ARGS__)
#define FOR_EACH_70(what, o, x, ...) what(o, x) FOR_EACH_69(what, o, __VA_ARGS__)
#define FOR_EACH_71(what, o, x, ...) what(o, x) FOR_EACH_70(what, o, __VA_ARGS__)
#define FOR_EACH_72(what, o, x, ...) what(o, x) FOR_EACH_71(what, o, __VA_ARGS__)
#define FOR_EACH_73(what, o, x, ...) what(o, x) FOR_EACH_72(what, o, __VA_ARGS__)
#define FOR_EACH_74(what, o, x, ...) what(o, x) FOR_EACH_73(what, o, __VA_ARGS__)
#define FOR_EACH_75(what, o, x, ...) what(o, x) FOR_EACH_74(what, o, __VA_ARGS__)
#define FOR_EACH_76(what, o, x, ...) what(o, x) FOR_EACH_75(what, o, __VA_ARGS__)
#define FOR_EACH_77(what, o, x, ...) what(o, x) FOR_EACH_76(what, o, __VA_ARGS__)
#define FOR_EACH_78(what, o, x, ...) what(o, x) FOR_EACH_77(what, o, __VA_ARGS__)
#define FOR_EACH_79(what, o, x, ...) what(o, x) FOR_EACH_78(what, o, __VA_ARGS__)
#define FOR_EACH_80(what, o, x, ...) what(o, x) FOR_EACH_79(what, o, __VA_ARGS__)
#define FOR_EACH_81(what, o, x, ...) what(o, x) FOR_EACH_80(what, o, __VA_ARGS__)
#define FOR_EACH_82(what, o, x, ...) what(o, x) FOR_EACH_81(what, o, __VA_ARGS__)
#define FOR_EACH_83(what, o, x, ...) what(o, x) FOR_EACH_82(what, o, __VA_ARGS__)
#define FOR_EACH_84(what, o, x, ...) what(o, x) FOR_EACH_83(what, o, __VA_ARGS__)
#define FOR_EACH_85(what, o, x, ...) what(o, x) FOR_EACH_84(what, o, __VA_ARGS__)
#define FOR_EACH_86(what, o, x, ...) what(o, x) FOR_EACH_85(what, o, __VA_ARGS__)
#define FOR_EACH_87(what, o, x, ...) what(o, x) FOR_EACH_86(what, o, __VA_ARGS__)
#define FOR_EACH_88(what, o, x, ...) what(o, x) FOR_EACH_87(what, o, __VA_ARGS__)
#define FOR_EACH_89(what, o, x, ...) what(o, x) FOR_EACH_88(what, o, __VA_ARGS__)
#define FOR_EACH_90(what, o, x, ...) what(o, x) FOR_EACH_89(what, o, __VA_ARGS__)
#define FOR_EACH_91(what, o, x, ...) what(o, x) FOR_EACH_90(what, o, __VA_ARGS__)
#define FOR_EACH_92(what, o, x, ...) what(o, x) FOR_EACH_91(what, o, __VA_ARGS__)
#define FOR_EACH_93(what, o, x, ...) what(o, x) FOR_EACH_92(what, o, __VA_ARGS__)
#define FOR_EACH_94(what, o, x, ...) what(o, x) FOR_EACH_93(what, o, __VA_ARGS__)
#define FOR_EACH_95(what, o, x, ...) what(o, x) FOR_EACH_94(what, o, __VA_ARGS__)
#define FOR_EACH_96(what, o, x, ...) what(o, x) FOR_EACH_95(what, o, __VA_ARGS__)
#define FOR_EACH_97(what, o, x, ...) what(o, x) FOR_EACH_96(what, o, __VA_ARGS__)

#define FOR_EACH_(N, what, o, x, ...) CONCATENATE(FOR_EACH_, N)(what, o, x, __VA_ARGS__)
#define FOR_EACH(what, o, x, ...) FOR_EACH_(COUNT_ARGS(x, __VA_ARGS__), what, o, x, __VA_ARGS__)


#endif // __MACRO_UTILS_H
