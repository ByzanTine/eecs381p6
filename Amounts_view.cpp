#include "Amounts_view.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Geometry.h"
#include "Utility.h"

using std::cout;
using std::endl;
using std::string;
using std::setw;
using std::pair;
using std::vector;


void Amounts_view::update_food_carry(const std::string& name, double food_carry)
{
	update_status(name, food_carry);
}	

void Amounts_view::draw()
{
	cout << "Current " << "Amounts" << ":" << endl;
	List_view::draw();
}
