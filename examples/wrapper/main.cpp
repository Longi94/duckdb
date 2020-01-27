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
    }
    
	auto result = con.Query(query);
    fprintf(stdout, "%s", result->ToString().c_str());
}
