#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

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

bool fresh_start = true;

int main()
{
  uWS::Hub h;
  //#define KP_INIT 0.05
  //#define KI_INIT 0.001
  //#define KD_INIT 0.8
  #define KP_INIT 0.08
  #define KI_INIT 0.0009
  #define KD_INIT 3.8 
  #define P_FLAG true
  #define I_FLAG true 
  #define D_FLAG true 
  #define TWIDDLE_EN true 
  PID pid;
  // TODO: Initialize the pid variable.
  pid.Init(KP_INIT, KI_INIT, KD_INIT, P_FLAG, I_FLAG, D_FLAG, TWIDDLE_EN);
  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
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
          double throttle_value;
          double target_speed;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          
          // The simulator provides the CTE and velocity (mph) to compute the steering angle
          pid.UpdateError(cte);
          steer_value = pid.TotalError();
          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: ";
          std::cout << "Speed: " << speed;
          std::cout << " Best CTE: " << pid.best_cte << std::endl;
          std::cout << "Kp: " << pid.Kp << " Ki: " << pid.Ki << " Kd: " << pid.Kd << std::endl;

          //if (fabs(cte)>0.03){

          //  target_speed = speed*0.75-cte*10;
          //  throttle_value = target_speed/100;
          //  if (speed < 10.0 && throttle_value < 0.0){

          //    throttle_value *= -1.0;
          //  }
          //}
          //else{ 
          //  throttle_value = 0.3;
          //}

          throttle_value = 0.3;

          //if (fabs(cte) > 1.5){

          //  if(steer_value <0.0){
          //      steer_value+=0.5;
          //  }
          //  else if(steer_value > 0.0){
          //      steer_value-=0.5;
          //  }
          //}

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle_value;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
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
    if (fresh_start){
        std::string msg = "42[\"reset\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        fresh_start = false;
    }    
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
