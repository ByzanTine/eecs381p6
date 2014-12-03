#ifndef LOCAL_VIEW_H
#define LOCAL_VIEW_H

#include "Grid_view.h"
#include <string>

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

#endif