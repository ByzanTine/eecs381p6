/* *** List_view class ***
All the inherited class definition.
Include local object view, health view and amount view
*/


#ifndef LIST_VIEW_H
#define LIST_VIEW_H
#include <map>
#include <string>
#include "View.h"

// Abstract class TODO
class List_view : public View {
public: 
	// update the view if a object is gone
	void update_remove(const std::string& name) override;

	void draw() override;

protected:
	using status_t = std::map<std::string, double>;
	void update_status(const std::string& name, double value);

private:
	status_t status;
};	


#endif