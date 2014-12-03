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
// prints out the current map

/* class definitions for Amounts view */
/* =========================================================*/
// Save the supplied food_carry data for future use in a draw() call
void Amounts_view::update_food_carry(const std::string& name, double food_carry)
{
	carry_status[name] = food_carry;
}	

// Remove the name and its location; 
void Amounts_view::update_remove(const std::string& name)
{
	if (carry_status.find(name) != carry_status.end())
		carry_status.erase(name);
}	

void Amounts_view::draw()
{
	cout << "Current " << "Amounts" << ":" << endl;
	cout << "--------------" << endl;
	for_each(carry_status.begin(), carry_status.end(), 
		[](const pair<string, double>& content_pair){
			cout << content_pair.first << ": " << content_pair.second << endl;});
	cout << "--------------" << endl;
}
/* =========================================================*/