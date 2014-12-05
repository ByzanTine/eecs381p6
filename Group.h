#ifndef GROUP_H
#define GROUP_H

#include "Unit.h"
#include <set>

class Group : public Unit {
public:

	// tell this group to start moving to location destination_
	void move_to(Point destination_) override;

	// tell this group to stop its activity
	void stop() override;

	void start_working(std::shared_ptr<Structure>, std::shared_ptr<Structure>) override;

	void start_attacking(std::shared_ptr<Agent>) override;

	void add_component(std::shared_ptr<Unit>) override;

	void remove_component(std::shared_ptr<Unit>) override;
private:
	std::set<std::shared_ptr<Unit> > members;
};

#endif