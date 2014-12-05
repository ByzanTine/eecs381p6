#ifndef AGRICULTURE_VIEW_H
#define AGRICULTURE_VIEW_H

#include "Grid_view.h"
#include <set>
#include <map>
class Agriculture_view : public Grid_view {
public:
	// default constructor sets the default size, scale, and origin
	Agriculture_view(); 
	// only update those that have the amount
	void update_location(const std::string& name, Point location) override;
	// get the name that has amount stored
	void update_food_carry(const std::string& name, double food_carry) override;
	// remove the inside garbage data here as well
	void update_remove(const std::string& name) override;
	// prints out the current map
	void draw() override;
private:
	std::map<std::string, Point> name_loc_pool;
	std::set<std::string> amount_holders;

};

#endif