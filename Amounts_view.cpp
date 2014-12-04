#include "Amounts_view.h"

#include <iostream>

using std::cout;
using std::endl;
using std::string;


void Amounts_view::update_food_carry(const std::string& name, double food_carry)
{
	update_status(name, food_carry);
}	

void Amounts_view::draw()
{
	cout << "Current " << "Amounts" << ":" << endl;
	List_view::draw();
}
