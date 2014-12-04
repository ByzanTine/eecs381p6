#include "List_view.h"

#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;
using std::pair;
using std::string;

// Remove the name and its location; 
void List_view::update_remove(const std::string& name)
{
	if (status.find(name) != status.end())
		status.erase(name);
}	



void List_view::draw()
{	
	cout << "--------------" << endl;
	for_each(status.begin(), status.end(), 
		[](const pair<string, double>& content_pair){
			cout << content_pair.first << ": " << content_pair.second << endl;});
	cout << "--------------" << endl;
}



void List_view::update_status(const std::string& name, double value)
{
	status[name] = value;
}