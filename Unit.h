#ifndef UNIT_H
#define UNIT_H

#include <memory>

class Structure;
class Agent;
class Point;
// A abstract and fat interface.
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
	// throw a error
	virtual void add_component(std::shared_ptr<Unit>);
	// throw a error
	virtual void remove_component(std::shared_ptr<Unit>);
	// set input unit as a parent
	void set_parent(std::shared_ptr<Unit>);
	// get name, only derived class have name
	virtual const std::string& get_name() const = 0;
protected:
	// determine if this unit is offspring of the input unit
	bool is_offspring_of(std::shared_ptr<Unit>);
private:

	std::weak_ptr<Unit> parent;
};

#endif
