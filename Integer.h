#pragma once

#include "NumberObject.h"

#include <string>
#include <iostream>
#include <vector>

typedef char integer_digit_t;
typedef std::vector<integer_digit_t> integer_value_t;

class Integer: public NumberObject {
public:
	/* Constructors */
	Integer();
	Integer(const char*);
	Integer(const Integer &);
	/* Getter and setters */
	Integer& set_value(const char*);
	std::string get_value() const;
	Integer& copy_value_from(const Integer &);
	bool null() const;
	bool zero() const;
	/* Overload operators */
	Integer& operator=(const char*);
	Integer& operator=(const Integer &);
	friend std::istream& operator>>(std::istream &in, Integer &integer);
	friend Integer operator+(Integer &a, Integer &b);
	friend Integer operator-(Integer &integer);
	friend Integer operator-(Integer &a, Integer &b);
	friend Integer operator*(Integer &a, Integer &b);
	friend Integer operator/(Integer &a, const Integer &b);
	friend Integer operator%(Integer &a, const Integer &b);
	friend bool operator>(Integer &a, Integer &b);
	friend bool operator>=(Integer &a, Integer &b);
	friend bool operator<(Integer &a, Integer &b);
	friend bool operator<=(Integer &a, Integer &b);
	friend bool operator==(Integer &a, Integer &b);

	/* Private members */
	bool sign;
	integer_value_t value;
	/* Private methods */
	void arrange(); // Remove leading zeros from the value
};
