#include "Controller.h"

#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>
#include <functional>
#include "Model.h"
#include "Group.h"
#include "Map_view.h"
#include "View.h"
#include "View_factory.h"

#include "Agent_factory.h"
#include "Structure_factory.h"
#include "Agent.h"
#include "Geometry.h"
#include "Utility.h"

using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::mem_fn;
using std::dynamic_pointer_cast;
/* refractored error message */
const char* invalid_object_name_msg = "Invalid name for new object!";
/* internal helpers */
int read_int();
double read_double();
string read_object_name();

Controller::Controller() :
agent_commands
{
	{ "move", &Controller::command_move },
	{ "work", &Controller::command_work },
	{ "attack", &Controller::command_attack },
	{ "stop", &Controller::command_stop }
},
general_commands
{
	{ "status", &Controller::command_status },
	{ "show", &Controller::command_show },
	{ "go", &Controller::command_go },
	{ "build", &Controller::command_build },
	{ "train", &Controller::command_train }
},
group_commands
{
	{ "group", &Controller::command_group },
	{ "add", &Controller::command_add },
	{ "remove", &Controller::command_remove },
	{ "disband", &Controller::command_disband },
},
view_commands
	{
		{ "open", &Controller::command_open },
		{ "close", &Controller::command_close },
		{ "default", &Controller::command_default },
		{ "size", &Controller::command_size },
		{ "zoom", &Controller::command_zoom },
		{ "pan", &Controller::command_pan }
} {}

// create View object, run the program by acccepting user commands, then destroy View object
void Controller::run()
{
	
	while(1)
	{
		try
		{
			cout << "\nTime " << Model::get_instance().get_time() << ": Enter command: ";
			string first_word;
			cin >> first_word;
			if (first_word == "quit")
			{
				cout << "Done" << endl;
				break;
			}
			else if (Model::get_instance().is_agent_present(first_word)) // true if agent exist
			{
				shared_ptr<Agent> tmp_agent = Model::get_instance().get_agent_ptr(first_word); // noexcept
				assert(tmp_agent->is_alive());
	
				string action;
				cin >> action;
				if (agent_commands.find(action) != agent_commands.end())
					(this->*agent_commands[action])(tmp_agent);
				else
					throw Error("Unrecognized command!");
				
			}
			else if (general_commands.find(first_word) != general_commands.end())
			{
				(this->*general_commands[first_word])();
			}
			else if (view_commands.find(first_word) != view_commands.end())
			{
				(this->*view_commands[first_word])();
			}
			else if (group_commands.find(first_word) != group_commands.end())
			{
				(this->*group_commands[first_word])();
			}
			else
			{
				throw Error("Unrecognized command!");
			}
		}
		catch(std::exception& e)
		{
			cout << e.what() << endl;
			cin.clear();
			while(cin.get() != '\n');
		}
		catch (...)
		{
			cout << "Unknown exception caught!" << endl;
			assert(0);
		}
	}
	
}

void Controller::command_open()
{
	string view_name;
	cin >> view_name;
	if (name_View_map.find(view_name) != name_View_map.end())
		throw Error("View of that name already open!");
	// add into map and attach
	// maintain two container
	shared_ptr<View> new_View = create_view(view_name);
	name_View_map.emplace(view_name, new_View);
	order_View_map.push_back(new_View);
	Model::get_instance().attach(new_View);
}

void Controller::command_close()
{
	string view_name;
	cin >> view_name;
	auto close_View_iter = name_View_map.find(view_name);
	if (close_View_iter == name_View_map.end())
		throw Error("No view of that name is open!");
	
	// maintain two container
	// find the thing first 
	shared_ptr<View> close_View = close_View_iter->second; // create a view to avoid erase order problem
	auto order_view_iter = find(order_View_map.begin(), order_View_map.end(), close_View);

	assert(order_view_iter != order_View_map.end()); 
	// erase two together 
	order_View_map.erase(order_view_iter);
	name_View_map.erase(view_name);
	Model::get_instance().detach(close_View);
	
}


void Controller::command_default()
{
	shared_ptr<Map_view> map_view = get_map_View();

	map_view->set_defaults();
}

void Controller::command_size()
{
	shared_ptr<Map_view> map_view = get_map_View();
	int map_size = read_int();

	map_view->set_size(map_size);
}

void Controller::command_zoom()
{

	shared_ptr<Map_view> map_view = get_map_View();
	
	double map_scale = read_double();
	map_view->set_scale(map_scale);
}

void Controller::command_pan()
{
	shared_ptr<Map_view> map_view = get_map_View();
	double x = read_double();
	double y = read_double();
	map_view->set_origin(Point(x, y));
}

void Controller::command_move(shared_ptr<Agent> agent_ptr)
{

	double x = read_double();
	double y = read_double();
	
	agent_ptr->move_to(Point(x, y));

}
void Controller::command_work(shared_ptr<Agent> agent_ptr)
{

	string source;
	cin >> source;
	shared_ptr<Structure> source_structure = Model::get_instance().get_structure_ptr(source);

	string destination;
	cin >> destination;
	shared_ptr<Structure> dest_structure = Model::get_instance().get_structure_ptr(destination);

	agent_ptr->start_working(source_structure, dest_structure);
}

void Controller::command_attack(shared_ptr<Agent> agent_ptr)
{
	string target_name;
	cin >> target_name;
	shared_ptr<Agent> target_agent = Model::get_instance().get_agent_ptr(target_name);
	agent_ptr->start_attacking(target_agent);
}

void Controller::command_stop(shared_ptr<Agent> agent_ptr)
{
	agent_ptr->stop();
}


void Controller::command_go()
{
	Model::get_instance().update();
}


void Controller::command_show()
{
	for_each(order_View_map.begin(), order_View_map.end(), 
		mem_fn(&View::draw));	
}

void Controller::command_status()
{
	Model::get_instance().describe();
}

void Controller::command_build()
{
	string object_name = read_object_name();
	string object_type;
	cin >> object_type;
	double x = read_double();
	double y = read_double();
	Model::get_instance().add_structure(
		create_structure(object_name, object_type, Point(x, y)));



}

void Controller::command_train()
{
	string object_name = read_object_name();
	string object_type;
	cin >> object_type;
	double x = read_double();
	double y = read_double();
	Model::get_instance().add_agent(
		create_agent(object_name, object_type, Point(x, y)));
}

void Controller::command_group() 
{
	string group_name = read_object_name();
	Model::get_instance().add_group(group_name, make_shared<Group>());
}
void Controller::command_add() 
{
	string group_name, unit_name;
	cin >> group_name;
	shared_ptr<Group> group_ptr = Model::get_instance().get_group_ptr(group_name);
	cin >> unit_name;
	shared_ptr<Unit> unit_ptr;
	if (Model::get_instance().is_group_present(unit_name))
		unit_ptr = Model::get_instance().get_group_ptr(unit_name);
	else if (Model::get_instance().is_agent_present(unit_name))
		unit_ptr = Model::get_instance().get_agent_ptr(unit_name);
	else
		throw (Error("No unit with that name!"));
	group_ptr->add_component(unit_ptr);
}
void Controller::command_remove() 
{
	string group_name, unit_name;
	cin >> group_name;
	shared_ptr<Group> group_ptr = Model::get_instance().get_group_ptr(group_name);
	cin >> unit_name;
	shared_ptr<Unit> unit_ptr;
	if (Model::get_instance().is_group_present(unit_name))
		unit_ptr = Model::get_instance().get_group_ptr(unit_name);
	else if (Model::get_instance().is_agent_present(unit_name))
		unit_ptr = Model::get_instance().get_agent_ptr(unit_name);
	else
		throw (Error("No unit with that name!"));
	group_ptr->remove_component(unit_ptr);
}
void Controller::command_disband() 
{
	string group_name;
	cin >> group_name;
	Model::get_instance().remove_group(group_name);
}


// Get the map view through dynamic cast
shared_ptr<Map_view> Controller::get_map_View()
{
	auto map_view = name_View_map.find("map");
	if (map_view == name_View_map.end())
		throw Error("No map view is open!");
	return dynamic_pointer_cast<Map_view>(map_view->second);
}


int read_int()
{
	int x;
	cin >> x;
	if (!cin.good())
		throw Error("Expected an integer!");
	else 
		return x; 
}
double read_double()
{
	double x;
	cin >> x;
	if (!cin.good())
		throw Error("Expected a double!");
	else 
		return x; 
}

string read_object_name()
{
	string object_name;
	cin >> object_name;
	if (object_name.size() < 2)
		throw Error(invalid_object_name_msg);
	for (char c : object_name)
		if (!isalnum(c))
			throw Error(invalid_object_name_msg);
	if(Model::get_instance().is_name_in_use(object_name))
		throw Error(invalid_object_name_msg);
	return object_name;
}

