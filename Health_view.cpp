#include "Health_view.h"
#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;
using std::pair;
using std::string;

/* class definitions for Health_view */
/* =========================================================*/

void Health_view::update_health(const std::string& name, double health)
{
	health_status[name] = health;
}

// Remove the name and its location; 
void Health_view::update_remove(const std::string& name)
{
	if (health_status.find(name) != health_status.end())
		health_status.erase(name);
}	


void Health_view::draw()
{
	cout << "Current " << "Health" << ":" << endl;
	cout << "--------------" << endl;
	for_each(health_status.begin(), health_status.end(), 
		[](const pair<string, double>& content_pair){
			cout << content_pair.first << ": " << content_pair.second << endl;});
	cout << "--------------" << endl;
}
/* =========================================================*/

