#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>
#include <limits>

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

int main()
{
  uWS::Hub h;


  PID pid_steer, pid_throttle;
  // TODO: Initialize the pid variable.
//  pid_steer.Init(0.2, 0.004, 3.0); //Weights sebastian gave in the lesson
  pid_steer.Init(0.2, 0.005, .5);
  h.onMessage([&pid_steer](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
	double cte, speed, angle, steer_value, throttle, dcte;
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          cte = std::stod(j[1]["cte"].get<std::string>());
          speed = std::stod(j[1]["speed"].get<std::string>());
          angle = std::stod(j[1]["steering_angle"].get<std::string>());

          dcte = cte - pid_steer.prev_cte;



          //desired_speed = 5.0 + 98/(1 + pow(fabs(dcte*50),4)/10000);
          //throttle = (desired_speed-speed)/10;
          if(speed<10.0 && speed>=5.0)
        	  throttle = 0.2;
          else if(speed<5.0)
        	  throttle = 0.5;
          else
        	  throttle = 0.0;

//          double throttle_grad = -0.08*fabs(cte)+0.04;
//          throttle = throttle_grad*speed + 0.2;

          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */


		  steer_value = (-1 * ((pid_steer.Kp * cte) + (pid_steer.Kd * dcte) + (pid_steer.Ki * pid_steer.total_cte)));
		  pid_steer.UpdateError(cte, steer_value, angle);

          //desired_angle = -8*cte;
          //steer_value = (desired_angle-angle)/50;
          //steer_value = -1*


          // DEBUG
          //std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          std::cout<<"angle:"<<angle <<"  Speed:"<< speed <<"  cte:"<<cte; //Sensor reading
          std::cout<<" steer:"<< steer_value<<" throttle:"<<throttle;
          std::cout<<" dcte:"<<dcte <<" max_cte:"<<pid_steer.max_cte<<std::endl;
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
