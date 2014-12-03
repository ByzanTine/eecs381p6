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