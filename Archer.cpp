#include "Archer.h"

#include <iostream>
#include <cassert>

#include "Structure.h"
#include "Model.h"

using std::cout;
using std::endl;
using std::shared_ptr;
Archer::Archer(const std::string& name_, Point location_):Warrior(name_, location_, 1, 6.0) // strength first
{}

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
	