/* *** Views class ***
All the inherited class definition.
Include local object view, health view and amount view
*/


#ifndef VIEWS_H
#define VIEWS_H
#include <map>
#include <vector>
#include <string>
#include "List_view.h"



/* class declarations for Amounts_view */
/* =========================================================*/
class Amounts_view : public List_view {
public: 
	// Save the supplied food_carry data for future use in a draw() call
	void update_food_carry(const std::string& name, double food_carry) override;
	// leaf draw
	void draw() override;

};


#endif