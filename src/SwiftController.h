#include "uarm/uarm.h"
using namespace uarm;

#include "SwiftPositions.h"

class SwiftController {

    Swift swift;
    SwiftPositions positions;

    void calibrate_positions(const std::vector<std::string>& position_list);
    void waitForEnter();

public:
    SwiftController(std::string port);
    
    ~SwiftController(){
        if(connected()) 
            swift.disconnect();
    }

    static void enumerate_ports();
    bool connected();

    bool load_positions(const std::string& file);
    bool save_positions(const std::string& file);
    
    void calibrate_bays();
    void calibrate_home();
    void calibrate_time();

    // can be used to "display"" loaded positions
    void move_through_positions(bool wait_for_enter = false);

    void pump_on(); 
    void pump_off(); 

    void go_home()  { go_pos(home); }
    void go_pos(const std::string& pos_name);
    void lift();
    void lower();

    void move_tile(const std::string& from, const std::string& to );

};