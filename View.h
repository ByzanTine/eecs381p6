/* View class, Abstract, like a interface
The View class encapsulates the data and functions needed to generate the map
display, and control its properties. It has a "memory" for the names and locations
of the to-be-plotted objects.

// Base clsas Usage: 
1. Call the update_location function with the name and position of each object
to be plotted. update_health and update_food_carry add more data protocol for the internal data
int the View to change. If the object is not already in the View's memory, it will be added
along with its location. If it is already present, its location will be set to the 
supplied location. If a single object changes location, its location can be separately
updated with a call to update_change. 
2. Call the update_remove function with the name of any object that should no longer
be plotted. This must be done *after* any call to update_change that 
has the same object name since update_change will add any object name supplied.
3. Call the draw function to print out the map.
// Derived Grid class 
1. As needed, change the origin, scale, or displayed size of the map 
with the appropriate functions. Since the view "remembers" the previously updated
information, immediately calling the draw function will print out a map showing the previous objects
using the new settings.
*/


#ifndef VIEW_H
#define VIEW_H
#include <map>
#include <string>
#include "Geometry.h"


class View {
public:
	// default constructor sets the default size, scale, and origin
	
	// Save the supplied location data for future use in a draw() call
	// If the name is already present,the new location replaces the previous one.
	virtual void update_location(const std::string& name, Point location) {};
	// Save the supplied health data for future use in a draw() call
	virtual void update_health(const std::string& name, double health) {};
	// Save the supplied food_carry data for future use in a draw() call
	virtual void update_food_carry(const std::string& name, double food_carry) {};
	// Remove the name and its location; no error if the name is not present.
	virtual void update_remove(const std::string& name) {};
	// For the ease of derived classes, make them empty 

	// prints out the current map
	virtual void draw() = 0; // you always need to override draw 
		

};
#endif