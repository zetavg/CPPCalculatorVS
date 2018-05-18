#pragma once

#include <iostream>
#include <string>

class NumberObject {
public:
	friend std::ostream& operator<<(std::ostream &out, const NumberObject &number) {
		out << number.get_value();
		return out;
	}
	virtual std::string get_value() const = 0;
	virtual bool null() const = 0;
	virtual bool zero() const = 0;
};
