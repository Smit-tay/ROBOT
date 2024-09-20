#include <fstream>
#include <sstream>

#include "SwiftPositions.h"


SwiftPositions::SwiftPositions()
{
    // List of 15 locations we need to use
    const std::vector<std::string> position_names = get_position_names();

    // initialize the position map with the position names
    for (const auto& name : position_names) {
        posMap[name] = INVALID_POSITION;
    }
    // always provide the home position
    // the arm moves here upon connection.
    posMap[home] = home_default;
}
    
// replaces a position in the map 
void SwiftPositions::update_position(const std::string& pos_name, const Position& pos){

    PositionMap::iterator iter = posMap.find(pos_name);
    if (iter != posMap.end()){
        iter->second = pos;
    } else {
        // There's a problem !
        std::cout << "ERROR: " << pos_name << " not found in position map !\n";
    }
}

const SwiftPositions::Position& SwiftPositions::retrieve_position(const std::string& pos_name){

    PositionMap::iterator iter = posMap.find(pos_name);
    if (iter != posMap.end()){
        return iter->second;
    } else {
        // There's a problem !
        std::cout << "ERROR: " << pos_name << " not found in position map !\n";
    }
    return INVALID_POSITION;
}

// Does what it says
bool SwiftPositions::save_positions_to_file(const std::string& filename){

    bool ret = false;
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& [key, pos] : posMap) {
            auto [x, y, z] = pos;
            // structured bindings ROCK !
            file << key << " " << x << " " << y << " " << z << "\n";
        }
        file.close();
        ret = true;
        std::cout << "Positions saved to file: " << filename << std::endl;
    } else {
        std::cerr << "Error opening file for saving positions!" << std::endl;
    }
    return ret;
}

// Does what it says
bool SwiftPositions::load_positions_from_file(const std::string& filename){

    bool ret = true;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string label;
            float x, y, z;
            if (!(iss >> label >>  x >> y >> z)) { 
                std::cerr << "Error reading position data from file!" << std::endl; 
                ret = false;
                break; 
            }
            update_position(label, Position(x,y,z));
        }
        file.close();
        if(ret){
            std::cout << "Positions loaded from file: " << filename << std::endl;
        }
    } else {
        std::cerr << "File not found. Calibration is required!" << std::endl;
        ret = false;
    }
    return ret;
}
