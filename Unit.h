#ifndef UNIT_H
#define UNIT_H

#include "Sim_object.h"
#include "Utility.h"
#include <memory>

class Structure;
class Agent;

class Unit : public std::enable_shared_from_this<Unit> {
public:

	// tell this Agent to start moving to location destination_
	virtual void move_to(Point destination_) = 0;

	// tell this Agent to stop its activity
	virtual void stop() = 0;

	/* Fat Interface for derived classes */
	// Throws exception that an Agent cannot work.
	virtual void start_working(std::shared_ptr<Structure>, std::shared_ptr<Structure>) = 0;

	// Throws exception that an Agent cannot attack.
	virtual void start_attacking(std::shared_ptr<Agent>) = 0;

	virtual void add_component(std::shared_ptr<Unit>) {
		throw (Error("I'm not a group!"));
	}

	virtual void remove_component(std::shared_ptr<Unit>) {
		throw (Error("I'm not a group!"));
	}
	void set_parent(std::shared_ptr<Unit>);
protected:
	bool is_offspring_of(std::shared_ptr<Unit>);
private :

	std::weak_ptr<Unit> parent;
};

#endif