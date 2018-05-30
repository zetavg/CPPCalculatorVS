#include "stdafx.h"

#include "parser.h"

#include "Decimal.h"

bool isNumber(const char c) {
	return (c >= '0' && c <= '9');
}

bool isAlpha(const char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isDigit(const char c) {
	return isNumber(c) || c == '.';
}

bool isParenthesis(const char c) {
	return (
		c == '(' ||
		c == ')'
	);
}

bool isOperator(const char c) {
	return (
		c == '^' ||
		c == '+' ||
		c == '-' ||
		c == '*' ||
		c == '/'
	);
}

bool isSign(const char c) {
	return (
		c == '+' ||
		c == '-'
	);
}

bool isFactorial(const char c) {
	return (
		c == '!'
	);
}

bool isNotation(const char c) {
	return isParenthesis(c) || isOperator(c) || isSign(c) || isFactorial(c);
}

bool isNumber(const std::string &s) {
	char c = s.at(0);
	return isNumber(c);
}

bool isVariable(const std::string &s) {
	char c = s.at(0);
	return isAlpha(c);
}

bool isOperator(const std::string &s) {
	char c = s.at(0);
	return isOperator(c);
}

bool isSign(const std::string &s) {
	char c = s.at(0);
	return isSign(c);
}

bool isFactorial(const std::string &s) {
	char c = s.at(0);
	return isFactorial(c);
}

char notationOf(const std::string &s) {
	char c = s.at(0);
	return c;
}

int notationWeight(char n) {
	switch (n) {
		case '(':
		case ')':
			return 1;
		case '!':
			return 2;
		case '^':
			return 3;
		case '*':
		case '/':
			return 5;
		case '+':
		case '-':
			return 6;
	}
}

int balancingParenthesis(std::vector<std::string> tokens, int start, int end) {
	if (notationOf(tokens.at(start)) != '(') return -1;
	int stage = 0;
	for (int i = start + 1; i <= end; ++i) {
		if (notationOf(tokens.at(i)) == '(') ++stage;
		else if (notationOf(tokens.at(i)) == ')') --stage;

		if (stage < 0) return i;
	}
	return -1;
}

std::vector<std::string> lex(std::string input) {
	std::vector<std::string> tokens;

	for (auto it = input.begin(); it != input.end(); ++it) {
		if (*it == ' ') continue;

		if (isNumber(*it)) { // Digit mode
			std::string degit;
			degit.push_back(*it);
			while (it + 1 != input.end() && isDigit(*(it + 1))) {
				++it;
				degit.push_back(*it);
			}
			tokens.push_back(degit);
		}
		else if (isAlpha(*it)) { // Token mode
			std::string token;
			token.push_back(*it);
			while (it + 1 != input.end() && isDigit(*(it + 1))) {
				++it;
				token.push_back(*it);
			}
			tokens.push_back(token);
		}
		else if (isNotation(*it)) {
			std::string sign;
			sign.push_back(*it);
			tokens.push_back(sign);
		}
		else {
			// TODO: Handle error
		}
	}

	return tokens;
}

Decimal eval(std::vector<std::string> tokens, int start, int end) {
	if (start > end) {
		Decimal result;
		result.set_value("null");
		return result;
	}

	// expr :

	// number
	if (start == end && isNumber(tokens.at(start))) {
		Decimal result;
		result.set_value(tokens.at(start).c_str());
		return result;
	}

	// (expr)
	if (notationOf(tokens.at(start)) == '(' && balancingParenthesis(tokens, start, end) == end) {
		return eval(tokens, start + 1, end - 1);
	}

	// expr op expr
	int splitPoint = -1;
	int splitPointNW = 0;
	if (end - start >= 2) {

		int i = start;

		while (i < end) {
			if (notationOf(tokens.at(i)) == '(') {
				i = balancingParenthesis(tokens, i, end) + 1;

				if (i == 0) {
					Decimal result;
					result.set_value("null");
					return result;
				}

				continue;
			}

			if (isOperator(tokens.at(i))) {
				if (i == start || i == end) {
					++i;
					continue;
				}
				if (isOperator(tokens.at(i - 1))) {
					++i;
					continue;
				}

				char op = notationOf(tokens.at(i));

				if (notationWeight(op) > splitPointNW) {
					splitPointNW = notationWeight(op);
					splitPoint = i;
				}
				else if (notationWeight(op) == splitPointNW) {
					switch (splitPointNW) {
						case 3:
							break;
						default:
							splitPoint = i;
					}
				}
			}

			++i;
		}
	}

	// sign expr
	if (isSign(tokens.at(start)) && splitPointNW < 4) {
		Decimal result;
		result = eval(tokens, start + 1, end);
		char sign = notationOf(tokens.at(start));
		if (sign == '-') result.sign = !result.sign;
		return result;
	}

	// expr !
	if (isFactorial(tokens.at(end)) && splitPointNW < 2) {
		Decimal result;
		result = eval(tokens, start, end - 1);
		// TODO
		return result;
	}

	// expr op expr
	if (splitPoint != -1) {
		Decimal lhs = eval(tokens, start, splitPoint - 1);
		Decimal rhs = eval(tokens, splitPoint + 1, end);

		char op = notationOf(tokens.at(splitPoint));

		switch (op) {
			case '+':
				return lhs + rhs;
			case '-':
				return lhs - rhs;
			case '*':
				return lhs * rhs;
			case '/':
				return lhs / rhs;
			case '^':
				// TODO
				return lhs + rhs;
		}
	}

	Decimal result;
	result.set_value("null");
	return result;
}

Decimal parse(std::string expr) {
	auto tokens = lex(expr);
	return eval(tokens, 0, tokens.size() - 1);
}

		// if (sign = '-') {
		// 	Decimal result;
		// 	result.set_value("-" + tokens.at(end));
		// 	return result;
		// }

		// Decimal result;
		// result.set_value("-" + tokens.at(end));
		// return result;
