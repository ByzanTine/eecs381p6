#include "Local_view.h"
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

const Point default_origin(-10.0, -10.0);
const double loca_view_scale = 2.0;
const int loca_view_size = 9;


Local_view::Local_view(const string& name):
	Grid_view(default_origin, loca_view_scale, loca_view_size)
{
	center_object_name = name;
	// No need to set the origin, location update will do the work
}

// override to also update the center
void Local_view::update_location(const string& name, Point location)
{
	Grid_view::update_location(name, location);

	if (name == center_object_name)
		callibrate_origin(location);
}


// prints out the current map
void Local_view::draw()
{
	cout << "Local view for: " << center_object_name << endl;
	// Init the display map
	vector< vector<string> >  display_matrix
		(loca_view_size, vector<string>(loca_view_size, ". "));
	
	// Assign the values first
	fill_display_matrix(display_matrix);
		
	draw_grid(display_matrix);
}
/* =========================================================*/