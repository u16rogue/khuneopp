#pragma once

extern "C" {
  #include <kh-core/refobj.h>
}

#include <khpp-core/types.hpp>

namespace khpp::refobj {

using rcb_fnt = kh_refobj_rcb_fnt;

template <typename T> struct irefobj;

/*
 *  Implements `kh_refobj` used for storing
 *  a refrence object.
 *
 *  NOTE: `refobj` stores its own `refobji` instance.
 *  At the point where the object is supposed to
 *  be unreferenced it is normal that the reference
 *  count is one (1). The destructor or `remove` method
 *  will handle this.
 */
template <typename T>
struct refobj {
  using self = refobj;
  friend irefobj<T>;

  refobj(T * pobject, rcb_fnt resource_callback = KH_REFOBJ_RCB_NO_CALLBACK) noexcept {
    if (kh_refobj_init(&this->ref, pobject, &this->iref, resource_callback) == KH_FALSE) {
      this->iref = KH_REFOBJ_INVALID_IREF;
    }
  }

  auto remove() noexcept -> bool {
    if (
      (kh_refobj_get_ref_count(this->iref) != 1)
      ||
      (this->iref != KH_REFOBJ_INVALID_IREF && kh_refobj_iremove(&this->iref) != KH_TRUE)
      ||
      (kh_refobj_uninit(&this->ref) != KH_TRUE)
    ) {
      return false;
    }

    return true;
  }

  ~refobj() noexcept {
    remove(); 
  }

  auto alive() const noexcept -> bool {
    return iref != KH_REFOBJ_INVALID_IREF && kh_refobj_alive(&this->ref) == KH_TRUE;
  }

  /*
   *  Returns the real reference count of a reference object.
   *
   *  NOTE: This does not include its internal reference to
   *  mirror khuneo's behavior.
   */
  auto ref_count_real() const noexcept -> khpp::u8 {
    return kh_refobj_get_ref_count(this->iref);
  }

  /*
   *  Returns the reference count of a reference object.
   */ 
  auto ref_count() const noexcept -> khpp::u8 {
    return ref_count_real() - (this->iref != KH_REFOBJ_INVALID_IREF);
  } 

  auto operator->() -> T * {
    return kh_refobj_get_object(this->iref);
  }

  auto operator*() -> T & {
    return *reinterpret_cast<T*>(kh_refobj_get_object(this->iref));
  }

  operator bool() const noexcept {
    return this->alive();
  }

  auto operator=(self &) -> void = delete;
  auto operator=(const self &) -> void = delete;
  auto operator=(self &&) -> void = delete;
  auto operator=(const self &&) -> void = delete;

private:
  kh_refobj ref = {
    ._object            = KH_NULLPTR,
    ._count             = 0,
    ._resource_callback = KH_REFOBJ_RCB_NO_CALLBACK,
  };

  kh_refobji iref = KH_REFOBJ_INVALID_IREF;
};

/*
 *  Implements `kh_refobji` used for representing
 *  an instance of a reference.
 */
template <typename T>
struct irefobj {
  using self = irefobj<T>;

  irefobj(refobj<T> & parent) {
    kh_refobj_icopy(&parent.iref, &this->iref);
  }

  auto remove() -> self & {
    if (this->iref != KH_REFOBJ_INVALID_IREF) {
      kh_refobj_iremove(&this->iref);
    }
    return *this;
  }

  auto copy(self & other) -> self & {
    remove();
    kh_refobj_icopy(&other.iref, &this->iref);
    return *this;
  }

  auto move(self && other) -> self & {
    remove();
    kh_refobj_imove(&other.iref, &this->iref);
    return *this;
  }

  irefobj(self & rhs) { 
    copy(rhs);
  }

  irefobj(self && rhs) {
    move(rhs);
  }

  ~irefobj() {
    remove();
  }

  auto alive() const noexcept -> bool{
    return kh_refobj_ialive(&this->iref) == KH_TRUE;
  }

  operator bool() const noexcept {
    return this->alive();
  }

  auto operator=(self && rhs) -> self & {
    return move(rhs);
  }

  auto operator=(self & rhs) -> self & {
    return copy(rhs);
  }

  auto operator->() -> T * {
    return reinterpret_cast<T *>(kh_refobj_get_object(this->iref));
  }

  auto operator*() -> T & {
    return *reinterpret_cast<T *>(kh_refobj_get_object(this->iref));
  }

  operator kh_refobji() {
    return kh_refobj_icopyarg(&this->iref);
  } 

private:
  kh_refobji iref = KH_REFOBJ_INVALID_IREF;
};

} // namespace khpp::refobj

