#include "Constants.h"
#include "SwiftController.h"


void SwiftController::waitForEnter() {
	std::cout << "Press Enter to Continue" << std::endl;
    std::string temp;
    std::getline(std::cin, temp);
	std::cin.clear(); // eat anything remaining
}

SwiftController::SwiftController(std::string port):
    swift(port)
{

}

// static helper
void SwiftController::enumerate_ports()
{
	std::vector<PortInfo> devices_found = list_ports();

	std::vector<PortInfo>::iterator iter = devices_found.begin();

	while (iter != devices_found.end())
	{
		PortInfo device = *iter++;
		std::cout << "port: " << device.port << ", desc: " << device.description << ", hardware_id: " << device.hardware_id << std::endl;
	}
}

bool SwiftController::connected(){
    return swift.connected;
}

bool SwiftController::load_positions(const std::string& file){
    return positions.load_positions_from_file(file);
}

bool SwiftController::save_positions(const std::string& file){
    return positions.save_positions_to_file(file);
}

void SwiftController::calibrate_positions(const std::vector<std::string>& position_list){
    // Recording 10 positions
    for (const std::string& pos : position_list) {

        // Prompt user to move the swift to position
        std::cout << "Move the swift to position " << pos << '\n';

        // Wait for user input (Enter key)
        waitForEnter();

        // Get the current position of the swift
		std::vector<float> new_position = swift.get_position(true);
        if ( new_position.empty() ) {
            std::cerr << "Error reading swift position!" << std::endl;
            break;
        }

        // Store the position in the vector
        SwiftPositions::Position position(new_position[0], new_position[1], new_position[2]);
        positions.update_position(pos, position);

        // Output the recorded position
        std::cout << "Position " << pos 
            << " recorded: (X: " << new_position[0] 
            << ", Y: " << new_position[1] 
            << ", Z: " << new_position[2] << ")" << '\n';
    }
}

void SwiftController::calibrate_bays(){

    std::vector<std::string> positions = 
        {bay_0, bay_1, bay_2, bay_3, bay_4, bay_5, bay_6, bay_7, bay_8, bay_9};
    calibrate_positions(positions);
}

void SwiftController::calibrate_home(){
    std::vector<std::string> positions = {home};
    calibrate_positions(positions);

}

void SwiftController::calibrate_time(){
    std::vector<std::string> positions = {time_H, time_h, time_M,  time_m};
    calibrate_positions(positions);
}


void SwiftController::move_through_positions(bool wait_for_enter){
    wait_for_enter = wait_for_enter ? true : false;
}

void SwiftController::pump_on(){
    std::cout << "Turning on the suction head..." << std::endl;
    
    int result = swift.set_pump(true);  // Turn the pump on
    // The pump takes a few milliseconds before the effect is realized.
    // So, we pause briefly here.
    sleep_milliseconds(200);

    if (result == 0) {
        std::cout << "Suction head is ON." << std::endl;
    } else {
        std::cerr << "Failed to turn on the suction head! Error code: " << result << std::endl;
    }
}

void SwiftController::pump_off(){
    std::cout << "Turning off the suction head..." << std::endl;

    int result = swift.set_pump(false);  // Turn the pump off
    // Similar to the pause at pump_on
    // what frequqnelty happens is that we move to a position, 
    // turn off the pump, then immediately move again.
    // So, we pause briefly here.
    sleep_milliseconds(200);
    
    if (result == 0) {
        std::cout << "Suction head is OFF." << std::endl;
    } else {
        std::cerr << "Failed to turn off the suction head! Error code: " << result << std::endl;
    }
} 

void SwiftController::go_pos(const std::string& pos_name){
    
    SwiftPositions::Position position = positions.retrieve_position(pos_name);
    auto [x, y, z] = position;
    
    int result = swift.set_position(x, y, z, 10000L, false, true);  // wait is true

    // Check if the movement was successful
    if (result != 0) {
        std::cerr << "Error moving UArm to home position. Error code: " << result << std::endl;
    }
}

// Lift the head just a little
void SwiftController::lift(){
    std::vector<float> posn = swift.get_position(true);
    float x(posn[0]), y(posn[1]), z(posn[2]+50.0);
    swift.set_position(x, y, z, 10000L, false, true);
    sleep_milliseconds(200); 

}

// lower the head just a little
void SwiftController::lower(){
    std::vector<float> posn = swift.get_position(true);
    float x(posn[0]), y(posn[1]), z(posn[2]-5.0);
    swift.set_position(x, y, z, 10000L, false, true); 
    sleep_milliseconds(200);

}

void SwiftController::move_tile(const std::string& from, const std::string& to ){

    // A tile is moved via the sequence: 

    // whereever we happen to be, lift the head just a little
    // to avoid disturbing anything
    lift();  

    // grab the thing at "from"
    go_pos(from);
    lower();
    pump_on();
    lift();

    // move the thing "to" and drop it
    go_pos(to);
    lower();
    pump_off();
    lift(); 
}