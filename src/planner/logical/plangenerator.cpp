
#include "parser/expression/basetableref_expression.hpp"
#include "parser/expression/join_expression.hpp"
#include "parser/expression/subquery_expression.hpp"

#include "planner/logical/plangenerator.hpp"

using namespace duckdb;
using namespace std;

void LogicalPlanGenerator::Visit(SelectStatement &statement) {
	if (statement.from_table) {
		// SELECT with FROM
		statement.from_table->Accept(this);
	} else {
		// SELECT without FROM, add dummy GET
		root = make_unique<LogicalGet>();
	}

	if (statement.where_clause) {
		statement.where_clause->Accept(this);

		auto filter = make_unique<LogicalFilter>(move(statement.where_clause));
		filter->children.push_back(move(root));
		root = move(filter);
	}

	if (statement.HasAggregation()) {
		auto aggregate = make_unique<LogicalAggregateAndGroupBy>();
		if (statement.HasGroup()) {
			// have to add group by columns
			aggregate->groups = move(statement.groupby.groups);
		}
		aggregate->children.push_back(move(root));
		root = move(aggregate);

		if (statement.HasHaving()) {
			statement.groupby.having->Accept(this);

			auto having = make_unique<LogicalFilter>(move(statement.groupby.having));
			having->children.push_back(move(root));
			root = move(having);
		}
	}

	if (statement.select_distinct) {
		auto distinct = make_unique<LogicalDistinct>();
		distinct->children.push_back(move(root));
		root = move(distinct);
	}
	if (statement.HasOrder()) {
		auto order = make_unique<LogicalOrder>();
		order->children.push_back(move(root));
		root = move(order);
	}
	if (statement.HasLimit()) {
		auto limit = make_unique<LogicalLimit>(statement.limit.limit, statement.limit.offset);
		limit->children.push_back(move(root));
		root = move(limit);
	}
}

void LogicalPlanGenerator::Visit(BaseTableRefExpression &expr) {
	auto table = catalog.GetTable(expr.schema_name, expr.table_name);
	auto get_table = make_unique<LogicalGet>(table);
	if (root) get_table->children.push_back(move(root));
	root = move(get_table);
}

void LogicalPlanGenerator::Visit(JoinExpression &expr) {
	throw NotImplementedException("Joins not implemented yet!");
}

void LogicalPlanGenerator::Visit(SubqueryExpression &expr) {
	throw NotImplementedException("Subquery not implemented yet!");
}
