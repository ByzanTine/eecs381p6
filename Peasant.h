/* 
A Peasant is an Agent that can move food between Structures. It can be commanded to
start_working, whereupon it moves to the source, picks up food, returns to destination,
deposits the food, returns to source.  If picks up zero food at the source, it waits there
and tries again on the next update. 
If commanded to move_to somewhere, it stops working, and goes there.
*/

#ifndef PEASANT_H
#define PEASANT_H
#include <memory>
#include "Agent.h"

class Peasant: public Agent
{
public:
	// initialize with zero amount being carried
	Peasant(const std::string& name_, Point location_);

	// implement Peasant behavior
	void update() override;
	
	// overridden to suspend working behavior
    void move_to(Point dest) override;
	
	// stop moving and working
	void stop() override;

	// starts the working process
	// Throws an exception if the source is the same as the destination.
	void start_working(std::shared_ptr<Structure> source_, std::shared_ptr<Structure> destination_) override;

	// output information about the current state
	void describe() const override;
	// ask model to update the state 
	void broadcast_current_state() override;

	bool can_attack() override 
	{
		return false;
	}

	bool can_work() override 
	{
		return true;
	}
private:
	double food_carrying;
	enum class Peasant_state {Not_working, Inbound, Collecting, Outbound, Depositing};
	Peasant_state peasant_state;
	std::shared_ptr<Structure> source;
	std::shared_ptr<Structure> destination;
};

#endif