//===----------------------------------------------------------------------===//
//                         DuckDB
//
// common/operator/numeric_inplace_bitwise_operators.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

namespace duckdb {

struct BitwiseXORInPlace {
	template <class T> static inline void Operation(T &left, T right) {
		left ^= right;
	}
};

struct BitwiseXORInPlace2 {
	template <class T> static inline void Operation(T &left, T &right) {
		left ^= right;
	}
};

struct BitwiseFloatXORInPlace {
	template <class T> static inline void Operation(T &left, T &right) {
		const auto l = reinterpret_cast<unsigned char *>(&left);
		const auto r = reinterpret_cast<unsigned char *>(&right);

		for (int i = 0; i < sizeof left; ++i) {
			l[i] ^= r[i];
		}
	}
};

} // namespace duckdb
