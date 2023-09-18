#pragma once

extern "C" {
  #include <kh-core/types.h>
}

namespace khpp {
  using u8   = kh_u8;
  using u64  = kh_u64;
  using sz   = kh_sz;

  using unum = kh_unum;
  using iflt = kh_iflt;

  using utf8     = kh_utf8;
  using utf8sp   = kh_utf8sp;
  using utf8sp_s = kh_utf8sp_s;

} // namespace khpp

