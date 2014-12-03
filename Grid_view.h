#ifndef GRID_VIEW_H
#define GRID_VIEW_H

#include "View.h"
#include <map>
#include <string>
#include <vector>

class Grid_view : public View {
public:
	
	// avoid instance creation
	// save the update_location pair for 
	void update_location(const std::string& name, Point location) override;
	void update_remove(const std::string& name) override;

	// Fat interface: disable the functionailty in some derived class 

	// modify the display parameters
	// if the size is out of bounds will throw Error("New map size is too big!")
	// or Error("New map size is too small!")
	virtual void set_size(int size_);
	
	// If scale is not postive, will throw Error("New map scale must be positive!");
	virtual void set_scale(double scale_);
	
	// any values are legal for the origin
	virtual void set_origin(Point origin_);
	
	// set the parameters to the default values
	virtual void set_defaults();

protected:
	// Constructor that comming parameter
	Grid_view(Point origin_, double scale_, int size_); // avoid instance creation
	// default constructor sets the default size, scale, and origin
	Grid_view(); 
	
	bool get_subscripts(int &ix, int &iy, Point location); // helper function
	// drawing helpers 
	// Print the size origin scale 
	void print_grid_header();
	// fill the display matrix for drawing
	void fill_display_matrix(std::vector< std::vector<std::string> >& display_matrix);
	// show outsiders
	void show_outsiders();
	// draw the grid with the axis label
	void draw_grid(const std::vector< std::vector<std::string> >& display_matrix);
	// callibrate the grid origin center to the object location
	void callibrate_origin(Point location);

private:
	Point origin;
	double scale;
	int size;

	std::map<std::string, Point> name_location_map;	
};

#endif