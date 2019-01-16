#include "catch.hpp"
#include "test_helpers.hpp"

using namespace duckdb;
using namespace std;

TEST_CASE("Test view creation", "[views]") {
	unique_ptr<DuckDBResult> result;
	DuckDB db(nullptr);
	DuckDBConnection con(db);
	con.EnableQueryVerification();

	// create a table
	REQUIRE_NO_FAIL(con.Query("CREATE TABLE t1(i INTEGER)"));
	REQUIRE_NO_FAIL(con.Query("INSERT INTO t1 VALUES (41), (42), (43)"));
	REQUIRE_NO_FAIL(con.Query("CREATE VIEW v1 AS SELECT i AS j FROM t1 WHERE i < 43"));
	REQUIRE_FAIL(con.Query("CREATE VIEW v1 AS SELECT 'whatever'"));

	result = con.Query("select j FROM v1 WHERE j > 41");
	REQUIRE(CHECK_COLUMN(result, 0, {42}));
	REQUIRE_NO_FAIL(con.Query("DROP VIEW v1"));
	REQUIRE_FAIL(con.Query("select j FROM v1 WHERE j > 41"));

	REQUIRE_NO_FAIL(con.Query("CREATE VIEW v1 AS SELECT 'whatever'"));
	result = con.Query("select * FROM v1");
	REQUIRE(CHECK_COLUMN(result, 0, {"whatever"}));

	REQUIRE_NO_FAIL(con.Query("CREATE OR REPLACE VIEW v1 AS SELECT 42"));
	result = con.Query("select * FROM v1");
	REQUIRE(CHECK_COLUMN(result, 0, {42}));

	REQUIRE_NO_FAIL(con.Query("DROP VIEW v1"));
	REQUIRE_FAIL(con.Query("DROP VIEW v1"));
	REQUIRE_NO_FAIL(con.Query("DROP VIEW IF EXISTS v1"));

	REQUIRE_FAIL(con.Query("CREATE VIEW v1 AS SELECT * FROM dontexist"));
}

TEST_CASE("Test view creation with alias", "[views]") {
	unique_ptr<DuckDBResult> result;
	DuckDB db(nullptr);
	DuckDBConnection con(db);
	con.EnableQueryVerification();

	// create a table
	REQUIRE_NO_FAIL(con.Query("CREATE TABLE t1(i INTEGER)"));
	REQUIRE_NO_FAIL(con.Query("INSERT INTO t1 VALUES (41), (42), (43)"));

	// this should fail because there are more aliases for the view than columns in the query
	REQUIRE_FAIL(con.Query("CREATE VIEW v1 (j, \"j2\") AS SELECT * FROM t1"));

	REQUIRE_NO_FAIL(con.Query("CREATE VIEW v1 (j, \"j2\") AS SELECT i,i+1 FROM t1"));
	result = con.Query("select j, j2 FROM v1");
	REQUIRE(result->column_count() == 2);
	REQUIRE(CHECK_COLUMN(result, 0, {41, 42, 43}));
	REQUIRE(CHECK_COLUMN(result, 1, {42, 43, 44}));
	REQUIRE_NO_FAIL(con.Query("DROP VIEW v1"));

	REQUIRE_NO_FAIL(con.Query("CREATE VIEW v1 (j, \"j2\") AS SELECT i,i+1, i+2 FROM t1"));
	result = con.Query("select j, j2 FROM v1");
	REQUIRE(result->column_count() == 2);
	REQUIRE(CHECK_COLUMN(result, 0, {41, 42, 43}));
	REQUIRE(CHECK_COLUMN(result, 1, {42, 43, 44}));
	REQUIRE_NO_FAIL(con.Query("DROP VIEW v1"));

	REQUIRE_NO_FAIL(con.Query("CREATE VIEW v1 (j, \"j2\") AS SELECT i,i+1, i+2 as x FROM t1"));
	result = con.Query("select j, j2, x FROM v1");
	REQUIRE(result->column_count() == 3);
	REQUIRE(CHECK_COLUMN(result, 0, {41, 42, 43}));
	REQUIRE(CHECK_COLUMN(result, 1, {42, 43, 44}));
	REQUIRE(CHECK_COLUMN(result, 2, {43, 44, 45}));

	REQUIRE_NO_FAIL(con.Query("DROP VIEW v1"));
}