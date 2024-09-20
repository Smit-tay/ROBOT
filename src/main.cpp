#include <iostream>
#include <exception>

#include "SwiftController.h"
#include "SwiftPositions.h"
#include "TimeUpdater.h"
#include "UIManager.h"

// global for now
TimeUpdater time_updater;

int get_choice(){
    int choice = 0;
    std::cin >> choice;
	std::cin.ignore(); // eat the newline
    return choice;
}

void waitForEnter() {
	std::cout << "Press Enter to Continue" << std::endl;
    std::string temp;
    std::getline(std::cin, temp);
	std::cin.clear(); // eat anything remaining
}



void setInitialTime(){
    std::cout << "\n=== Set Initial Time ===" << '\n';
    std::cout << "\nEnter Hours and minutes seperated by a space" << '\n';

    std::string line;
    std::getline(std::cin, line);
    // Create a string stream to parse the line
    std::istringstream iss(line);
    int hour, minute;
    // Extract two integers from the stream
    if (iss >> hour >> minute) {
        time_updater.setTime(hour, minute);
    }
}

TimeUpdater::ChangeInfo updateTime(){

    auto [current_hours, current_minutes] = time_updater.getCurrentSystemTime();
    
    // Simulate updating the time to a new value, for example 03:41
    auto changes = time_updater.updateTime(current_hours, current_minutes);

    return changes;
}

// Function to map digit values to bay strings (e.g., "bay_0" for digit 0)
std::string getTimeLocationString(int position) {
    switch (position) {
        case 0: return "time_H"; // Hour tens place
        case 1: return "time_h"; // Hour ones place
        case 2: return "time_M"; // Minute tens place
        case 3: return "time_m"; // Minute ones place
        default: return "unknown"; // Safety check
    }
}
// Function to map digit values to bay strings (e.g., "bay_0" for digit 0)
std::string getBayString(int digit) {
    return "bay_" + std::to_string(digit);
}
void manifestChange(SwiftController& controller, TimeUpdater::ChangeInfo changes){

    // what we get is a vector which indicates the locations which need to change.
    // The change indicates the current value and the new value

    for (const auto& change : changes) {
        
        // extract the time_location, the current digit there, and the required digit there
        int position = change.first;               // Time position (0, 1, 2, 3)
        int from_digit = change.second.first;      // Current digit
        int to_digit = change.second.second; 
        
        // Map time position to time location string (e.g., time_H, time_M)
        std::string time_location = getTimeLocationString(position);

        // Map the digits to bay strings (e.g., bay_3, bay_4)
        // these names are a little confusing, so Watch out !
        std::string from_bay = getBayString(from_digit);
        std::string to_bay = getBayString(to_digit);
       
       // Construct the output string and display it
        std::cout << time_location << ": change from " << from_digit << " to " << to_digit << std::endl;
  
        controller.move_tile(time_location, from_bay);
        controller.move_tile(to_bay, time_location);
    }

}

// testing function - let's the user moves tiles 
void move_tile(SwiftController& controller) {

    std::cout << "\n=== Move Tile ===" << '\n';
    std::cout << "\nEnter from and to position seperated by a space" << '\n';

    int i=0;
    for( const std::string& pos_name : SwiftPositions::get_position_names()){
        std::cout << i++ << ".) " << pos_name << '\n';
    }

    std::string line;
    std::getline(std::cin, line);
    // Create a string stream to parse the line
    std::istringstream iss(line);
    int from, to;
    // Extract two integers from the stream
    if (iss >> from >> to) {
       SwiftPositions::PositionNames names = SwiftPositions::get_position_names();
       controller.move_tile(names[from], names[to]);
    }
}

void handle_menu_choice(SwiftController& controller) {

    int choice = get_choice();

    switch (choice) {
		case 0:
            controller.calibrate_bays();
            controller.calibrate_home();
            controller.calibrate_time();
            break;
        case 1:
            // Calibrated ? save to file            
            controller.save_positions("positions.txt");
            break;
        case 2:
            // Load saved positions and move swift
            controller.load_positions("positions.txt");
            break;
        case 3:
            // Turn suction head ON
            controller.pump_on();
            break;
        case 4:
            // Turn suction head OFF
            controller.pump_off();
            break;
        case 5:
            setInitialTime();
            break;
        case 6:
            {
            TimeUpdater::ChangeInfo change = updateTime();
            manifestChange(controller, change);
            }
            break;
        case 9:
            std::cout << "Quitting..." << std::endl;
            exit(0);
            break;
        case 99:
            move_tile(controller);
            break;
        default:
            std::cout << "Invalid choice! Try again." << std::endl;
    }
}

void display_menu() {
	
    std::cout << "\n=== swift Application Menu ===" << '\n';
    std::cout << "0. Calibrate and Set Home Position" << '\n';
    std::cout << "1. Save positions to file" << '\n';
    std::cout << "2. Load Saved Positions and (optionally) Execute Movements" << '\n';
    std::cout << "3. Turn Suction Head ON" << '\n';
    std::cout << "4. Turn Suction Head ON" << '\n';
    
    std::cout << "5. Set Initial Time" << '\n';
    std::cout << "6. Set New Time" << '\n';

    std::cout << "9. Quit" << '\n';
    std::cout << "99. Move a tile from A to B - prompted\n";
    std::cout << "Enter your choice: ";
}

int main(int argc, char **argv) {
	
	if (argc < 2) {
		// Print usage
		std::cout << "Usage: test_swift [-e | {<serial port address>} ]" << std::endl;
		return -1;
	}
	std::string arg1(argv[1]);
	std::string port = "";
		
	if (arg1 == "-e") {
		SwiftController::enumerate_ports();
		return 0;
	}
	else {
		port = arg1;
	}
	

	SwiftController controller(port);
	sleep_milliseconds(2000);
	if (!controller.connected()) {
		return -2;
	}

	controller.go_home();

	// Main loop for handling menu choices
    while (true) {
        display_menu();
        handle_menu_choice(controller);
    }

}
