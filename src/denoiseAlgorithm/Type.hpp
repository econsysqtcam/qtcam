/*
 * BM3D denoising filter - VapourSynth plugin
 * Copyright (c) 2015-2016 mawen1250
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TYPE_H_
#define TYPE_H_

#include <stddef.h>

#include <cfloat>
#include <cstdint>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef int8_t sint8;
typedef int16_t sint16;
typedef int32_t sint32;
typedef int64_t sint64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef long double ldbl;

typedef int PCType;
typedef float FLType;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// determine whether _Ty satisfies Signed Int requirements
template <typename _Ty>
struct _IsSInt
    : std::integral_constant<bool, std::is_same<_Ty, signed char>::value || std::is_same<_Ty, short>::value ||
                                       std::is_same<_Ty, int>::value || std::is_same<_Ty, long>::value ||
                                       std::is_same<_Ty, long long>::value> {};

// determine whether _Ty satisfies Unsigned Int requirements
template <typename _Ty>
struct _IsUInt
    : std::integral_constant<bool,
                             std::is_same<_Ty, unsigned char>::value || std::is_same<_Ty, unsigned short>::value ||
                                 std::is_same<_Ty, unsigned int>::value || std::is_same<_Ty, unsigned long>::value ||
                                 std::is_same<_Ty, unsigned long long>::value> {};

// determine whether _Ty satisfies Int requirements
template <typename _Ty>
struct _IsInt : std::integral_constant<bool, _IsSInt<_Ty>::value || _IsUInt<_Ty>::value> {};

// determine whether _Ty satisfies Float requirements
template <typename _Ty>
struct _IsFloat : std::integral_constant<bool, std::is_same<_Ty, float>::value || std::is_same<_Ty, double>::value ||
                                                   std::is_same<_Ty, long double>::value> {};

#define isSInt(T) (_IsSInt<T>::value)
#define isUInt(T) (_IsUInt<T>::value)
#define isInt(T) (_IsInt<T>::value)
#define isFloat(T) (_IsFloat<T>::value)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Min value and Max value of each numeric type
#define IntMin(type) \
  (type(sizeof(type) <= 1 ? INT8_MIN : sizeof(type) <= 2 ? INT16_MIN : sizeof(type) <= 4 ? INT32_MIN : INT64_MIN))
#define IntMax(type) \
  (type(sizeof(type) <= 1 ? INT8_MAX : sizeof(type) <= 2 ? INT16_MAX : sizeof(type) <= 4 ? INT32_MAX : INT64_MAX))
#define UIntMin(type) (type(0))
#define UIntMax(type) \
  (type(sizeof(type) <= 1 ? UINT8_MAX : sizeof(type) <= 2 ? UINT16_MAX : sizeof(type) <= 4 ? UINT32_MAX : UINT64_MAX))
#define FltMin(type) (type(sizeof(type) <= 4 ? FLT_MIN : sizeof(type) <= 8 ? DBL_MIN : LDBL_MIN))
#define FltMax(type) (type(sizeof(type) <= 4 ? FLT_MAX : sizeof(type) <= 8 ? DBL_MAX : LDBL_MAX))
#define FltNegMax(type) (type(sizeof(type) <= 4 ? -FLT_MAX : sizeof(type) <= 8 ? -DBL_MAX : -LDBL_MAX))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Ty>
_Ty _TypeMinInt(const std::false_type&) {
  return UIntMin(_Ty);
}

template <typename _Ty>
_Ty _TypeMinInt(const std::true_type&) {
  return IntMin(_Ty);
}

template <typename _Ty>
_Ty _TypeMin(const std::false_type&) {
  return _TypeMinInt<_Ty>(_IsSInt<_Ty>());
}

template <typename _Ty>
_Ty _TypeMin(const std::true_type&) {
  return FltNegMax(_Ty);
}

template <typename _Ty>
_Ty TypeMin() {
  return _TypeMin<_Ty>(_IsFloat<_Ty>());
}

template <typename _Ty>
_Ty TypeMin(const _Ty&) {
  return TypeMin<_Ty>();
}

template <typename _Ty>
_Ty _TypeMaxInt(const std::false_type&) {
  return UIntMax(_Ty);
}

template <typename _Ty>
_Ty _TypeMaxInt(const std::true_type&) {
  return IntMax(_Ty);
}

template <typename _Ty>
_Ty _TypeMax(const std::false_type&) {
  return _TypeMaxInt<_Ty>(_IsSInt<_Ty>());
}

template <typename _Ty>
_Ty _TypeMax(const std::true_type&) {
  return FltMax(_Ty);
}

template <typename _Ty>
_Ty TypeMax() {
  return _TypeMax<_Ty>(_IsFloat<_Ty>());
}

template <typename _Ty>
_Ty TypeMax(const _Ty&) {
  return TypeMax<_Ty>();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Pos;
struct Pos3;

struct Pos {
  typedef Pos _Myt;

  PCType y = 0;
  PCType x = 0;

  explicit Pos(PCType _y = 0, PCType _x = 0) : y(_y), x(_x) {}

  explicit Pos(const Pos3& _pos3);

  _Myt& operator=(const Pos3& right);

  bool operator==(const _Myt& right) const { return y == right.y && x == right.x; }

  bool operator!=(const _Myt& right) const { return y != right.y || x != right.x; }

  bool operator<(const _Myt& right) const {
    if (y < right.y) {
      return true;
    } else if (y > right.y) {
      return false;
    } else if (x < right.x) {
      return true;
    } else {
      return false;
    }
  }

  bool operator>(const _Myt& right) const {
    if (y > right.y) {
      return true;
    } else if (y < right.y) {
      return false;
    } else if (x > right.x) {
      return true;
    } else {
      return false;
    }
  }

  bool operator>=(const _Myt& right) const { return !(*this < right); }

  bool operator<=(const _Myt& right) const { return !(*this > right); }

  friend std::ostream& operator<<(std::ostream& out, const _Myt& src) {
    out << "(" << src.y << ", " << src.x << ")";

    return out;
  }
};

struct Pos3 {
  typedef Pos3 _Myt;

  PCType z = 0;
  PCType y = 0;
  PCType x = 0;

  explicit Pos3(PCType _z = 0, PCType _y = 0, PCType _x = 0) : z(_z), y(_y), x(_x) {}

  explicit Pos3(const Pos& _pos2, const PCType& _z = 0);

  _Myt& operator=(const Pos& right);

  bool operator==(const _Myt& right) const { return z == right.z && y == right.y && x == right.x; }

  bool operator!=(const _Myt& right) const { return z != right.z || y != right.y || x != right.x; }

  bool operator<(const _Myt& right) const {
    if (z < right.z) {
      return true;
    } else if (z > right.z) {
      return false;
    } else if (y < right.y) {
      return true;
    } else if (y > right.y) {
      return false;
    } else if (x < right.x) {
      return true;
    } else {
      return false;
    }
  }

  bool operator>(const _Myt& right) const {
    if (z > right.z) {
      return true;
    } else if (z < right.z) {
      return false;
    } else if (y > right.y) {
      return true;
    } else if (y < right.y) {
      return false;
    } else if (x > right.x) {
      return true;
    } else {
      return false;
    }
  }

  bool operator>=(const _Myt& right) const { return !(*this < right); }

  bool operator<=(const _Myt& right) const { return !(*this > right); }

  friend std::ostream& operator<<(std::ostream& out, const _Myt& src) {
    out << "(" << src.z << ", " << src.y << ", " << src.x << ")";

    return out;
  }
};

inline Pos::Pos(const Pos3& _pos3) : y(_pos3.y), x(_pos3.x) {}

inline Pos& Pos::operator=(const Pos3& right) {
  y = right.y;
  x = right.x;
  return *this;
}

inline Pos3::Pos3(const Pos& _pos2, const PCType& _z) : z(_z), y(_pos2.y), x(_pos2.x) {}

inline Pos3& Pos3::operator=(const Pos& right) {
  y = right.y;
  x = right.x;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Ty1, typename _Ty2>
struct KeyPair : public std::pair<_Ty1, _Ty2> {
  typedef KeyPair<_Ty1, _Ty2> _Myt;
  typedef std::pair<_Ty1, _Ty2> _Mybase;

  typedef _Ty1 KeyType;
  typedef _Ty2 ValType;

  KeyPair() : _Mybase() {}

  KeyPair(const _Ty1& _Val1, const _Ty2& _Val2) : _Mybase(_Val1, _Val2) {}

  KeyPair(const _Myt& _Right) : _Mybase(_Right) {}

  KeyPair(_Myt&& _Right) : _Mybase(_Right) {}

  _Myt& operator=(const _Myt& _Right) {
    _Mybase::operator=(_Right);
    return *this;
  }

  _Myt& operator=(_Myt&& _Right) {
    _Mybase::operator=(_Right);
    return *this;
  }

  bool operator==(const _Myt& _Right) { return this->first == _Right.first; }

  bool operator!=(const _Myt& _Right) { return this->first != _Right.first; }

  bool operator<(const _Myt& _Right) { return this->first < _Right.first; }

  bool operator>(const _Myt& _Right) { return this->first > _Right.first; }

  bool operator<=(const _Myt& _Right) { return this->first <= _Right.first; }

  bool operator>=(const _Myt& _Right) { return this->first >= _Right.first; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
