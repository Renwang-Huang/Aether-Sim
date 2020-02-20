/***************************************************************************************************************************
*
* Author: bingo
* Email: bingobin.lw@gmail.com
* Time: 2020.02.19
* Description: 实现小车在offboard模式下的走预定航点
***************************************************************************************************************************/
#include "mission_car_offboard.h"
using namespace std;
using namespace Eigen;
MissionCar::MissionCar(const ros::NodeHandle& nh, const ros::NodeHandle& nh_private):
  nh_(nh),
  nh_private_(nh_private) {
  Initialize();
  cmdloop_timer_ = nh_.createTimer(ros::Duration(0.1), &MissionCar::CmdLoopCallback, this); // Define timer for constant loop rate

}

MissionCar::~MissionCar() {
  //Destructor
}
void MissionCar::CmdLoopCallback(const ros::TimerEvent& event)
{
	MissionStateUpdate();
}

void MissionCar::MissionStateUpdate(){

  Eigen::Vector4d  px4_vel_;
  px4_vel_[0] = 1800;
  px4_vel_[1] = 1800;
  px4_vel_[2] = 1800;
  px4_vel_[3] = 1800;

  OffboardControl_.send_actuator_setpoint(px4_vel_);

}

void MissionCar::Initialize()
{
}
int main(int argc, char** argv) {
  ros::init(argc,argv,"mission_car_offboard");
  ros::NodeHandle nh("");
  ros::NodeHandle nh_private("~");

  MissionCar MissionCar(nh, nh_private);

  ros::spin();
  return 0;
}
