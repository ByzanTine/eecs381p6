#include "Town_Hall.h"

#include <iostream>

using std::cout;
using std::endl;
using std::string;

const double deduct_rate = 0.1;
const double minimum_threshold = 1.0; 
Town_Hall::Town_Hall (const string& name_, Point location_):Structure(name_, location_)
{
	// cout << "Town_Hall " << get_name() << " constructed" << endl;
}
	
Town_Hall::~Town_Hall()
{

	// cout << "Town_Hall " << get_name() << " destructed" << endl;
}

// deposit adds in the supplied amount
void Town_Hall::deposit(double deposit_amount)
{
	add_food(deposit_amount);
}
// Return whichever is less, the request or (the amount on hand - 10%) (a "tax"),
// but amounts less than 1.0 are not supplied - the amount returned is zero.
// update the amount on hand by subtracting the amount returned.
double Town_Hall::withdraw(double amount_to_obtain)
{
	double cur_food = get_food();
	double amount_available = cur_food * (1 - deduct_rate);
	if (cur_food < minimum_threshold)
		amount_available = 0.0;
	if (amount_to_obtain > amount_available)
	{
		add_food(-amount_available);
		return amount_available;
	}
	else
	{
		add_food(-amount_to_obtain);
		return amount_to_obtain;
	}
}
// output information about the current state
void Town_Hall::describe() const
{
	cout << "Town_Hall ";
	Structure::describe();
	cout << "   Contains " <<  get_food() << endl;
}