#pragma once

#include <variant>
#include <optional>
#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "Constants.h"

class SwiftPositions {

    // can't use it until it's defined
    // but, prefer to have public at the bottom
    // annoying !
public:
    using Position = std::tuple<float, float, float>;

private:  
    using PositionMap = std::map<std::string, Position>;

    // A map to hold the various positions we need to access.
    // Spent a lot of time wondering about enumerated types
    // and mapping them to strings - essentially a waste of time.
    // Ultimately, a map from a string to a value is required
    // no matter how you try to solve it.
    PositionMap posMap;

    const size_t POS_COUNT = {15}; // total positions stored: home (1), bay_0->bay_9 (10), HH:MM locations (4)
    const Position INVALID_POSITION = {-1.0f, -1.0f, -1.0f}; // detect an uninitialized posiition ?
    const Position home_default = {225.0f, 00.0f, 150.0f};  // position default

public:

    SwiftPositions();
    
    bool save_positions_to_file(const std::string& filename);
    bool load_positions_from_file(const std::string& filename);

    void update_position(const std::string& pos_name, const Position& pos);
    const Position& retrieve_position(const std::string& pos_name);

    // List of 15 locations we need to use
    using PositionNames = std::vector<std::string>;
    static const PositionNames get_position_names() { return {
        home,
        bay_0, bay_1, bay_2, bay_3, bay_4, bay_5, bay_6, bay_7, bay_8, bay_9,
        time_H, time_h, time_M, time_m };
    };

};

