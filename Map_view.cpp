#include "Map_view.h"

#include <vector>
#include <string>
using std::vector;
using std::string;

const int max_map_size = 30; // TODO refactor with Grid View

Map_view::Map_view():Grid_view()
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
