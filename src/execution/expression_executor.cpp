
#include "execution/expression_executor.hpp"
#include "execution/vector/vector_operations.hpp"

#include "common/exception.hpp"

#include "parser/expression/aggregate_expression.hpp"
#include "parser/expression/basetableref_expression.hpp"
#include "parser/expression/columnref_expression.hpp"
#include "parser/expression/comparison_expression.hpp"
#include "parser/expression/conjunction_expression.hpp"
#include "parser/expression/constant_expression.hpp"
#include "parser/expression/crossproduct_expression.hpp"
#include "parser/expression/function_expression.hpp"
#include "parser/expression/join_expression.hpp"
#include "parser/expression/operator_expression.hpp"
#include "parser/expression/subquery_expression.hpp"
#include "parser/expression/tableref_expression.hpp"

using namespace duckdb;
using namespace std;

void ExpressionExecutor::Execute(AbstractExpression *expr, Vector &result) {
	expr->Accept(this);

	if (result.type != vector.type) {
		throw NotImplementedException("FIXME: cast");
	}
	vector.Move(result);
}

void ExpressionExecutor::Merge(AbstractExpression *expr, Vector &result) {
	if (result.type != TypeId::BOOLEAN) {
		throw NotImplementedException("Expected a boolean!");
	}
	expr->Accept(this);

	if (vector.type != TypeId::BOOLEAN) {
		throw NotImplementedException("Expected a boolean!");
	}
	VectorOperations::And(vector, result, result);
}

void ExpressionExecutor::Visit(AggregateExpression &expr) {
	throw NotImplementedException("");
}

void ExpressionExecutor::Visit(BaseTableRefExpression &expr) {
	throw NotImplementedException("");
}

void ExpressionExecutor::Visit(ColumnRefExpression &expr) {
	if (expr.index == (size_t)-1) {
		throw Exception("Column Reference not bound!");
	}
	vector.Reference(*chunk.data[expr.index].get());
}

void ExpressionExecutor::Visit(ComparisonExpression &expr) {
	Vector l, r, result;
	expr.children[0]->Accept(this);
	vector.Move(l);
	expr.children[1]->Accept(this);
	vector.Move(r);
	vector.Resize(std::max(l.count, r.count), TypeId::BOOLEAN);
	switch(expr.type) {
	case ExpressionType::COMPARE_EQUAL:
		VectorOperations::Compare(l, r, vector);
		break;
	case ExpressionType::COMPARE_NOTEQUAL:
		throw NotImplementedException("Unimplemented compare: COMPARE_NOTEQUAL");
	case ExpressionType::COMPARE_LESSTHAN:
		throw NotImplementedException("Unimplemented compare: COMPARE_LESSTHAN");
	case ExpressionType::COMPARE_GREATERTHAN:
		VectorOperations::GreaterThan(l, r, vector);
		break;
	case ExpressionType::COMPARE_LESSTHANOREQUALTO:
		throw NotImplementedException("Unimplemented compare: COMPARE_LESSTHANOREQUALTO");
	case ExpressionType::COMPARE_GREATERTHANOREQUALTO:
		throw NotImplementedException("Unimplemented compare: COMPARE_GREATERTHANOREQUALTO");
	case ExpressionType::COMPARE_LIKE:
		throw NotImplementedException("Unimplemented compare: COMPARE_LIKE");
	case ExpressionType::COMPARE_NOTLIKE:
		throw NotImplementedException("Unimplemented compare: COMPARE_NOTLIKE");
	case ExpressionType::COMPARE_IN:
		throw NotImplementedException("Unimplemented compare: COMPARE_IN");
	case ExpressionType::COMPARE_DISTINCT_FROM:
		throw NotImplementedException("Unimplemented compare: COMPARE_DISTINCT_FROM");
	default:
		throw NotImplementedException("Unknown result!");
	}
}

void ExpressionExecutor::Visit(ConjunctionExpression &expr) {
	throw NotImplementedException("");
}

void ExpressionExecutor::Visit(ConstantExpression &expr) {
	Vector v(expr.value);
	v.Move(vector);
}

void ExpressionExecutor::Visit(CrossProductExpression &expr) {
	throw NotImplementedException("");
}

void ExpressionExecutor::Visit(FunctionExpression &expr) {
	throw NotImplementedException("");
}

void ExpressionExecutor::Visit(JoinExpression &expr) {
	throw NotImplementedException("");
}

void ExpressionExecutor::Visit(OperatorExpression &expr) {
	if (expr.children.size() == 1) {
		Vector l;
		expr.children[0]->Accept(this);
		vector.Move(l);

	} else if (expr.children.size() == 2) {
		Vector l, r;
		expr.children[0]->Accept(this);
		vector.Move(l);
		expr.children[1]->Accept(this);
		vector.Move(r);

		vector.Resize(std::max(l.count, r.count));

		switch (expr.type) {
		case ExpressionType::OPERATOR_PLUS:
			VectorOperations::Add(l, r, vector);
			break;
		case ExpressionType::OPERATOR_MINUS:
			VectorOperations::Subtract(l, r, vector);
			break;
		case ExpressionType::OPERATOR_MULTIPLY:
			VectorOperations::Multiply(l, r, vector);
			break;
		case ExpressionType::OPERATOR_DIVIDE:
			VectorOperations::Divide(l, r, vector);
			break;
		default:
			throw NotImplementedException("operator");
		}
	} else {
		throw NotImplementedException("operator");
	}
}

void ExpressionExecutor::Visit(SubqueryExpression &expr) {
	throw NotImplementedException("");
}

void ExpressionExecutor::Visit(TableRefExpression &expr) {
	throw NotImplementedException("");
}