#include "Health_view.h"
#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;
using std::pair;
using std::string;


void Health_view::update_health(const std::string& name, double health)
{
	update_status(name, health);
}

void Health_view::draw()
{
	cout << "Current " << "Health" << ":" << endl;
	List_view::draw();
}


