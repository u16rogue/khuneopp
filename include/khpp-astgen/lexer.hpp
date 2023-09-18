#pragma once

#if defined(KHPP_ASTGEN) && KHPP_ASTGEN == 1

extern "C" {
  #include <kh-astgen/lexer.h>
}

#include <khpp-core/refobj.hpp>

namespace khpp::lexer {

using raw_code_ref = khpp::refobj::irefobj<kh_utf8sp>;
using status       = kh_lexer_status;
using token_type   = kh_lexer_token_type;
using result_value = kh_lexer_parse_result_value;
using result       = kh_lexer_parse_result;

enum class action {
  STOP,
  CONTINUE,
  _INVALID,
};

/*
 *  Extends and implements `kh_lexer_context` while providing
 *  member functions for operations related to the lexer context.
 *
 *  Initialization and Uninitialization of the underlying context
 *  is automatically managed by the struct's constructor and destructor.
 */
struct context {

  /*
   *  Implements `kh_lexer_context_init`.
   *  Takes an `irefobj` that contains the
   *  raw source code.
   */
  context(raw_code_ref code);

  /*
   *  Implements `kh_lexer_context_uninit`
   */
  ~context();

  operator bool() const noexcept;

  /*
   *  Implements `kh_lexer_context_parse_next`
   */
  auto parse_next(lexer::result & out_result) -> status;

  /*
   *  # Parse context
   *  Run a continuous parse until a EOB (End of buffer)
   *  or error is met.
   *
   *  ## Arguments
   *  - cb  : Result callback. Called everytime a lexeme is produced.
   *        : Signature: `callback(result &) -> action`
   *            * Returning an Action of `CONTINUE` will cause the parser to continue.
   *            * Returning an Action of `STOP` will stop the parser and return `true`.
   *
   *  - err : [optional] Error callback. Called when the lexer produces a non ok status.
   *        : Signature: `error(status) -> action` 
   *            * Returning an Action of `CONTINUE` will ignore the status and would continue
   *              to dispatch. This can be used to either ignore or if the error/warning has been
   *              fixed and it is safe to continue.
   *            * Returning an Action of `STOP` will stop the parser and returns `true` under the
   *              assumption that the error was handled by the error callback.
   *        : If `err` is not provided the parser will return false upon a non ok status.
   */
  template <typename T, typename U, bool has_error_handler = true>
  auto parse(T && cb, U && err) -> bool {

    for (;;) {
      lexer::result result = {
        .type  = KH_LEXER_TOKEN_TYPE_NONE,
        .value = {},
      };
      auto status = parse_next(result);

      if (status != KH_LEXER_STATUS_MATCH) {
        if (status == KH_LEXER_STATUS_EOB) {
          return true;
        }

        if constexpr (has_error_handler) {
          switch (err(status)) {
            case action::CONTINUE:
              break;
            case action::STOP:
              return true;
            default:
              return false;
          }
        } else {
          return false;
        }
      }

      static_assert(
        /* requires { cb(result, status); } || */ requires { cb(result); },
        "Callback function should have a signature that can takes a parse result and an optional status eg: 'cb(result &) -> action'."// or 'cb(result &, status) -> action'."
      );

      action act = action::_INVALID;
      //if constexpr (requires { cb(result, status); }) {
      //  act = cb(result, status);
      //} else if constexpr (requires { cb(result); }) {
        act = cb(result);
      //}

      switch (act) {
        case action::CONTINUE:
          break;
        case action::STOP:
          return true;
        default:
          return false;
      }
    }

    return false;
  }

  template <typename T>
  auto parse(T && cb) -> bool {
    return [&]<typename U>(U && err) -> bool {
      return parse<T, U, false>(static_cast<T &&>(cb), static_cast<U &&>(err));
    }( [](status) -> action { return action::_INVALID; } );
  }

  auto operator->() const noexcept -> const kh_lexer_context *;

private:
  kh_lexer_context ctx = {
    ._code_buffer = KH_REFOBJ_INVALID_IREF,
    ._code_index  = 0,
  };
};

} // namespace khpp::lexer

#endif
