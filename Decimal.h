#pragma once

#include "NumberObject.h"
#include "Integer.h"

#include <string>
#include <iostream>
#include <vector>

class Decimal: public NumberObject {
public:
	/* Constructors */
	Decimal();
	Decimal(const char*);
	Decimal(const Decimal &);
	/* Getter and setters */
	Decimal& set_value(const char*);
	std::string get_value() const;
	Decimal& copy_value_from(const Decimal &);
	bool null() const;
	bool zero() const;
	/* Overload operators */
	Decimal& operator=(const char*);
	Decimal& operator=(const Decimal &);
	friend std::ostream& operator<<(std::ostream &out, Decimal &decimal);
	friend std::istream& operator>>(std::istream &in, Decimal &decimal);
	friend Decimal operator+(Decimal &a, Decimal &b);
	friend Decimal operator+(Integer &a, Decimal &b);
	friend Decimal operator+(Decimal &a, Integer &b);
	friend Decimal operator-(Decimal &decimal);
	friend Decimal operator-(Decimal &a, Decimal &b);
	friend Decimal operator*(Decimal &a, Decimal &b);
	friend Decimal operator+(Integer &a, Decimal &b);
	friend bool operator<(Decimal &a, Decimal &b);
	friend bool operator>(Decimal &a, Decimal &b);
	friend Decimal operator/(Decimal &a, Decimal &b);
	/* Functions */
	Decimal factorial() const;

	/* Private members */
	bool sign;
	Integer molecular, denominator;
	/* Private methods */
	void simplify(); // Make fraction in lowest terms
};
