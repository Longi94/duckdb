#include "common/operator/numeric_bitwise_operators.hpp"
#include "common/vector_operations/fold_loops.hpp"
#include "common/vector_operations/vector_operations.hpp"

using namespace std;
using namespace duckdb;

namespace duckdb {

struct BitwiseFloatXOR {
	template <class T> static inline T Operation(T left, T right) {
		const auto l = reinterpret_cast<unsigned char *>(&left);
		const auto r = reinterpret_cast<unsigned char *>(&right);

		for (int i = 0; i < sizeof left; ++i) {
			l[i] ^= r[i];
		}
		return left;
	}
};
} // namespace duckdb

Value VectorOperations::ChecksumXor(Vector &input) {
	if (input.count == 0) {
		return 0;
	}
	Value result;
	switch (input.type) {
	case TypeId::BOOLEAN:
		result = Value::BOOLEAN(0);
		templated_unary_fold<int8_t, int8_t, duckdb::BitwiseXOR>(input, &result.value_.boolean);
		break;
	case TypeId::TINYINT:
		result = Value::TINYINT(0);
		templated_unary_fold<int8_t, int8_t, duckdb::BitwiseXOR>(input, &result.value_.tinyint);
		break;
	case TypeId::SMALLINT:
		result = Value::SMALLINT(0);
		templated_unary_fold<int16_t, int16_t, duckdb::BitwiseXOR>(input, &result.value_.smallint);
		break;
	case TypeId::INTEGER:
		result = Value::INTEGER(0);
		templated_unary_fold<int32_t, int32_t, duckdb::BitwiseXOR>(input, &result.value_.integer);
		break;
	case TypeId::VARCHAR:
	case TypeId::BIGINT:
		result = Value::BIGINT(0);
		templated_unary_fold<int64_t, int64_t, duckdb::BitwiseXOR>(input, &result.value_.bigint);
		break;
	case TypeId::FLOAT:
		result = Value::FLOAT(0);
		templated_unary_fold<float, float, duckdb::BitwiseFloatXOR>(input, &result.value_.float_);
		break;
	case TypeId::DOUBLE:
		result = Value::DOUBLE(0);
		templated_unary_fold<double, double, duckdb::BitwiseFloatXOR>(input, &result.value_.double_);
		break;
	default:
		break;
	}
	return result;
}
