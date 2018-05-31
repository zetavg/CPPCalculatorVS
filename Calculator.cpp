#include "stdafx.h"

#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

#include "Integer.h"
#include "Decimal.h"
#include "NumberObject.h"
#include "parser.h"

using namespace std;

void apiDemo();

int main()
{
	// apiDemo();

	string input;

	while (getline(cin, input)) {
		cout << parse(input) << endl;
	}
}

void apiDemo() {
	cout << "Overriding Constructor" << endl;
	Integer i = "12345";
	Decimal d = "0.3 * 3";
	cout << i << endl;
	cout << d << endl;
	Integer i_2 = "12 * 3 / 5";
	Decimal d_2 = "0.3 * 3 + ((1 + 1) * 2)";
	cout << i_2 << endl;
	cout << d_2 << endl;
	Integer i_3 = "1/0";
	Decimal d_3 = "0";
	cout << i_3 << endl;
	cout << d_3 << endl;

	cout << "Overriding operator <<, >>, +, -, *, /" << endl;
	Integer i2 = "50";
	Decimal d2 = "50.0001";
	cout << i2 + d2 << endl;

	cout << "Number Object" << endl;
	Integer i3 = "123";
	Decimal d3 = "123.3";
	vector<NumberObject*> nums;
	nums.push_back(&i3);
	nums.push_back(&d3);
	for (const auto& num : nums) cout << *num << endl;

	system("pause");
}
