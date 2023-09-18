#include <khpp-extra/astgen/lexer.hpp>

extern "C" {
  #include <kh-extra/astgen/lexer.h>
}

#include <memory>

auto khpp::extra::lexer::tostr_ctx_status(khpp::lexer::status status) -> std::string {
  return kh_extra_lexer_tostr_ctx_status(status)->buffer;
}

auto khpp::extra::lexer::tostr_token_type(khpp::lexer::token_type type) -> std::string {
  return kh_extra_lexer_tostr_token_type(type)->buffer;
}

auto khpp::extra::lexer::tostr_result_value(const khpp::utf8sp & in_raw_code, const khpp::lexer::result & in_result) -> std::string {
  const auto sz = kh_extra_lexer_predict_size_tostr_result_value(&in_result) + 1;
  auto buff     = std::make_unique<char[]>(sz);

  if (!buff) {
    return "<khpp alloc failed>";
  }

  khpp::utf8sp_s out = {
    .str = {
      .size   = 0,
      .buffer = buff.get(),
    },
    .buffer_size = sz,
  };

  if (kh_extra_lexer_tostr_result_value(&in_raw_code, &in_result, &out) == KH_TRUE) {
    return std::string(out.str.buffer);
  }

  return "<error>";
}

auto khpp::extra::lexer::tostr_result_value(const khpp::lexer::context & in_ctx_with_code, const khpp::lexer::result & in_result) -> std::string {
  return tostr_result_value(*reinterpret_cast<kh_utf8sp *>(kh_refobj_get_object(in_ctx_with_code->_code_buffer)), in_result);
}
