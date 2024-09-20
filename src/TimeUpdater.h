#pragma once

#include <vector>
#include <iostream>

#include "Constants.h"

class TimeUpdater{

    std::vector<int> current_time_digits;

public:
    TimeUpdater(int hours = 0, int minutes = 0);

    // Set the current time
    void setTime(int hours, int minutes);
    
    
    using ChangeInfo = std::vector<std::pair<int, std::pair<int, int>>>;
    // Method to update time and return the changes in the form of (position, from, to)
    ChangeInfo updateTime(int new_hours, int new_minutes);
        
    // Function to get the current system time in hours and minutes
    std::pair<int, int> getCurrentSystemTime();

    // Method to display (what we think) the current time in HH:MM format
    void displayCurrentTime() const ;

};
