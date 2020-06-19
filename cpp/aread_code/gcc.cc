using source_location = uint32_t
using cppchar_t = uint32_t

// token type
enum cpp_ttype  // cpplib.h

// 词法分析调用链
c-parser.c::c_lex_one_token
c_lex_with_flags
cpp_get_token_with_location
macro.c::cpp_get_token_1
lex.c::_cpp_lex_token
lex.c::_cpp_lex_direct

_cpp_lex_direct(cpp_reader *pfile) cpp_token* {
  // lex.c
  // 解析出一个token
}
