#include "Warriors.h"

#include <iostream>
#include <cassert>

#include "Model.h"
#include "Utility.h"
#include "Structure.h"
using std::cout;
using std::endl;
using std::shared_ptr;

/* class definitions for Warrior */
/* =========================================================*/
// destructor 
Warrior::~Warrior()
{
	// cout << "Warrior " << get_name() << " destructed" << endl;
}
// start_attacking is the same protocol
void Warrior::start_attacking(std::shared_ptr<Agent> target_ptr)
{
	if (target_ptr == shared_from_this()) //maybe use get()==this?
		throw Error(get_name() + ": I cannot attack myself!");

	if (!target_ptr->is_alive())
		throw Error(get_name() + ": Target is not alive!");

	if (cartesian_distance(get_location(), target_ptr->get_location()) > attack_range)
		throw Error(get_name() + ": Target is out of range!");
	go_war(target_ptr);

	cout << get_name() << ": I'm attacking!" << endl;
}

void Warrior::stop()
{
	cout << get_name() << ": Don't bother me" << endl;
}


// output information about the current state, need access to all variables
void Warrior::describe() const
{
	Agent::describe();
	// Get the thing from shared_ptr
	if (warrior_state == Warrior_state::Attacking)
	{
		shared_ptr<Agent> shared_target = target.lock();
		if (!shared_target || !shared_target->is_alive())
			cout << "   Attacking dead target" << endl;
		else
			cout << "   Attacking " << shared_target->get_name() << endl;
	}
	else
	{
		cout << "   Not attacking" << endl;	
	}
}

// Make this an abstract class by making the constructor protected to prevent direct creation.
Warrior::Warrior(const std::string& name_, Point location_, int attack_strength_, int attack_range_):Agent(name_, location_)
{
	// cout << "Warrior " << get_name() << " constructed" << endl;
	warrior_state = Warrior_state::Not_Attacking;
	attack_strength = attack_strength_;
	attack_range = attack_range_;
}
// a derived class specific interface to transfer to the not-attacking state and discard target_ptr
void Warrior::go_peace()
{
	warrior_state = Warrior_state::Not_Attacking;
	target.reset();
}
// a derived class specific interface to transfer to the attacking state and assign target_ptr
void Warrior::go_war(std::shared_ptr<Agent> target_ptr)
{	
	warrior_state = Warrior_state::Attacking;
	target = target_ptr;
}
// * getters 
bool Warrior::is_attacking()
{
	return warrior_state == Warrior_state::Attacking;
}
// return a shared ptr by the weak ptr, may return nullptr.
std::shared_ptr<Agent> Warrior::get_target_ptr()
{
	return target.lock();
}
/* =========================================================*/

/* class definitions for Soldier */
/* Soldier is inherit from the Warrior class */
/* =========================================================*/
// If the file is seperated, the parameters should be at the top part of the file
// const int init_attack_strength = 2;
// const double init_attack_range = 2.0;
Soldier::Soldier(const std::string& name_, Point location_):Warrior(name_, location_, 2, 2.0) // only will be used once
{
	// cout << "Soldier " << get_name() << " constructed" << endl;

}

Soldier::~Soldier()
{
	// cout << "Soldier " << get_name() << " destructed" << endl;
}

// update implements Soldier behavior
void Soldier::update()
{
	Agent::update();
	if (!is_alive())
		return;
	if (!is_attacking())
		return;
	// In attack state now, target should have decent weak_ptr
	shared_ptr<Agent> shared_target = get_target_ptr();

	if (!shared_target || !shared_target->is_alive())
	{
		cout << get_name() << ": Target is dead" << endl;
		go_peace();
		return;
	}
	if (cartesian_distance(get_location(), shared_target->get_location()) > get_attack_range())
	{
		cout << get_name() << ": Target is now out of range" << endl;
		go_peace();
		return;
	}
	cout << get_name() << ": Clang!" << endl;
	shared_target->take_hit(get_attack_strength(), shared_from_this());
	if (!shared_target->is_alive())
	{
		cout << get_name() << ": I triumph!" << endl;
		go_peace();	
	}
}



// Overrides Agent's take_hit to counterattack when attacked.
void Soldier::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr)
{
	assert(is_alive());
	
	lose_health(attack_strength);
	if (is_attacking() && !is_alive())
		go_peace();
	if (!is_attacking() && 
		is_alive()&&attacker_ptr->is_alive())
	{
		go_war(attacker_ptr);
		cout << get_name() << ": I'm attacking!" << endl;
	}
}

// output information about the current state
void Soldier::describe() const 
{
	cout << "Soldier ";
	Warrior::describe();
	
}
/* =========================================================*/
/* class definitions for Archer */
/* Archer is inherit from the Warrior class */
/* =========================================================*/
Archer::Archer(const std::string& name_, Point location_):Warrior(name_, location_, 1, 6.0) // strength first
{
	// cout << "Soldier " << get_name() << " constructed" << endl;

}

Archer::~Archer()
{
	// cout << "Soldier " << get_name() << " destructed" << endl;
}

// update implements Soldier behavior
void Archer::update()
{
	Agent::update();
	if (!is_alive())
		return;
	
	if (is_attacking())
	{
		shared_ptr<Agent> shared_target = get_target_ptr();

		if (!shared_target || !shared_target->is_alive())
		{
			cout << get_name() << ": Target is dead" << endl;
			go_peace();
		}
		else if (cartesian_distance(get_location(), shared_target->get_location()) > get_attack_range())
		{
			cout << get_name() << ": Target is now out of range" << endl;
			go_peace();
		}
		else
		{
			cout << get_name() << ": Twang!" << endl;
			shared_target->take_hit(get_attack_strength(), shared_from_this());
			if (!shared_target->is_alive())
			{
				cout << get_name() << ": I triumph!" << endl;
				go_peace();	
			}
		}
		
	}
	if (!is_attacking())
	{
		
		// find the closest guy to attack
		shared_ptr<Agent> nearest_agent = Model::get_instance().get_nearest_agent_ptr(get_name());
		if (nearest_agent)
		{
			assert(nearest_agent != shared_from_this());
			// if in range, 
			if (cartesian_distance(get_location(), nearest_agent->get_location()) <= get_attack_range())
			{
				go_war(nearest_agent);
				cout << get_name() << ": I'm attacking!" << endl;
			}
		}		
	}
	
}



// Overrides Agent's take_hit to counterattack when attacked.
void Archer::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr)
{
	assert(is_alive());
	
	lose_health(attack_strength);
	if (!is_alive())
		return;

	if (attacker_ptr->is_alive())
	{
		// find a nearest structure to hide
		shared_ptr<Structure> nearest_structure = Model::get_instance().get_nearest_structure_ptr(get_name());
		if (nearest_structure)
		{
			cout << get_name() << ": I'm going to run away to " << nearest_structure->get_name() << endl;
			move_to(nearest_structure->get_location());
		}
		
	}
}

// output information about the current state
void Archer::describe() const 
{
	cout << "Archer ";
	Warrior::describe();
	
}


/* =========================================================*/
	