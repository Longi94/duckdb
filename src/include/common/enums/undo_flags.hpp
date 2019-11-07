//===----------------------------------------------------------------------===//
//                         DuckDB
//
// common/enums/undo_flags.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/constants.hpp"

namespace duckdb {

enum class UndoFlags : uint8_t {
	EMPTY_ENTRY = 0,
	CATALOG_ENTRY = 1,
	INSERT_TUPLE = 2,
	DELETE_TUPLE = 3,
	UPDATE_TUPLE = 4,
	QUERY = 5,
	DATA = 6
};

} // namespace duckdb
