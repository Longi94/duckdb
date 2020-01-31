#include "duckdb.hpp"
#include <iostream>

using namespace std;
using namespace duckdb;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }
	DuckDB db(argv[1]);
	Connection con(db);
    
    string query;
    string line;
    
    while (getline(cin, line)) {
        query.append(line);
        query.append("\n");
    }
    
	auto result = con.Query(query);
	if (result.success) {
		fprintf(stdout, "%s", result->ToString().c_str());
		return 0;
	} else {
		fprintf(stderr, "%s", result->ToString().c_str());
		return 1;
	}
}
