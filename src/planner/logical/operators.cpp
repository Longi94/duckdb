
#include "planner/logical/operators.hpp"

using namespace duckdb;
using namespace std;

// Split a set of predicates separate by AND statements
// These are the predicates that are safe to push down because all of them MUST be true
void LogicalFilter::SplitPredicates(std::unique_ptr<AbstractExpression> expression) {
	if (expression->GetExpressionType() == ExpressionType::CONJUNCTION_AND) {
		// Traverse down the expression tree along conjunction
		for(auto& child : expression->children) {
			SplitPredicates(move(child));
		}
	} else {
		// Find an expression that is the child of conjunction expression
		expressions.push_back(move(expression));
	}
}

LogicalFilter::LogicalFilter(unique_ptr<AbstractExpression> expression) :
	LogicalOperator(LogicalOperatorType::FILTER) {
	SplitPredicates(move(expression));
}
