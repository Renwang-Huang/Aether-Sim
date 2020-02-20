#include <ros/ros.h>
#include <iostream>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <geometry_msgs/Twist.h>
#include <mavros_msgs/PositionTarget.h>
#include <Eigen/Dense>
#include "offboard_control.h"
using namespace std;
using namespace Eigen;
class MissionCar {
 public:
    /**
     *默认构造函数
     */
    MissionCar(const ros::NodeHandle& nh, const ros::NodeHandle& nh_private);
    /**
     * 析构函数
     */
    ~MissionCar();
    void Initialize();


  OffboardControl OffboardControl_;
 private:
  ros::NodeHandle nh_;
  ros::NodeHandle nh_private_;
  ros::Timer cmdloop_timer_;
  void CmdLoopCallback(const ros::TimerEvent& event);
  void MissionStateUpdate(void);
};
