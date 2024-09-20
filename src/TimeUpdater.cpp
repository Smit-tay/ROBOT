#include <chrono>  // for time functions
#include <ctime>   // for std::tm

#include "TimeUpdater.h"

TimeUpdater::TimeUpdater(int hours, int minutes){
    setTime(hours, minutes);
}

// Set the current time
void TimeUpdater::setTime(int hours, int minutes) {
    current_time_digits = {
        hours / 10, hours % 10,  // Two digits for hours
        minutes / 10, minutes % 10,  // Two digits for minutes
    };
}

    
// Method to update time and return the changes in the form of (position, from, to)
TimeUpdater::ChangeInfo TimeUpdater::updateTime(int new_hours, int new_minutes) {

    std::vector<int> new_time_digits = {
        new_hours / 10, new_hours % 10,  // Two digits for new hours
        new_minutes / 10, new_minutes % 10  // Two digits for new minutes
    };

    // This will store the positions that changed and their old and new values
    ChangeInfo changes;

    // Compare each digit and track which positions changed
    for (std::size_t i = 0; i < current_time_digits.size(); ++i) {
        if (current_time_digits[i] != new_time_digits[i]) {
            // we don't want to store a size_t here, 
            // because we must interpret the values as zero or greater
            changes.push_back({static_cast<int>(i), {current_time_digits[i], new_time_digits[i]}});
        }
    }

    // Update the current time digits with the new time
    current_time_digits = new_time_digits;

    return changes;
}

std::pair<int, int> TimeUpdater::getCurrentSystemTime() {
    using namespace std::chrono;

    // Get the current time as a time_point
    auto now = system_clock::now();
    std::time_t current_time = system_clock::to_time_t(now);

    // Convert to a tm struct
    std::tm* local_time = std::localtime(&current_time);

    // Extract hours and minutes
    int hours = local_time->tm_hour;
    int minutes = local_time->tm_min;

    return {hours, minutes};
}

    
// Method to display the current time in HH:MM format
void TimeUpdater::displayCurrentTime() const {
    std::cout << "Current Time: "
                << current_time_digits[0] << current_time_digits[1] << ":"
                << current_time_digits[2] << current_time_digits[3] << std::endl;
}
