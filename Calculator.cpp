#include "stdafx.h"

#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

#include "Integer.h"
#include "Decimal.h"
#include "NumberObject.h"

using namespace std;

void apiDemo();

int main()
{
	apiDemo();
}

void apiDemo() {
	// Overriding Constructor
	Integer i = "12345";
	Decimal d = "0.3 * 3";

	// Overriding operator <<, >>, +, -, *, /
	Integer i2 = "50";
	Decimal d2 = "50.0001";
	cout << i2 + d2 << endl;

	// Number Object
	Integer i3 = "123";
	Decimal d3 = "123.3";
	vector<NumberObject*> nums;
	nums.push_back(&i3);
	nums.push_back(&d3);
	for(const auto& num : nums) cout << *num << endl;

	system("pause");
}
