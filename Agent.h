/* 
Agents are a kind of Sim_object, and privately inherit from Moving_object.
Agents can be commanded to move to a destination. Agents have a health, which
is decreased when they take a hit. If the Agent's health > 0, it is alive.
If its heath <= 0, it starts dying, then on subsequent updates, 
it becomes dead, and finally disappearing.
*/

#ifndef AGENT_H
#define AGENT_H

#include <memory>
#include "Sim_object.h"
#include "Unit.h"
#include "Moving_object.h"


class Structure;

// Inherit from Sim_object and Moving Object
class Agent : public Sim_object, public Unit
{
public:

	// declare destructor
	~Agent();
	// return true if this agent is Alive
	bool is_alive() const
		{return alive_state == Alive_state::Alive;}
	
	// return this Agent's location
	Point get_location() const override;

	// return true if this Agent is in motion
	bool is_moving() const;
	
	// tell this Agent to start moving to location destination_
	void move_to(Point destination_) override;

	// tell this Agent to stop its activity
	void stop() override;

	// Tell this Agent to accept a hit from an attack of a specified strength
	// The attacking Agent identifies itself with its this pointer.
	// A derived class can override this function.
	// The function lose_health is called to handle the effect of the attack.
	virtual void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr);
	
	// update the moving state and Agent state of this object.
	void update() override;
	
	// output information about the current state
	void describe() const override;
	
	// ask Model to broadcast our current state to all Views
	void broadcast_current_state() override;

	/* Fat Interface for derived classes */
	// Throws exception that an Agent cannot work.
	void start_working(std::shared_ptr<Structure>, std::shared_ptr<Structure>) override;

	// Throws exception that an Agent cannot attack.
	void start_attacking(std::shared_ptr<Agent>) override;

	// Accessor of name, resolve ambiguity from Sim_object and Unit
	const std::string& get_name() const override;

protected:
 	// Make this an abstract class by making the constructor protected to prevent direct creation.
	// create with initial health is 5, speed is 5, state is Alive
	Agent(const std::string& name_, Point location_);

	// calculate loss of health due to hit.
	// if health decreases to zero or negative, Agent state becomes Dying, and any movement is stopped.
	void lose_health(int attack_strength);
private:
	int health;
	enum class Alive_state {Alive, Dead};
	Alive_state alive_state;
	Moving_object mov_object;

};
#endif