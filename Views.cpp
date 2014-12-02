#include "Views.h"

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

const Point default_origin(-10.0, -10.0);
const double default_scale = 2.0;
const int default_size = 25;
const double loca_view_scale = 2.0;
const int loca_view_size = 9;
const int max_map_size = 30;

Grid_view::Grid_view(Point origin_, double scale_, int size_)
{
	origin = origin_;
	scale = scale_;
	size = size_;
}
void Grid_view::update_location(const std::string& name, Point location)
{
	name_location_map[name] = location;
}

// Remove the name and its location; no error if the name is not present.
void Grid_view::update_remove(const std::string& name)
{
	if (name_location_map.find(name) != name_location_map.end())
		name_location_map.erase(name);
}
// modify the display parameters
// if the size is out of bounds will throw Error("New map size is too big!")
// or Error("New map size is too small!")
void Grid_view::set_size(int size_)
{
	if (size_ > 30)
		throw Error("New map size is too big!");
	if (size_ <= 6)
		throw Error("New map size is too small!");
	size = size_;
}

// If scale is not postive, will throw Error("New map scale must be positive!");
void Grid_view::set_scale(double scale_)
{
	if (scale_ <= 0)
		throw Error("New map scale must be positive!");
	else
		scale = scale_;
}

// any values are legal for the origin
void Grid_view::set_origin(Point origin_)
{
	origin = origin_;
}

void Grid_view::set_defaults()
{
	origin = default_origin;
  	scale = default_scale;
  	size = default_size;

}


// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool Grid_view::get_subscripts(int &ix, int &iy, Point location)
{
	// adjust with origin and scale
	Cartesian_vector subscripts = (location - origin) / scale;
	// truncate coordinates to integer after taking the floor
	// floor function will return the largest integer smaller than the supplied value
	// even for negative values, so -0.05 => -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
		}
	else
		return true;
}

void Grid_view::print_grid_header()
{
	cout << "Display size: " << size << ", scale: " << scale << ", origin: " << origin << endl;
}

void Grid_view::fill_display_matrix(std::vector< std::vector<std::string> >& display_matrix)
{
	for (const pair<string, Point>& name_loc_pair : name_location_map)
	{
		int ix;
		int iy;
		string name = name_loc_pair.first;
		if (get_subscripts(ix, iy, name_loc_pair.second))
		{
			if (display_matrix[iy][ix] != ". ")
				display_matrix[iy][ix] = "* ";
			else
				display_matrix[iy][ix] = name.substr(0, 2);
		}
	}
	
}

void Grid_view::show_outsiders()
{
	bool first = true; // flag for out 
	for (const pair<string, Point>& name_loc_pair : name_location_map)
	{
		int ix;
		int iy;
		string name = name_loc_pair.first;
		if (!get_subscripts(ix, iy, name_loc_pair.second))
		{
			if (first)
				first = false; // turn down if got one outside
			else
				cout << ", ";
			cout << name;
		}
	}
	if (!first)
		cout << " outside the map" << endl;
}


void Grid_view::draw_grid(const std::vector< std::vector<std::string> >& display_matrix)
{
	int old_precision = cout.precision();
	cout.precision(0);
	
	for (int i = size - 1; i >= 0; --i)
	{
		// y label
		if (i%3 == 0)
			cout << setw(4) << origin.y + scale * i << " ";
		else
			cout << "     ";
		// actual content
		for (int j = 0; j < size; ++j)
			cout << display_matrix[i][j];
		cout << endl;
	}
	// x label
	for (int j = 0; j < size; ++j)
		if (j%3 == 0)
			cout << "  " << setw(4) << origin.x + scale * j;
	cout << endl;
	cout.precision(old_precision);
}

void Grid_view::callibrate_origin(Point location)
{
	double offset = (size/2.)*scale;
	origin = Point(location.x - offset, location.y - offset);
}




/* class declarations for Health_view */
/* =========================================================*/

Map_view::Map_view():Grid_view(default_origin, default_scale, default_size)
{

}

void Map_view::draw()
{

	print_grid_header();
	// Init the display map
	vector< vector<string> >  display_matrix
		(max_map_size, vector<string>(max_map_size, ". "));
	
	// Assign the values first
	fill_display_matrix(display_matrix);
	
	show_outsiders();
	
	draw_grid(display_matrix);
}
/* =========================================================*/

/* class declarations for Health_view */
/* =========================================================*/
// constructor
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
		(max_map_size, vector<string>(max_map_size, ". "));
	
	// Assign the values first
	fill_display_matrix(display_matrix);
		
	draw_grid(display_matrix);
}
/* =========================================================*/

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