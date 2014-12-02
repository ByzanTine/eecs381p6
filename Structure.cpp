#include "Structure.h"

#include <iostream>
#include "Model.h"
#include "Utility.h"
using std::cout;
using std::endl;
using std::string;
Structure::Structure(const std::string& name_, Point location_, double food_):Sim_object(name_)
{
	location = location_;
	food = food_;
	// cout << "Structure " << get_name() << " constructed" << endl;

}


void Structure::broadcast_current_state()
{
	Model::get_instance().notify_location(get_name(), location);
	Model::get_instance().notify_food_carry(get_name(), food);
}


void Structure::deposit(double amount_to_give)
{}

double Structure::withdraw(double amount_to_get)
{
	return 0.0;
}


void Structure::describe() const
{
	cout << get_name() << " at " << location << endl;
}	

Structure::~Structure()
{
	// cout << "Structure " << get_name() << " destructed" << endl;
}

void Structure::add_food(double food_added)
{
	food += food_added;
	Model::get_instance().notify_food_carry(get_name(), food);
	
}