#include "Health_view.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;


void Health_view::update_health(const string& name, double health)
{
	update_status(name, health);
}

void Health_view::draw()
{
	cout << "Current " << "Health" << ":" << endl;
	List_view::draw();
}


