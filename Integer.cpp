#include "stdafx.h"

#include "Integer.h"

#include "parser.h"

#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

/*
* Value calculation helpers
*/

// Adds up two integer_value_t, and return the sum
integer_value_t addup_raw_value(integer_value_t &a, integer_value_t &b) {
	integer_value_t result;
	int a_size = a.size();
	int b_size = b.size();
	int max_size = std::max(a_size, b_size);


	for (int i = 0, carry = 0; i < max_size || carry > 0; ++i) {
		int digit = carry;
		if (i < a_size) digit += a.at(i);
		if (i < b_size) digit += b.at(i);
		carry = digit / 10;
		digit = digit % 10;
		result.push_back(digit);
	}

	return result;
}

// Subtracts two integer_value_t (a - b), and return the remaining value
//
// The first value must be larger then the second one. It is the caller's
// responsibility to ensure the inputs are valid, and to deal with the leading
// zeros of the result.
integer_value_t substractdown_raw_value(integer_value_t &a, integer_value_t &b) {
	integer_value_t result;
	int a_size = a.size();
	int b_size = b.size();

	for (int i = 0, carry = 0; i < a_size; ++i) {
		int digit = a.at(i) + carry;
		carry = 0;
		if (i < b_size) digit -= b.at(i);
		if (digit < 0) {
			digit += 10;
			carry = -1;
		}
		result.push_back(digit);
	}

	return result;
}

// Multiply two integer_value_t (a * b)
integer_value_t multiply_raw_value(integer_value_t &a, integer_value_t &b) {
	integer_value_t result;
	int a_size = a.size();
	int b_size = b.size();

	for (int i = 0, carry = 0; i < a_size; ++i) {
		for (int j = 0; j < b_size; ++j) {
			unsigned long p = i + j;
			integer_digit_t digit = a.at(i) * b.at(j) + carry;
			if (result.size() > p) digit += result.at(p);
			carry = digit / 10;
			digit = digit % 10;
			if (result.size() > p) {
				result.at(p) = digit;
			}
			else {
				result.push_back(digit);
			}
		}

		if (carry > 0) {
			result.push_back(carry);
			carry = 0;
		}
	}

	return result;
}

// Compares two integer_value_t
//
// Returns 1 if the first one is larger then the second, -1 if smaller,
// or 0 if same.
int compare_raw_value(integer_value_t &a, integer_value_t &b) {
	int a_size = a.size();
	int b_size = b.size();

	if (a_size > b_size) {
		return 1;
	}
	else if (b_size > a_size) {
		return -1;
	}
	else {
		for (
			auto a_i = a.rbegin(),
			b_i = b.rbegin();
			a_i != a.rend() && b_i != b.rend();
			++a_i, ++b_i
			) {
			if (*a_i > *b_i) {
				return 1;
			}
			else if (*b_i > *a_i) {
				return -1;
			}
		}

		return 0;
	}
}

integer_value_t arrange_raw_value(integer_value_t &v) {
	while (v.size() > 1 && v.back() == 0) {
		v.pop_back();
	}

	return v;
}

/*
* Constructors
*/

Integer::Integer() {
	set_value("null");
}

Integer::Integer(const char *number) {
	auto d = parse(number);
	copy_value_from(d.molecular / d.denominator);
}


Integer::Integer(const Integer &integer) {
	copy_value_from(integer);
}

/*
* Getter and setters
*/

Integer& Integer::set_value(const char *number) {
	sign = true;
	value.clear();

	// TODO: Support setting a value via expression

	if (strcmp(number, "null") != 0) {
		int last;
		for (last = 0; number[last] != '\0'; ++last) continue;

		for (int i = last - 1; i >= 0; --i) {
			if (number[i] == '-') {
				sign = false;
			}
			else {
				int n = number[i] - 48;
				if (n < 0 && n > 9) continue;
				value.push_back(n);
			}
		}

		arrange();
	}

	return *this;
}

std::string Integer::get_value() const {
	if (value.empty()) {
		return "null";
	}
	else {
		std::string str;

		if (!sign) str.push_back('-');

		for (auto it = value.rbegin(); it != value.rend(); ++it) {
			str.push_back(*it + 48);
		}

		return str;
	}
}

Integer& Integer::copy_value_from(const Integer &reference) {
	sign = reference.sign;
	value = integer_value_t(reference.value);
	return *this;
}

bool Integer::null() const {
	return value.empty();
}

bool Integer::zero() const {
	return value.size() == 1 && value.at(0) == 0;
}

/*
* Overload operators
*/

Integer& Integer::operator=(const char *number) {
	auto d = parse(number);
	copy_value_from(d.molecular / d.denominator);
	return *this;
}

Integer& Integer::operator=(const Integer &integer) {
	copy_value_from(integer);
	return *this;
}

std::ostream& operator<<(std::ostream &out, const Integer &integer) {
	out << integer.get_value();
	return out;
}

std::istream& operator>>(std::istream &in, Integer &integer) {
	std::string value;
	in >> value;
	integer.set_value(value.c_str());

	return in;
}

Integer operator+(Integer &a, Integer &b) {
	if (a.sign == b.sign) {
		// simply add the values and set the sign
		Integer result;
		result.sign = a.sign;
		result.value = addup_raw_value(a.value, b.value);
		return result;

	}
	else {
		// we need to compare the values and perform an substraction
		Integer *positive_integer, *negative_integer;

		// find the positive and negative integer
		if (a.sign && !b.sign) {
			positive_integer = &a;
			negative_integer = &b;
		}
		else {
			positive_integer = &b;
			negative_integer = &a;
		}

		// compare the values to decide what to do
		int comparing_result = compare_raw_value(positive_integer->value, negative_integer->value);

		if (comparing_result > 0) {
			// the positive value is larger then the negative value
			Integer result;
			result.sign = true;
			result.value = substractdown_raw_value(positive_integer->value, negative_integer->value);
			result.arrange();
			return result;
		}
		else if (comparing_result < 0) {
			// the negative value is larger then the positive value
			Integer result;
			result.sign = false;
			result.value = substractdown_raw_value(negative_integer->value, positive_integer->value);
			result.arrange();
			return result;
		}
		else {
			// the two values are same
			Integer result("0");
			return result;
		}
	}
}

Integer operator-(Integer &integer) {
	Integer result(integer);
	result.sign = !integer.sign;
	return result;
}

Integer operator-(Integer &a, Integer &b) {
	Integer negative_b = -b;
	return (a + negative_b);
}

Integer operator*(Integer &a, Integer &b) {
	Integer result;
	result.value = multiply_raw_value(a.value, b.value);
	result.sign = a.sign == b.sign;
	return result;
}

Integer operator/(Integer &a, const Integer &b) {
	integer_value_t aValue(a.value), bValue(b.value);

	integer_value_t oneValue;
	oneValue.push_back(1);
	integer_value_t resultValue;
	resultValue.push_back(0);

	while (compare_raw_value(aValue, bValue) >= 0) {
		resultValue = addup_raw_value(resultValue, oneValue);
		aValue = substractdown_raw_value(aValue, bValue);
		aValue = arrange_raw_value(aValue);
	}

	Integer result;
	result.value = resultValue;

	return result;
}

Integer operator%(Integer &a, const Integer &b) {
	integer_value_t aValue(a.value), bValue(b.value);

	while (compare_raw_value(aValue, bValue) >= 0) {
		aValue = substractdown_raw_value(aValue, bValue);
		aValue = arrange_raw_value(aValue);
	}

	Integer result;
	result.value = aValue;

	return result;
}

bool operator>(Integer &a, Integer &b) {
	if (a.sign == b.sign) {
		return (!a.sign) ^ (compare_raw_value(a.value, b.value) == 1);
	}
	else {
		return a.sign;
	}
}

bool operator>=(Integer &a, Integer &b) {
	int result = compare_raw_value(a.value, b.value);
	if (a.sign == b.sign) {
		return (result == 0) || (!a.sign) ^ result == 1;
	}
	else {
		return a.sign;
	}
}

bool operator<(Integer &a, Integer &b) {
	if (a.sign == b.sign) {
		return (a.sign) ^ compare_raw_value(a.value, b.value) == -1;
	}
	else {
		return b.sign;
	}
}

bool operator<=(Integer &a, Integer &b) {
	int result = compare_raw_value(a.value, b.value);
	if (a.sign == b.sign) {
		return (result == 0) || (a.sign) ^ result == 1;
	}
	else {
		return b.sign;
	}
}

bool operator==(Integer &a, Integer &b) {
	return a.sign == b.sign && compare_raw_value(a.value, b.value) == 0;
}

/*
* Private methods
*/

void Integer::arrange() {
	value = arrange_raw_value(value);
}
