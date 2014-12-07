#ifndef UNIT_H
#define UNIT_H

#include <memory>

class Structure;
class Agent;
class Point;

class Unit : public std::enable_shared_from_this<Unit> 
{
public:

	// tell this Unit to start moving to location destination_
	virtual void move_to(Point destination_) = 0;

	// tell this Unit to stop its activity
	virtual void stop() = 0;

	/* Fat Interface for derived classes */
	virtual void start_working(std::shared_ptr<Structure>, std::shared_ptr<Structure>) = 0;

	virtual void start_attacking(std::shared_ptr<Agent>) = 0;

	virtual void add_component(std::shared_ptr<Unit>);

	virtual void remove_component(std::shared_ptr<Unit>);

	void set_parent(std::shared_ptr<Unit>);
protected:
	bool is_offspring_of(std::shared_ptr<Unit>);
private :

	std::weak_ptr<Unit> parent;
};

#endif
