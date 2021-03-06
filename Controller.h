/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/


#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <map>
#include <vector>
#include <memory>
class Unit;
class View;
class Grid_view;
class Controller {
public:	
	Controller();

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();
private:
	typedef void (Controller::*general_command_function)(); // got model as global
	typedef void (Controller::*agent_command_function)(std::shared_ptr<Unit> agent_ptr);
	using command_map_t = std::map<std::string, general_command_function>;

	command_map_t view_commands;
	command_map_t general_commands;
	command_map_t group_commands;
	std::map<std::string, agent_command_function> agents_commands;

	// Command set
	// View commands
	void command_open();
	void command_close();
	void command_default();
	void command_size();
	void command_zoom();
	void command_pan();
	// Agent commands
	void command_move(std::shared_ptr<Unit>);
	void command_work(std::shared_ptr<Unit>);
	void command_attack(std::shared_ptr<Unit>);
	void command_stop(std::shared_ptr<Unit>);
	void command_join_group(std::shared_ptr<Unit>);
	void command_leave_group(std::shared_ptr<Unit>);
	// General commands
	void command_go();
	void command_show();
	void command_status();
	void command_build();
	void command_train();
	// set of group commands 
	void command_create_group();
	void command_disband_group();

	// Views
	using view_map_name_t = std::map<std::string, std::shared_ptr<View>>;
	view_map_name_t name_View_map;
	// A container to keep the opened order of the Views
	using view_map_insert_ordered_t = std::vector<std::shared_ptr<View> >;
	view_map_insert_ordered_t  order_View_map;
	// View factory creation helper
	// Create and return the pointer to the specified View type. If the type
	// is unrecognized, throws Error("Trying to create view of unknown type!")

	std::shared_ptr<View> create_View(std::string& type);

	// Map View accessor 
	// helper accessor for the map or agriculture view
	std::shared_ptr<Grid_view> get_map_or_agri_view();

};
#endif
