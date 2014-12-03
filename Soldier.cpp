#include "Soldier.h"

#include <iostream>
#include <cassert>
#include "Utility.h"

using std::cout;
using std::endl;
using std::shared_ptr;


/* =========================================================*/

/* class definitions for Soldier */
/* Soldier is inherit from the Warrior class */
/* =========================================================*/
// If the file is seperated, the parameters should be at the top part of the file
// const int init_attack_strength = 2;
// const double init_attack_range = 2.0;
Soldier::Soldier(const std::string& name_, Point location_):Warrior(name_, location_, 2, 2.0) // only will be used once
{
	

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
