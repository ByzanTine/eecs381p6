/* *** Views class ***
All the inherited class definition.
Include local object view, health view and amount view
*/


#ifndef VIEWS_H
#define VIEWS_H
#include <map>
#include <vector>
#include <string>
#include "View.h"
/* intermediate base class */
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
	// default constructor sets the default size, scale, and origin
	Grid_view(Point origin_, double scale_, int size_); // avoid instance creation
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

class Map_view : public Grid_view {
public:
	// default constructor sets the default size, scale, and origin
	Map_view(); 
	// prints out the current map
	void draw() override;

};


class Local_view : public Grid_view {
public:
	// constructor
	Local_view(const std::string& name);
	// override to also update the center
	void update_location(const std::string& name, Point location) override;
	// prints out the current map
	void draw() override;

	// disable the set functions be empty out them
	// Maybe throw exception can be better
	void set_size(int size_) override {};
	
	// If scale is not postive, will throw Error("New map scale must be positive!");
	void set_scale(double scale_) override {};
	
	// any values are legal for the origin
	void set_origin(Point origin_) override {};
	
	// set the parameters to the default values
	void set_defaults() override {};
private:
	std::string center_object_name; // for location sync
};

/* class declarations for Health_view */
/* =========================================================*/

class Health_view : public View {
public: 
	// Save the supplied health data for future use in a draw() call
	void update_health(const std::string& name, double health) override;
	// update the view if a object is gone
	void update_remove(const std::string& name) override;
	void draw() override;
private:
	using status_t = std::map<std::string, double>;
	status_t health_status;
};

/* class declarations for Amounts_view */
/* =========================================================*/
class Amounts_view : public View {
public: 
	// Save the supplied food_carry data for future use in a draw() call
	void update_food_carry(const std::string& name, double food_carry) override;
	// update the view if a object is gone
	void update_remove(const std::string& name) override;
	void draw() override;
private:
	using status_t = std::map<std::string, double>;
	status_t carry_status;

};


#endif