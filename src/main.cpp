#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

#include <random>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>


#define MAX_NUM_STEPS 3000



// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }



// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
    auto found_null = s.find("null");
    auto b1 = s.find_first_of("[");
    auto b2 = s.find_last_of("]");
    if (found_null != std::string::npos) {
        return "";
    }
    else if (b1 != std::string::npos && b2 != std::string::npos) {
        return s.substr(b1, b2 - b1 + 1);
    }
    return "";
}

class PIDTest {
public:
    double tau_p;
    double tau_d;
    double tau_i;
    double throttle;
    int num_steps;
    double mean_speed;
    double cte_tolerance;
    double max_abs_cte;
    
    friend std::ostream& operator<<(std::ostream& os, const PIDTest& pidTest);

    static std::vector<PIDTest> load(std::istream& is);
    static void save(std::string filename, std::vector<PIDTest>& pidTests);
    
};

std::ostream& operator<<(std::ostream& os, const PIDTest& pidTest) {
    os << pidTest.tau_p;
    os << pidTest.tau_d;
    os << pidTest.tau_i;
    os << pidTest.throttle;
    os << pidTest.num_steps;
    os << pidTest.mean_speed;
    os << pidTest.cte_tolerance;
    os << pidTest.max_abs_cte;
    return os;
}



std::vector<PIDTest> PIDTest::load(std::istream& is) {
    std::vector<PIDTest> result;

    std::string s;

    is >> s;

    while (is) {
        PIDTest pidTest;
        is >> pidTest.tau_p;
        is >> s;
        is >> pidTest.tau_d;
        is >> s;
        is >> pidTest.tau_i;
        is >> s;
        is >> pidTest.throttle;
        is >> s;
        is >> pidTest.num_steps;
        is >> s;
        is >> pidTest.mean_speed;
        is >> s;
        is >> pidTest.cte_tolerance;
        is >> s;
        is >> pidTest.max_abs_cte;
        is >> s;
        result.push_back(pidTest);
    }
    return result;
}

void PIDTest::save(std::string filename, std::vector<PIDTest>& pidTests) {
    std::ofstream fos(filename.c_str());
    fos << "tau_p;tau_d;tau_i;throttle;num_steps;mean_speed;cte_tolerante;max_abs_cte" << std::endl;
    std::for_each(pidTests.begin(), pidTests.end(),
                  [&fos](const PIDTest& pidTest) {
                      fos <<
                      pidTest.tau_p << ";" <<
                      pidTest.tau_d << ";" <<
                      pidTest.tau_i << ";" <<
                      pidTest.throttle << ";" <<
                      pidTest.num_steps << ";" <<
                      pidTest.mean_speed << ";" <<
                      pidTest.cte_tolerance << ";" <<
                      pidTest.max_abs_cte  << ";" <<
                      std::endl; }
                  );
    fos.close();
}

int main()
{
    uWS::Hub h;
    
    std::ifstream ifs("/Users/silviohohne/projeto/CarND-PID-Control-Project/pidTests.csv");
    std::vector<PIDTest> pidTests = PIDTest::load(ifs);
    
    std::for_each(pidTests.begin(), pidTests.end(), [](const PIDTest& pidTest) { std::cout << pidTest << std::endl;});

    /*
    PIDTest pidTest1;
    pidTest1.tau_p = 0.1;
    pidTest1.tau_d = 0.2;
    pidTest1.tau_i = 0.3;
    pidTest1.throttle = 0.4;
    pidTest1.num_steps = 100;
    pidTest1.mean_speed = 3.444;
    pidTest1.cte_tolerance = 2.0;
    pidTest1.max_abs_cte = 1.9;
    pidTests.push_back(pidTest1);
    
    pidTest1 = PIDTest();
    pidTest1.tau_p = 1.1;
    pidTest1.tau_d = 1.2;
    pidTest1.tau_i = 1.3;
    pidTest1.throttle = 1.4;
    pidTest1.num_steps = 200;
    pidTest1.mean_speed = 4.444;
    pidTest1.cte_tolerance = 3.0;
    pidTest1.max_abs_cte = 333.9;
    pidTests.push_back(pidTest1);

    PIDTest::save("/Users/silviohohne/projeto/CarND-PID-Control-Project/pidTests.txt", pidTests);
    */
    
    if (true) {
        return 0;
    }
    
    PID pid;
    // TODO: Initialize the pid variable.
    
    std::default_random_engine generator;
    std::normal_distribution<double> dist_steer(0, .06);
    
    
    const double tau_p =  .1;
    const double tau_d = 0.5;
    const double tau_i = 0.0;
    const double throttle = 0.25; // T = 1:28
    int step = 0;
    double sum_speed = 0;
    
    /*
     const double tau_p =  .1;
     const double tau_d = 0.5;
     const double tau_i = 0.0;
     const double throttle = 0.3; // T = 1:14
     */
    
    /*
     const double tau_p =  .1;
     const double tau_d = 0.5;
     const double tau_i = 0.0;
     const double throttle = 0.33; // T = 1:08
     */
    
    /*
     const double tau_p =  .1;
     const double tau_d = 0.5;
     const double tau_i = 0.0;
     const double throttle = 0.35; // T = 1:04
     */
    
    /*
     const double tau_p =  .12;
     const double tau_d = 0.64;
     const double tau_i = 0.0;
     const double throttle = 0.35; // T = 1:04
     */
    
    /*
     const double tau_p =  .1;
     const double tau_d = 1.5;
     const double tau_i = 0.0;
     const double throttle = 0.37; // T = 1:02
     */
    
    /*
     const double tau_p =  .1;
     const double tau_d = 1.5;
     const double tau_i = 0.0;
     const double throttle = 0.39; // T = 1:00
     */
    
    /*
     const double tau_p =  .1;
     const double tau_d = 1.5;
     const double tau_i = 0.0;
     const double throttle = 0.42; // T = 0:56
     */
    
    /*
     const double tau_p =  .1;
     const double tau_d = 1.7;
     const double tau_i = 0.0;
     const double throttle = 0.42; // T = 0:57 nao
     */
    
    /*
     const double tau_p =  .28;
     const double tau_d =  7.5;
     const double tau_i = 0.0000;
     const double throttle = 0.40;  // T = 1:25
     */
    
    /*
     const double tau_p =  .28;
     const double tau_d =  14.5;
     const double tau_i = 0.00;
     const double throttle = 0.50;  // T = 1:40
     */
    
    
    
    
    
    
    double max_abs_cte = 0.0;
    double cte_prev = 0.0;
    double cte_sum = 0.0;
    
    
    h.onMessage([&pid, &pidTests, &step, &max_abs_cte, &throttle, &tau_p, &tau_d, &tau_i, &cte_prev, &cte_sum](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
        // "42" at the start of the message means there's a websocket message event.
        // The 4 signifies a websocket message
        // The 2 signifies a websocket event
        if (length && length > 2 && data[0] == '4' && data[1] == '2')
        {
            auto s = hasData(std::string(data).substr(0, length));
            if (s != "") {
                auto j = json::parse(s);
                std::string event = j[0].get<std::string>();
                if (event == "telemetry") {
                    // j[1] is the data JSON object
                    double cte = std::stod(j[1]["cte"].get<std::string>());
                    double speed = std::stod(j[1]["speed"].get<std::string>());
                    double angle = std::stod(j[1]["steering_angle"].get<std::string>());
                    double steer_value;
                    
                    if (fabs(cte) > 4.0) { //} 2.0) {
                        cte_sum = 0;
                        step = 0;
                        std::string msg("42[\"reset\", {}]");
                        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
                    }
                    /*
                     * TODO: Calcuate steering value here, remember the steering value is
                     * [-1, 1].
                     * NOTE: Feel free to play around with the throttle and speed. Maybe use
                     * another PID controller to control the speed!
                     */
                    
                    steer_value = -tau_p * cte - tau_d * (cte - cte_prev) - tau_i * cte_sum;
                    
                    
                    // DEBUG
                    std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;
                    
                    json msgJson;
                    msgJson["steering_angle"] = steer_value;
                    msgJson["throttle"] = throttle; // 0.45; //0.3;
                    auto msg = "42[\"steer\"," + msgJson.dump() + "]";
                    std::cout << msg << std::endl;
                    ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
                    
                    cte_prev = cte;
                    cte_sum += cte;
                    if (fabs(cte) > max_abs_cte) max_abs_cte = fabs(cte);
                    std::cout << "max_abx_cte: " << max_abs_cte << " step: " << step << std::endl;  // max_abs_cte shoulb be less than 2.5
                    step++;
                    if (step > MAX_NUM_STEPS) {
                        cte_sum = 0;
                        step = 0;
                        std::string msg("42[\"reset\", {}]");
                        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
                    }
                }
            } else {
                // Manual driving
                std::string msg = "42[\"manual\",{}]";
                ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
            }
        }
    });
    
    // We don't need this since we're not using HTTP but if it's removed the program
    // doesn't compile :-(
    h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
        const std::string s = "<h1>Hello world!</h1>";
        if (req.getUrl().valueLength == 1)
        {
            res->end(s.data(), s.length());
        }
        else
        {
            // i guess this should be done more gracefully?
            res->end(nullptr, 0);
        }
    });
    
    h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
        std::cout << "Connected!!!" << std::endl;
    });
    
    h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
        ws.close();
        std::cout << "Disconnected" << std::endl;
    });
    
    int port = 4567;
    if (h.listen(port))
    {
        std::cout << "Listening to port " << port << std::endl;
    }
    else
    {
        std::cerr << "Failed to listen to port" << std::endl;
        return -1;
    }
    h.run();
}
