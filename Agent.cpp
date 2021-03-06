#include "Agent.h"


#include <iostream>
#include <cassert>
#include "Model.h"
#include "Utility.h"

using std::cout;
using std::cerr;
using std::endl;
using std::shared_ptr;
using std::string;
using std::dynamic_pointer_cast;
const int init_health = 5;
const double init_speed = 5.0;

Agent::~Agent()
{
	// cout << "Agent " <<  get_name() << " destructed" << endl;
}

Point Agent::get_location() const
{
	return mov_object.get_current_location();
}

// return true if this Agent is in motion
bool Agent::is_moving() const
{
	return mov_object.is_currently_moving();
}

// tell this Agent to start moving to location destination_
void Agent::move_to(Point destination_)
{
	mov_object.start_moving(destination_);
	if (mov_object.is_currently_moving())
		cout << get_name() << ": I'm on the way" << endl;
	else
		cout << get_name() << ": I'm already there" << endl;
}

// tell this Agent to stop its activity
void Agent::stop()
{
	if (mov_object.is_currently_moving())
	{
		cout << get_name() << ": I'm stopped" << endl;
		mov_object.stop_moving();
	}
	
}

// Tell this Agent to accept a hit from an attack of a specified strength
// The attacking Agent identifies itself with its this pointer.
// A derived class can override this function.
// The function lose_health is called to handle the effect of the attack.
void Agent::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr)
{
	lose_health(attack_strength);

}

// update the moving state and Agent state of this object.
void Agent::update()
{
	switch(alive_state)
	{
		case Alive_state::Alive:
			if (is_moving())
			{
				if (mov_object.update_location())
					cout << get_name() << ": I'm there!"<< endl;
				else
					cout << get_name() << ": step..." << endl;
				
				Model::get_instance().notify_location(get_name(), get_location());
			}
			break;
		case Alive_state::Dead:
			alive_state = Alive_state::Dead;
			break;
		default:
			cerr << "Agent is in a inaccessible state" << endl;
			assert(0);

	}
}

// output information about the current state
void Agent::describe() const
{
	cout << get_name()  << " at " << get_location() << endl;
	
	switch(alive_state)
	{
		case Alive_state::Alive:
			cout << "   Health is " << health << endl;
			if (is_moving())
				cout << "   Moving at speed " << mov_object.get_current_speed() << " to " << mov_object.get_current_destination() << endl;
			else
				cout << "   Stopped" << endl;
			break;
		case Alive_state::Dead:
			cout << "   Is dead" << endl;
			break;
		default:
			cerr << "Agent in a inaccessible state" << endl;
			assert(0);
	}
	
	
	
}

// ask Model to broadcast our current state to all Views
void Agent::broadcast_current_state()
{
	Model::get_instance().notify_health(get_name(), health);
	Model::get_instance().notify_location(get_name(), get_location());
}

/* Fat Interface for derived classes */
// Throws exception that an Agent cannot work.
void Agent::start_working(shared_ptr<Structure>, shared_ptr<Structure>)
{
	 throw Error(get_name() + ": Sorry, I can't work!");

}

// Throws exception that an Agent cannot attack.
void Agent::start_attacking(shared_ptr<Agent>)
{
	throw Error(get_name()  + ": Sorry, I can't attack!");
}
// resolve ambiguity
const string& Agent::get_name() const
{
	return Sim_object::get_name();
}

// Make this an abstract class by making the constructor protected to prevent direct creation.
// create with initial health is 5, speed is 5, state is Alive
Agent::Agent(const std::string& name_, Point location_): Sim_object(name_)
{
	health = init_health;
	alive_state = Alive_state::Alive;
	mov_object = Moving_object(location_, init_speed);
	// cout << "Agent " << get_name() << " constructed" << endl;

}

// calculate loss of health due to hit.
// if health decreases to zero or negative, Agent state becomes Dying, and any movement is stopped.
void Agent::lose_health(int attack_strength)
{
	health -= attack_strength;
	if (health <= 0)
	{
		cout << get_name() << ": Arrggh!" << endl;
		alive_state = Alive_state::Dead;
		mov_object.stop_moving();
		Model::get_instance().notify_gone(get_name());
		Model::get_instance().remove_agent(get_name());

	}
	else
	{
		cout << get_name() << ": Ouch!" << endl;
		
		Model::get_instance().notify_health(get_name(), health);
	}
}