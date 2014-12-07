#include "Zealot.h"

#include <iostream>
#include <cassert>
#include "Model.h"
#include "Utility.h"

using std::cout;
using std::endl;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::string;

const int init_attack_strength = 2;
const double init_attack_range = 2.0;

Zealot::Zealot(const string& name_, Point location_):
	Warrior(name_, location_, init_attack_strength, init_attack_range)
{}

void Zealot::start_attacking(shared_ptr<Agent> target_ptr)
{
	if (target_ptr == shared_from_this()) //maybe use get()==this?
		throw Error(get_name() + ": I cannot attack myself!");

	if (!target_ptr->is_alive())
		throw Error(get_name() + ": Target is not alive!");
	// whether out range or not, attack you!
	go_war(target_ptr);
	// do different thing if the guy is out/in range
	Point target_location = target_ptr->get_location();
	if (cartesian_distance(get_location(), target_location) > get_attack_range())
	{
		cout << get_name() << ": Target is not in range, I am chasing!" << endl;
		// chase it down	
		move_to(target_location);	
	}
	else
	{
		cout << get_name() << ": I'm attacking!" << endl;
	}
}
// update implements Soldier behavior
void Zealot::update()
{
	Agent::update();
	if (!is_alive())
		return;
	// if Zealot is still chasing the guy,
	// No need to bother about the target
	if (is_moving())
		return;
	// if Zealot is not attacking, find a target anyway
	if (!is_attacking())
	{
		attack_nearest_target();
		return;
	}
	// In attack state now, target should have decent weak_ptr
	shared_ptr<Agent> shared_target = get_target_ptr();

	if (!shared_target || !shared_target->is_alive())
	{
		cout << get_name() << ": Target is dead" << endl;
		// find another target to attack
		attack_nearest_target();
		return;
	}
	Point target_location = shared_target->get_location();
	if (cartesian_distance(get_location(), target_location) > get_attack_range())
	{
		cout << get_name() << ": Target is out of range, chase it down!" << endl;
		// chase it down	
		move_to(target_location);	
		// go_peace();
		return;
	}
	cout << get_name() << ": Clang!" << endl;
	shared_target->take_hit(get_attack_strength(), dynamic_pointer_cast<Agent>(shared_from_this()));
	if (!shared_target->is_alive())
	{
		cout << get_name() << ": I triumph!" << endl;
		// Find another target to attack
		attack_nearest_target();
	}
}

// output information about the current state
void Zealot::describe() const 
{
	cout << "Zealot ";
	Warrior::describe();
	
}

void Zealot::attack_nearest_target()
{
	shared_ptr<Agent> nearest_agent = Model::get_instance().get_nearest_agent_ptr(get_name());
	if (nearest_agent)
	{
		assert(nearest_agent != shared_from_this());
		// if in range, 
		start_attacking(nearest_agent); // should not throw any error
	}
	else
	{
		go_peace();
	}
}