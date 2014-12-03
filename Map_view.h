#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include "Grid_view.h"

class Map_view : public Grid_view {
public:
	// default constructor sets the default size, scale, and origin
	Map_view(); 
	// prints out the current map
	void draw() override;

};

#endif