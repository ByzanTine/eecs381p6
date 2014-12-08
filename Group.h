#ifndef GROUP_H
#define GROUP_H

#include "Unit.h"
#include <set>
#include <string>

class Group : public Unit {
public:
	Group(const std::string&);

	// tell this group to start moving to location destination_
	void move_to(Point destination_) override;

	// tell this group to stop its activity
	void stop() override;
	// One that can work will work, others will complain
	void start_working(std::shared_ptr<Structure>, std::shared_ptr<Structure>) override;
	// One that can attack will attack, others will complain
	void start_attacking(std::shared_ptr<Agent>) override;
	// add a unit to a group
	void add_component(std::shared_ptr<Unit>) override;
	// remove a unit from this group
	void remove_component(std::shared_ptr<Unit>) override;
	// accessor of name
	const std::string& get_name() const override
	{return name;}
	// output a list of names that the Group has
	// No internal information is printed 
	void describe();
private:
	std::set<std::shared_ptr<Unit> > members;
	std::string name;
};

#endif
