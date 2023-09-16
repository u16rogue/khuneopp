#include <khpp-astgen/lexer.hpp>


khpp::lexer::context::context(raw_code_ref code) {
  if (kh_lexer_context_init(&this->ctx, code) != KH_TRUE) {
    ctx._code_buffer = KH_REFOBJ_INVALID_IREF;
  }
}

khpp::lexer::context::~context() {
  kh_lexer_context_uninit(&this->ctx);
}

khpp::lexer::context::operator bool() const noexcept {
  return ctx._code_buffer != KH_REFOBJ_INVALID_IREF;
}

auto khpp::lexer::context::parse_next(result & out_result) -> status {
  return kh_lexer_context_parse_next(&this->ctx, &out_result);
}
