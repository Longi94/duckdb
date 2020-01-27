#include "duckdb.hpp"

using namespace duckdb;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        return 1;
    }
	DuckDB db(argv[1]);
	Connection con(db);
	auto result = con.Query(argv[2]);
    fprintf(stdout, "%s", result->ToString().c_str());
}
