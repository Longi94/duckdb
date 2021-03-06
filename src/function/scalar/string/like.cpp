#include "duckdb/function/scalar/string_functions.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/common/vector_operations/vector_operations.hpp"

using namespace std;

namespace duckdb {

struct LikeOperator {
	static bool Operation(const char *left, const char *right, const char *escape = nullptr);
};

struct NotLikeOperator {
	static inline bool Operation(const char *left, const char *right, const char *escape = nullptr) {
		return !LikeOperator::Operation(left, right, escape);
	}
};

bool LikeOperator::Operation(const char *s, const char *pattern, const char *escape) {
	const char *t, *p;

	t = s;
	for (p = pattern; *p && *t; p++) {
		if (escape && *p == *escape) {
			p++;
			if (*p != *t) {
				return false;
			}
			t++;
		} else if (*p == '_') {
			t++;
		} else if (*p == '%') {
			p++;
			while (*p == '%') {
				p++;
			}
			if (*p == 0) {
				return true; /* tail is acceptable */
			}
			for (; *p && *t; t++) {
				if (LikeOperator::Operation(t, p, escape)) {
					return true;
				}
			}
			if (*p == 0 && *t == 0) {
				return true;
			}
			return false;
		} else if (*p == *t) {
			t++;
		} else {
			return false;
		}
	}
	if (*p == '%' && *(p + 1) == 0) {
		return true;
	}
	return *t == 0 && *p == 0;
}

void LikeFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(
	    ScalarFunction("~~", {SQLType::VARCHAR, SQLType::VARCHAR}, SQLType::BOOLEAN,
	                   ScalarFunction::BinaryFunction<const char *, const char *, bool, LikeOperator, true>));
	set.AddFunction(
	    ScalarFunction("!~~", {SQLType::VARCHAR, SQLType::VARCHAR}, SQLType::BOOLEAN,
	                   ScalarFunction::BinaryFunction<const char *, const char *, bool, NotLikeOperator, true>));
}

} // namespace duckdb
