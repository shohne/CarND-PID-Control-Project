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


#define MAX_NUM_STEPS 9000
#define PID_TESTS_CVS_FILE "./result/pidTests.csv"
#define CTE_TOLERANCE   3.0
#define CENTER_TAU_P    0.137615
#define DELTA_TAU_P     0
//0.0025
#define CENTER_TAU_D    2.20015
#define DELTA_TAU_D     0
//0.024
#define CENTER_TAU_I    0.0000325
#define DELTA_TAU_I     0
//0.0000005
#define THROTTLE        0.570

#define SAVE            false

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

    PIDTest();
    
    PIDTest(    
        std::default_random_engine& generator, 
        std::uniform_real_distribution<double>& dist_tau_p,
        std::uniform_real_distribution<double>& dist_tau_d,
        std::uniform_real_distribution<double>& dist_tau_i,
        double throttle,
        double cte_tolerance
    );

    double tau_p;
    double tau_d;
    double tau_i;
    double throttle;
    int num_steps;
    double mean_speed;
    double cte_tolerance;
    double max_abs_cte;
    int result;
    
    friend std::ostream& operator<<(std::ostream& os, const PIDTest& pidTest);

    static std::vector<PIDTest> load(std::istream& is);
    static void save(std::string filename, std::vector<PIDTest>& pidTests);
    
};


PIDTest::PIDTest() {
    tau_p = 0;
    tau_d = 0;
    tau_i = 0;
    throttle = 0;
    num_steps = 0;
    mean_speed = 0;
    cte_tolerance = CTE_TOLERANCE;
    max_abs_cte = 0;
    result = 0;
}


PIDTest::PIDTest(
    std::default_random_engine& generator, 
    std::uniform_real_distribution<double>& dist_tau_p,
    std::uniform_real_distribution<double>& dist_tau_d,
    std::uniform_real_distribution<double>& dist_tau_i,
    double throttle_p,
    double cte_tolerance_p
) {
    tau_p = dist_tau_p(generator);
    tau_d = dist_tau_d(generator);
    tau_i = dist_tau_i(generator);
    throttle = throttle_p;
    num_steps = 0;
    mean_speed = 0;
    cte_tolerance = cte_tolerance_p;
    max_abs_cte = 0;
    result = 0;
}


std::ostream& operator<<(std::ostream& os, const PIDTest& pidTest) {
    os << pidTest.tau_p << " ";
    os << pidTest.tau_d << " ";
    os << pidTest.tau_i << " ";
    os << pidTest.throttle << " ";
    os << pidTest.num_steps << " ";
    os << pidTest.mean_speed << " ";
    os << pidTest.cte_tolerance << " ";
    os << pidTest.max_abs_cte << " ";
    os << pidTest.result << std::endl;
    return os;
}



std::vector<PIDTest> PIDTest::load(std::istream& is) {
    std::vector<PIDTest> result;

    char c;
    std::string s;

    is >> s;

    while (is) {
        PIDTest pidTest;
        is >> pidTest.tau_p;
        is >> c;
        is >> pidTest.tau_d;
        is >> c;
        is >> pidTest.tau_i;
        is >> c;
        is >> pidTest.throttle;
        is >> c;
        is >> pidTest.num_steps;
        is >> c;
        is >> pidTest.mean_speed;
        is >> c;
        is >> pidTest.cte_tolerance;
        is >> c;
        is >> pidTest.max_abs_cte;
        is >> c;
        is >> pidTest.result;
        result.push_back(pidTest);
    }
    result.pop_back();

    return result;
}

void PIDTest::save(std::string filename, std::vector<PIDTest>& pidTests) {
    std::ofstream fos(filename.c_str());
    fos << "tau_p;tau_d;tau_i;throttle;num_steps;mean_speed;cte_tolerante;max_abs_cte;result" << std::endl;
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
                      pidTest.result  <<
                      std::endl; }
                  );
    fos.close();
}


void saveAndReset(std::vector<PIDTest>& pidTests, PIDTest& pidTest, uWS::WebSocket<uWS::SERVER> ws) {
   // std::cout << "saveAndReset" << std::endl;
    pidTests.push_back(pidTest);

    
    if (pidTests.size() % 15 == 14 && SAVE) {
        PIDTest::save(PID_TESTS_CVS_FILE, pidTests);
    }
    std::string msg("42[\"reset\", {}]");
    ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
}

int main(int argc, char **argv)
{
    uWS::Hub h;

    std::vector<PIDTest> pidTests;

    std::ifstream ifs(PID_TESTS_CVS_FILE);
    if(ifs) {
        std::cout << std::endl << "loading file: " << PID_TESTS_CVS_FILE << std::endl;
        pidTests = PIDTest::load(ifs);
    }
    
    bool fast_mode = false;
    if (argc >= 2) {
        if (std::strcmp(argv[1], "fast") == 0) fast_mode = true;
    }

    std::default_random_engine generator;
    std::uniform_real_distribution<double> dist_tau_p(CENTER_TAU_P - DELTA_TAU_P, CENTER_TAU_P + DELTA_TAU_P);
    std::uniform_real_distribution<double> dist_tau_d(CENTER_TAU_D - DELTA_TAU_D, CENTER_TAU_D + DELTA_TAU_D);
    std::uniform_real_distribution<double> dist_tau_i(CENTER_TAU_I - DELTA_TAU_I, CENTER_TAU_I + DELTA_TAU_I);

    double throttle = THROTTLE;
    int step = 0;
    double cte_prev = 0.0;
    double cte_sum = 0.0;

    PIDTest pidTest = PIDTest(generator, dist_tau_p, dist_tau_d, dist_tau_i, throttle, CTE_TOLERANCE);
    
    h.onMessage([
        &pidTests, 
        &pidTest, 
        &throttle, 
        &cte_prev, 
        &cte_sum,
        &generator,
        &dist_tau_p,
        &dist_tau_d,
        &dist_tau_i,
        &step,
        &fast_mode
    ](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {

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
                    
                    if (fabs(cte+cte_prev) > 2*pidTest.cte_tolerance) {
                        pidTest.result = 0;
                        pidTest.num_steps = step;
                        pidTest.mean_speed = pidTest.mean_speed / step;
                        saveAndReset(pidTests, pidTest, ws);
                        step = 0;
                        cte_prev = 0;
                        cte_sum = 0;
                        pidTest = PIDTest(generator, dist_tau_p, dist_tau_d, dist_tau_i, throttle, CTE_TOLERANCE);
                    }

                    // implement PID
                    steer_value = -pidTest.tau_p * cte - (step > 0 ? pidTest.tau_d : 0) * (cte - cte_prev) - pidTest.tau_i * cte_sum;
                    
                    if (fast_mode) {
                        if (speed < 70 - 6*fabs(cte)) throttle = 1.00;
                        else throttle = 0.25;
                    }
                    else {
                        throttle = 0.25;
                    }
                    
                    json msgJson;
                    msgJson["steering_angle"] = steer_value;
                    msgJson["throttle"] = throttle; 
                    auto msg = "42[\"steer\"," + msgJson.dump() + "]";
                    ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
                    
                    cte_prev = cte;
                    cte_sum += cte;
                    if (fabs(cte) > pidTest.max_abs_cte) pidTest.max_abs_cte = fabs(cte);
                    step++;
                    pidTest.mean_speed += speed;

                    if (step > MAX_NUM_STEPS) {
                        pidTest.result = 1;
                        pidTest.num_steps = step;
                        pidTest.mean_speed = pidTest.mean_speed / step;
                        saveAndReset(pidTests, pidTest, ws);
                        step = 0;
                        cte_prev = 0;
                        cte_sum = 0;
                        pidTest = PIDTest(generator, dist_tau_p, dist_tau_d, dist_tau_i, throttle, CTE_TOLERANCE);
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


