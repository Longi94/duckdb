#include "common/vector_operations/vector_operations.hpp"
#include "common/vector_operations/fold_loops.hpp"
#include "common/operator/numeric_bitwise_operators.hpp"

using namespace std;
using namespace duckdb;

Value VectorOperations::ChecksumXor(Vector &input) {
	if (input.count == 0) {
		return 0;
	}
	Value result = Value::Numeric(input.type, 0);
	switch (input.type) {
	case TypeId::BOOLEAN: {
		templated_unary_fold<int8_t, int8_t, duckdb::BitwiseXOR>(input, &result.value_.boolean);
		break;
	}
	case TypeId::VARCHAR: {
		break;
	}
	case TypeId::TINYINT:
		templated_unary_fold<int8_t, int8_t, duckdb::BitwiseXOR>(input, &result.value_.tinyint);
		break;
	case TypeId::SMALLINT:
		templated_unary_fold<int16_t, int16_t, duckdb::BitwiseXOR>(input, &result.value_.smallint);
		break;
	case TypeId::INTEGER:
		templated_unary_fold<int32_t, int32_t, duckdb::BitwiseXOR>(input, &result.value_.integer);
		break;
	case TypeId::BIGINT:
		templated_unary_fold<int64_t, int64_t, duckdb::BitwiseXOR>(input, &result.value_.bigint);
		break;
	case TypeId::FLOAT:
		// templated_unary_fold<float, float, duckdb::BitwiseXOR>(input, &result.value_.float_);
		break;
	case TypeId::DOUBLE:
		// templated_unary_fold<double, double, duckdb::BitwiseXOR>(input, &result.value_.double_);
		break;
	default:
		break;
	}
	return result;
}
