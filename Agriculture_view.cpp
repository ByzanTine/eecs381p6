#include "Agriculture_view.h"

#include <vector>
#include <string>
using std::vector;
using std::string;

const int max_map_size = 30; // TODO refactor with Grid View

Agriculture_view::Agriculture_view():Grid_view()
{}

void Agriculture_view::update_location(const std::string& name, Point location)
{
	name_loc_pool[name] = location;
	if (amount_holders.find(name) != amount_holders.end())
		Grid_view::update_location(name, location);
}

void Agriculture_view::update_food_carry(const std::string& name, double food_carry)
{
	amount_holders.insert(name);
	auto iter = name_loc_pool.find(name);
	if (iter != name_loc_pool.end())
		Grid_view::update_location(name, iter->second);
}

// No change to the draw
void Agriculture_view::update_remove(const std::string& name)
{
	Grid_view::update_remove(name);
	// clear the view data if object disappear
	if (name_loc_pool.find(name) != name_loc_pool.end())
		name_loc_pool.erase(name);
	if (amount_holders.find(name) != amount_holders.end())
		amount_holders.erase(name);
}

void Agriculture_view::draw()
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
