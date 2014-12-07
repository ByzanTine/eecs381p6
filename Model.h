/*
Model is part of a simplified Model-View-Controller pattern.
Model keeps track of the Sim_objects in our little world. It is the only
component that knows how many Structures and Agents there are, but it does not
know about any of their derived classes, nor which Agents are of what kind of Agent. 
It has facilities for looking up objects by name, and removing Agents.  When
created, it creates an initial group of Structures and Agents using the Structure_factory
and Agent_factory.
Finally, it keeps the system's time.

Controller tells Model what to do; Model in turn tells the objects what do, and
when asked to do so by an object, tells all the Views whenever anything changes that might be relevant.
Model also provides facilities for looking up objects given their name.

Notice how only the Standard Library headers need to be included - reduced coupling!

*/


#ifndef MODEL_H
#define MODEL_H
#include <map>
#include <set>
#include <string>
#include <memory>

class Sim_object;

class Structure; 
class Agent;
class Point;
class Group;
class Unit;

class View; // temp

class Model {
public:
	static Model& get_instance();

	// return the current time
	int get_time() {return time;}

	// is name already in use for either agent or structure?
    // return true if the name matches the name of an existing agent or structure
	bool is_name_in_use(const std::string& name) const;
	// is there a structure with this name?
	bool is_structure_present(const std::string& name) const;
	// add a new structure; assumes none with the same name
	void add_structure(std::shared_ptr<Structure>);
	// will throw Error("Structure not found!") if no structure of that name
	std::shared_ptr<Structure> get_structure_ptr(const std::string& name) const;
	// find the nearest structure 
	std::shared_ptr<Structure> get_nearest_structure_ptr(const std::string& name) const;
	// is there an agent with this name?
	bool is_agent_present(const std::string& name) const;
	// add a new agent; assumes none with the same name
	void add_agent(std::shared_ptr<Agent>);
	// will throw Error("Agent not found!") if no agent of that name
	std::shared_ptr<Agent> get_agent_ptr(const std::string& name) const;
	// remove a agent from the pools given the agent_ptr
	void remove_agent(std::shared_ptr<Agent>);
	// find the nearest agent 
	std::shared_ptr<Agent> get_nearest_agent_ptr(const std::string& name) const;
	// return if a group is present in the model
	bool is_group_present(const std::string& name) const;
	// add a group, assumes none with the same name
	void add_group(const std::string& name, std::shared_ptr<Group> group_ptr);
	// remove a group from the model
	void remove_group(const std::string& name);
	// get a group by name, throw error if not found
	std::shared_ptr<Group> get_group_ptr(const std::string& name) const;
	// get a unit by name, throw error if not found
	bool is_unit_present(const std::string& name) const;
	// return if a unit is in Model 
	std::shared_ptr<Unit> get_unit_ptr(const std::string& name) const;
	// tell all objects to describe themselves to the console
	void describe() const;
	// increment the time, and tell all objects to update themselves
	void update();
	
	/* View services */
	// Attaching a View adds it to the container and causes it to be updated
    // with all current objects'location (or other state information.
	void attach(std::shared_ptr<View>);
	// Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
	void detach(std::shared_ptr<View>);
    // notify the views about an object's location changes
	void notify_location(const std::string& name, Point location);
	// notify the view about an object's health change 
	void notify_health(const std::string& name, double health);
	// notify the view about an object's food carry change
	void notify_food_carry(const std::string& name, double food_carry);
	// notify the views that an object is now gone
	void notify_gone(const std::string& name);
	
	// prevent copy/move construction and assignment
	Model(const Model&) = delete;
	Model& operator= (const Model&) = delete;
	Model(const Model&&) = delete;
	Model& operator= (const Model&&) = delete;

private:
	// an int member variable to store the simulated time
	// private constructor to make singleton
	Model();
	// destroy all objects
	~Model();
	int time;

	// container of objects
	using sim_object_pool_t = std::map<std::string, std::shared_ptr<Sim_object>>;
	sim_object_pool_t sim_object_pool;
	 
	std::map<std::string, std::shared_ptr<Agent> >  agent_pool;
	std::map<std::string, std::shared_ptr<Structure> > structure_pool;
	std::map<std::string, std::shared_ptr<Group> > group_pool;
	std::map<std::string, std::shared_ptr<Unit> > unit_pool;
	// container of Views
	std::set<std::shared_ptr<View>> views;
	
};

#endif