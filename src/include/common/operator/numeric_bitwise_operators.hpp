//===----------------------------------------------------------------------===//
//                         DuckDB
//
// common/operator/numeric_bitwise_operators.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

namespace duckdb {

struct BitwiseXOR {
	template <class T> static inline T Operation(T left, T right) {
		return left ^ right;
	}
};

struct BitwiseFloatXOR
{
	template <class T> static inline T Operation(T left, T right) {
		unsigned char *l = reinterpret_cast<unsigned char *>(&left);
		unsigned char *r = reinterpret_cast<unsigned char *>(&right);

		for (int i = 0; i < sizeof left; ++i) {
			l[i] ^= r[i];
		}
		return left;
	}
};

struct BitwiseAND {
	template <class T> static inline T Operation(T left, T right) {
		return left & right;
	}
};

struct BitwiseOR {
	template <class T> static inline T Operation(T left, T right) {
		return left | right;
	}
};

struct BitwiseShiftLeft {
	template <class T> static inline T Operation(T left, T right) {
		return left << right;
	}
};

struct BitwiseShiftRight {
	template <class T> static inline T Operation(T left, T right) {
		return left >> right;
	}
};

struct BitwiseNOT {
	template <class T> static inline T Operation(T input) {
		return ~input;
	}
};

} // namespace duckdb
