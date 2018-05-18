#include "stdafx.h"

#include "Decimal.h"

#include <string>
#include <cstring>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include "Integer.h"

#define DECIMAL_APPROXIMATE_DENOMINATOR_MAX_DIGITS 100

/*
* Constructors
*/

Decimal::Decimal() {
	set_value("null");
}

Decimal::Decimal(const char *number) {
	set_value(number);
}


Decimal::Decimal(const Decimal &decimal) {
	copy_value_from(decimal);
}

/*
* Getter and setters
*/

Decimal& Decimal::set_value(const char *number) {
	// Initialize
	sign = true;
	molecular.set_value("null");
	denominator.set_value("1");

	// Do nothing if the value should be set to null
	if (strcmp(number, "null") == 0) return *this;

	// Get the length of the number string
	int numberLength;
	for (numberLength = 0; number[numberLength] != '\0'; ++numberLength) continue;

	// 0: reading value,
	// 1: reading digits back of the floating point of an decimal,
	// 2: reading denominator of an fraction
	int state = 0;

	std::string molecularString = "", denominatorString = "1";

	// Read through the number string from the back
	for (int i = 0; number[i] != '\0'; ++i) {
		switch (number[i]) {
			case '-':
				sign = !sign;
				continue;

			case '.':
				state = 1;
				continue;

			case '/':
				state = 2;
				denominatorString.clear();
				continue;

			default:
				int n = number[i];
				if (n < 48 && n > 57) continue;

				if (state == 2) {
					denominatorString.push_back(n);
				}
				else {
					molecularString.push_back(n);
				}

				if (state == 1) denominatorString.push_back('0');
		}
	}

	switch (state) {
		case 0:
			molecular.set_value(molecularString.c_str());
			break;
		default:
			molecular.set_value(molecularString.c_str());
			denominator.set_value(denominatorString.c_str());
			break;
	}

	simplify();

	return *this;
}

std::string Decimal::get_value() const {
	if (molecular.null()) {
		return "null";
	}
	else if (molecular.get_value() == "0") {
		return "0";
	}

	Integer rem(molecular);

	std::string output = (rem / denominator).get_value();
	rem = rem % denominator;

	if (!rem.zero()) {
		output.push_back('.');
		Integer ten = "10";
		int i = DECIMAL_APPROXIMATE_DENOMINATOR_MAX_DIGITS;

		while (!rem.zero()) {
			--i;
			if (i < 0) break;

			rem = rem * ten;
			output = output + (rem / denominator).get_value();
			rem = rem % denominator;
		}
	}

	return output;
}

Decimal& Decimal::copy_value_from(const Decimal &reference) {
	sign = reference.sign;
	molecular = Integer(reference.molecular);
	denominator = Integer(reference.denominator);
	return *this;
}

// Decimal& Decimal::copy_value_from(const Integer &reference) {
// 	sign = reference.sign;
// 	molecular = Integer(reference.molecular);
// 	denominator = Integer("1");
// 	return *this;
// }

bool Decimal::null() const {
	return molecular.null();
}

bool Decimal::zero() const {
	return molecular.zero();
}

/*
* Overload operators
*/

Decimal& Decimal::operator=(const char *number) {
	set_value(number);
	return *this;
}

Decimal& Decimal::operator=(const Decimal &decimal) {
	copy_value_from(decimal);
	return *this;
}

std::ostream& operator<<(std::ostream &out, Decimal &decimal) {
	out << decimal.get_value();
	return out;
}

std::istream& operator>>(std::istream &in, Decimal &decimal) {
	std::string str;
	in >> str;
	decimal.set_value(str.c_str());

	return in;
}

Decimal operator+(Decimal &a, Decimal &b) {
	// Make the denominator same
	Integer result_denominator = a.denominator * b.denominator;

	// Calculate the values after multiplication
	Integer a_molecular_after_multiplication = a.molecular * b.denominator;
	Integer b_molecular_after_multiplication = b.molecular * a.denominator;

	if (a.sign == b.sign) {
		// simply add the values and set the sign
		Decimal result;
		result.sign = a.sign;
		result.molecular = a.molecular + b.molecular;
		result.denominator = result_denominator;
		result.simplify();
		return result;
	}
	else {
		Decimal result;
		result.sign = a.sign;
		result.molecular = a.molecular - b.molecular;
		result.denominator = result_denominator;
		result.simplify();
		return result;
	}
}

Decimal operator+(Integer &a, Decimal &b) {
	Decimal decimalA;
	Integer decimalAMolecular(a);
	decimalAMolecular.sign = true;
	decimalA.sign = a.sign;
	decimalA.molecular = decimalAMolecular;

	return decimalA + b;
}

Decimal operator+(Decimal &a, Integer &b) {
	Decimal decimalB;
	Integer decimalBMolecular(b);
	decimalBMolecular.sign = true;
	decimalB.sign = b.sign;
	decimalB.molecular = decimalBMolecular;

	return a + decimalB;
}

Decimal operator-(Decimal &decimal) {
	Decimal result(decimal);
	result.sign = !decimal.sign;
	return result;
}

Decimal operator-(Decimal &a, Decimal &b) {
	Decimal negative_b = -b;
	return (a + negative_b);
}

Decimal operator*(Decimal &a, Decimal &b) {
	Decimal result;
	result.molecular = a.molecular * b.molecular;
	result.denominator = a.denominator * b.denominator;
	result.sign = a.sign == b.sign;
	result.simplify();
	return result;
}

Decimal operator/(Decimal &a, Decimal &b) {
	Decimal result;
	result.molecular = a.molecular * b.denominator;
	result.denominator = a.denominator * b.molecular;
	result.sign = a.sign == b.sign;
	result.simplify();
	return result;
}

/*
* Private methods
*/

void Decimal::simplify() {
	// TODO: Impl.
}
