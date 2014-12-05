#include "Model.h"

#include <algorithm>
#include "View.h"
#include "Structure_factory.h"
#include "Agent_factory.h"
#include "Geometry.h"
#include "Structure.h"
#include "Agent.h"
#include "Group.h"
#include "Utility.h"
#include <functional>
using std::bind;
using std::pair;
using std::string;
using std::shared_ptr;
using namespace std::placeholders;
using std::for_each;


Model& Model::get_instance()
{
	static Model m;
	return m;
}

Model::Model()
{
	time = 0;
	// Create the default initial structures 
	shared_ptr<Structure> structure_R = create_structure("Rivendale", "Farm", Point(10., 10.));
	shared_ptr<Structure> structure_S = create_structure("Sunnybrook", "Farm", Point(0., 30.));
	shared_ptr<Structure> structure_Sh = create_structure("Shire", "Town_Hall", Point(20., 20.));
	shared_ptr<Structure> structure_P = create_structure("Paduca", "Town_Hall", Point(30., 30.));
	// Create the default initial agents
	shared_ptr<Agent> agent_P =  create_agent("Pippin", "Peasant", Point(5., 10.));
	shared_ptr<Agent> agent_M = create_agent("Merry", "Peasant", Point(0., 25.));
	shared_ptr<Agent> agent_Z =  create_agent("Zug", "Soldier", Point(20., 30.));
	shared_ptr<Agent> agent_B = create_agent("Bug", "Soldier", Point(15., 20.));
	shared_ptr<Agent> agent_I = create_agent("Iriel", "Archer", Point(20.,38.));

	sim_object_pool = 
	{
		{structure_R->get_name(), structure_R},
		{structure_S->get_name(), structure_S},
		{structure_Sh->get_name(), structure_Sh},
		{structure_P->get_name(), structure_P},	
		{agent_P->get_name(), agent_P},
		{agent_M->get_name(), agent_M},
		{agent_Z->get_name(), agent_Z},
		{agent_B->get_name(), agent_B},
		{agent_I->get_name(), agent_I}
	};
	agent_pool = 	
	{
		{agent_P->get_name(), agent_P},
		{agent_M->get_name(), agent_M},
		{agent_Z->get_name(), agent_Z},
		{agent_B->get_name(), agent_B},
		{agent_I->get_name(), agent_I}
	};
	structure_pool = 
	{
		{structure_R->get_name(), structure_R},
		{structure_S->get_name(), structure_S},
		{structure_Sh->get_name(), structure_Sh},
		{structure_P->get_name(), structure_P}
	};
	
}

Model::~Model()
{}
	


// is name already in use for either agent or structure?
// return true if the name matches the name of an existing agent or structure
bool Model::is_name_in_use(const string& name) const
{
	return sim_object_pool.find(name) != sim_object_pool.end()
		|| group_pool.find(name) != group_pool.end();
}

// is there a structure with this name?
bool Model::is_structure_present(const string& name) const
{
	return structure_pool.find(name) != structure_pool.end();
}
// add a new structure; assumes none with the same name
void Model::add_structure(shared_ptr<Structure> structure_ptr)
{
	
	sim_object_pool.emplace(structure_ptr->get_name(), structure_ptr);
	structure_pool.emplace(structure_ptr->get_name(), structure_ptr);
	structure_ptr->broadcast_current_state();
}
// will throw Error("Structure not found!") if no structure of that name
shared_ptr<Structure> Model::get_structure_ptr(const string& name) const
{
	auto structure_iter = structure_pool.find(name);
	if (structure_iter == structure_pool.end())
		throw Error("Structure not found!");
	else
		return structure_iter->second;
}

shared_ptr<Structure> Model::get_nearest_structure_ptr(const string& name) const
{
	shared_ptr<Structure> min_distance_structure = nullptr;
	shared_ptr<Agent> cur_agent = get_agent_ptr(name);
	for (const pair<string, shared_ptr<Structure>>& content : structure_pool)
	{
		if (!min_distance_structure)
			min_distance_structure = content.second;
		else if (
			cartesian_distance(content.second->get_location(), cur_agent->get_location())
			<
			cartesian_distance(min_distance_structure->get_location(), cur_agent->get_location())
			)
		{
			min_distance_structure = content.second;
		}
	}
	return min_distance_structure;
}

// is there an agent with this name?
bool Model::is_agent_present(const string& name) const
{
	return agent_pool.find(name) != agent_pool.end();
}
// add a new agent; assumes none with the same name
void Model::add_agent(shared_ptr<Agent> agent_ptr)
{
	sim_object_pool.emplace(agent_ptr->get_name(), agent_ptr);
	agent_pool.emplace(agent_ptr->get_name(), agent_ptr);
	agent_ptr->broadcast_current_state();
}
// will throw Error("Agent not found!") if no agent of that name
shared_ptr<Agent> Model::get_agent_ptr(const string& name) const
{
	auto agent_iter = agent_pool.find(name);
	if (agent_iter == agent_pool.end())
		throw Error("Agent not found!");
	else
		return agent_iter->second;
}

void Model::remove_agent(shared_ptr<Agent> agent_ptr)
{
	sim_object_pool.erase(agent_ptr->get_name());
	agent_pool.erase(agent_ptr->get_name());
}

// Using a range for looks better then min_element with struct
shared_ptr<Agent> Model::get_nearest_agent_ptr(const string& name) const
{
	shared_ptr<Agent> min_distance_agent = nullptr;
	shared_ptr<Agent> cur_agent = get_agent_ptr(name);
	for (const pair<string, shared_ptr<Agent>>& content : agent_pool)
	{ 
		// ignore the same name
		if (content.first == name)
			continue;
		if (!min_distance_agent)
			min_distance_agent = content.second;
		else if (
			cartesian_distance(content.second->get_location(), cur_agent->get_location())
			<
			cartesian_distance(min_distance_agent->get_location(), cur_agent->get_location())
			)
		{
			min_distance_agent = content.second;
		}
	}
	return min_distance_agent;
}
// tell all objects to describe themselves to the console
void Model::describe() const
{
	for_each(sim_object_pool.begin(), sim_object_pool.end(), 
		bind(&Sim_object::describe,
			bind(&std::map<string, shared_ptr<Sim_object>>::value_type::second, _1))); 
}
// increment the time, and tell all objects to update themselves
void Model::update()
{
	++time;
	for_each(sim_object_pool.begin(), sim_object_pool.end(), 
		bind(&Sim_object::update,
			bind(&sim_object_pool_t::value_type::second, _1))); 
}

/* View services */
// Attaching a View adds it to the container and causes it to be updated
// with all current objects'location (or other state information.
void Model::attach(shared_ptr<View> view_in)
{
	views.insert(view_in);
	for_each(sim_object_pool.begin(), sim_object_pool.end(), 
		bind(&Sim_object::broadcast_current_state,
			bind(&sim_object_pool_t::value_type::second, _1))); 
}
// Detach the View by discarding the supplied pointer from the container of Views
// - no updates sent to it thereafter.
void Model::detach(shared_ptr<View> view_in)
{
	views.erase(view_in);
}
// notify the views about an object's location
void Model::notify_location(const string& name, Point location)
{
	for_each(views.begin(), views.end(), 
		bind(&View::update_location, _1, name, location));
}
// notify the view about an object's health change 
void Model::notify_health(const string& name, double health)
{
	for_each(views.begin(), views.end(), 
		bind(&View::update_health, _1, name, health));
}
// notify the view about an object's food carry change
void Model::notify_food_carry(const string& name, double food_carry)
{
	for_each(views.begin(), views.end(), 
		bind(&View::update_food_carry, _1, name, food_carry));
}
// notify the views that an object is now gone
void Model::notify_gone(const string& name)
{
	for_each(views.begin(), views.end(), 
		bind(&View::update_remove, _1, name));
}

bool Model::is_group_present(const string& name) const {
	return group_pool.find(name) != group_pool.end();
}

void Model::add_group(const string& name, shared_ptr<Group> group_ptr) {
	group_pool[name] = group_ptr;
}

void Model::remove_group(const std::string& name) {
	auto group_it = group_pool.find(name);
	if (group_it == group_pool.end()) {
		return;
	}
	group_it->second->set_parent(shared_ptr<Unit>());
	group_pool.erase(group_it);
}

shared_ptr<Group> Model::get_group_ptr(const string& name) const {
	auto group_it = group_pool.find(name);
	if (group_it == group_pool.end()) {
		throw (Error("Group not found!"));
	}
	return group_it->second;
}
	
