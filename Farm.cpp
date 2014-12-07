#include "Farm.h"

#include <iostream>



using std::cout;
using std::endl;
using std::string;

const double init_food = 50.0;
const double food_increase_rate = 2.0;
Farm::Farm (const string& name_, Point location_):Structure(name_, location_, init_food)
{}


Farm::~Farm()
{}


double Farm::withdraw(double amount_to_get)
{
	double cur_food = get_food();
	if (amount_to_get > cur_food)
	{
		int amount_available = cur_food;
		add_food(-amount_available);
		return amount_available;
	}
	else
	{
		add_food(-amount_to_get);
		return amount_to_get;
	}
}

void Farm::describe() const
{
	cout << "Farm ";
	Structure::describe();
	cout << "   Food available: " << get_food() << endl;
}

void Farm::update() 
{
	add_food(food_increase_rate);
	cout << "Farm " << get_name() << " now has " << get_food() << endl;
}