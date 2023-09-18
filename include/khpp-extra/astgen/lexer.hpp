#pragma once

#if defined(KHPP_EXTRA_ASTGEN) && KHPP_EXTRA_ASTGEN == 1

#include <khpp-astgen/lexer.hpp>
#include <string>

namespace khpp::extra::lexer {

  auto tostr_ctx_status(khpp::lexer::status status) -> std::string;
  auto tostr_token_type(khpp::lexer::token_type type) -> std::string;

  auto tostr_result_value(const khpp::utf8sp & in_raw_code, const khpp::lexer::result & in_result) -> std::string;
  auto tostr_result_value(const khpp::lexer::context & in_ctx_with_code, const khpp::lexer::result & in_result) -> std::string;

} // namespace khpp::extra::lexer

#endif
