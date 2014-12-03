#ifndef HEALTH_VIEW_H
#define HEALTH_VIEW_H

#include "List_view.h"
#include <string>

class Health_view : public List_view {
public: 
	// Save the supplied health data for future use in a draw() call
	void update_health(const std::string& name, double health) override;
	// leaf draw
	void draw() override;

};

#endif