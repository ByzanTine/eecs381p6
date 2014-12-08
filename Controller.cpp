#include "Controller.h"

#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>
#include <functional>
#include "Model.h"
#include "Group.h"
#include "Grid_view.h"
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
shared_ptr<Group> get_group_input();

Controller::Controller()
{
	agents_commands = 
	{
		{ "move", &Controller::command_move },
		{ "work", &Controller::command_work },
		{ "attack", &Controller::command_attack },
		{ "stop", &Controller::command_stop },
		{ "join", &Controller::command_join_group },
		{ "leave", &Controller::command_leave_group }
	};
	general_commands =
	{
		{ "status", &Controller::command_status },
		{ "show", &Controller::command_show },
		{ "go", &Controller::command_go },
		{ "build", &Controller::command_build },
		{ "train", &Controller::command_train }
	};
	group_commands = 
	{
		{ "group", &Controller::command_create_group },
		{ "disband", &Controller::command_disband_group }
	};
	view_commands = 
	{
		{ "open", &Controller::command_open },
		{ "close", &Controller::command_close },
		{ "default", &Controller::command_default },
		{ "size", &Controller::command_size },
		{ "zoom", &Controller::command_zoom },
		{ "pan", &Controller::command_pan }
	};
}

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
			if (Model::get_instance().is_unit_present(first_word))
			{
				shared_ptr<Unit> tmp_unit = Model::get_instance().get_unit_ptr(first_word);
				string action;
				cin >> action;
				if (agents_commands.find(action) != agents_commands.end())
					(this->*agents_commands[action])(tmp_unit);
				else
					throw Error("Unrecognized command!");
				continue;
				
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

// only works for map and agriculture
// throw exception otherwise
void Controller::command_default()
{
	
	shared_ptr<Grid_view> grid_view = get_map_or_agri_view();

	grid_view->set_defaults();
}

void Controller::command_size()
{
	shared_ptr<Grid_view> grid_view = get_map_or_agri_view();
	int map_size = read_int();

	grid_view->set_size(map_size);
}

void Controller::command_zoom()
{

	shared_ptr<Grid_view> grid_view = get_map_or_agri_view();
	
	double map_scale = read_double();
	grid_view->set_scale(map_scale);
}

void Controller::command_pan()
{
	shared_ptr<Grid_view> grid_view = get_map_or_agri_view();
	double x = read_double();
	double y = read_double();
	grid_view->set_origin(Point(x, y));
}

void Controller::command_move(shared_ptr<Unit> unit_ptr)
{

	double x = read_double();
	double y = read_double();
	
	unit_ptr->move_to(Point(x, y));

}
void Controller::command_work(shared_ptr<Unit> unit_ptr)
{

	string source;
	cin >> source;
	shared_ptr<Structure> source_structure = Model::get_instance().get_structure_ptr(source);

	string destination;
	cin >> destination;
	shared_ptr<Structure> dest_structure = Model::get_instance().get_structure_ptr(destination);

	unit_ptr->start_working(source_structure, dest_structure);
}

void Controller::command_attack(shared_ptr<Unit> unit_ptr)
{
	string target_name;
	cin >> target_name;
	shared_ptr<Agent> target_agent = Model::get_instance().get_agent_ptr(target_name);
	unit_ptr->start_attacking(target_agent);
}

void Controller::command_stop(shared_ptr<Unit> unit_ptr)
{
	unit_ptr->stop();
}

void Controller::command_join_group(shared_ptr<Unit> unit_ptr) 
{
	shared_ptr<Group> group_ptr = get_group_input();
	group_ptr->add_component(unit_ptr);
}
void Controller::command_leave_group(shared_ptr<Unit> unit_ptr) 
{
	shared_ptr<Group> group_ptr = get_group_input();
	group_ptr->remove_component(unit_ptr);
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

void Controller::command_create_group() 
{
	string group_name = read_object_name();
	Model::get_instance().add_group(make_shared<Group>(group_name));
}


void Controller::command_disband_group() 
{
	string group_name;
	cin >> group_name;

	Model::get_instance().remove_group(group_name);
}



// a helper function to obtain map or agri view
shared_ptr<Grid_view> Controller::get_map_or_agri_view()
{
	string view_name;
	cin >> view_name;
	if (view_name == "map" || view_name == "agriculture")
	{
		auto grid_view = name_View_map.find(view_name);
		if (grid_view == name_View_map.end())
			throw Error("No " + view_name + " view is open!");
		return dynamic_pointer_cast<Grid_view>(grid_view->second);
	}
	else
	{
		throw Error("Not a map or agriculture View");
	}
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

shared_ptr<Group> get_group_input()
{
	string group_name;
	cin >> group_name;
	shared_ptr<Group> group_ptr = Model::get_instance().get_group_ptr(group_name);
	return group_ptr;
}
