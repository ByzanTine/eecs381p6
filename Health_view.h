#ifndef HEALTH_VIEW_H
#define HEALTH_VIEW_H

#include "View.h"
#include <string>

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

#endif