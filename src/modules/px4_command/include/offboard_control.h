#include <ros/ros.h>
#include <iostream>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <geometry_msgs/Twist.h>
#include <mavros_msgs/PositionTarget.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/ActuatorControl.h>
#include <mavros_msgs/AttitudeTarget.h>
#include <mavros_msgs/GlobalPositionTarget.h>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;
class OffboardControl {
 public:
    /**
     *默认构造函数
     */
  OffboardControl(void):
    offboard_nh_("~") {
  mavros_setpoint_pos_pub_ = offboard_nh_.advertise<mavros_msgs::PositionTarget>("/mavros/setpoint_raw/local", 10);

  mavros_setpoint_local_pos_pub_ = offboard_nh_.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);

  mavros_setpoint_global_pos_pub_ = offboard_nh_.advertise<mavros_msgs::GlobalPositionTarget>("/mavros/setpoint_position/global", 10);

  actuator_setpoint_pub_ = offboard_nh_.advertise<mavros_msgs::ActuatorControl>("/mavros/actuator_control", 10);
  setpoint_raw_attitude_pub_ = offboard_nh_.advertise<mavros_msgs::AttitudeTarget>("/mavros/setpoint_raw/attitude", 10);

}
    void send_velxy_posz_setpoint(const Eigen::Vector3d& vel_sp, float desire_z);
    void send_pos_setpoint(const Eigen::Vector3d& pos_sp, float yaw_sp);
    void send_velxyz_setpoint(const Eigen::Vector3d& vel_sp, float yaw_sp);
    void send_local_pos_setpoint(const Eigen::Vector3d& pos_sp);
    void send_global_velxy_setpoint(const Eigen::Vector3d& vel_sp);
    void send_actuator_setpoint(const Eigen::Vector4d& actuator_sp);
    void send_attitude_setpoint(const Eigen::Vector4d& _AttitudeReference,float thrust_sp);
    void send_attitude_rate_setpoint(const Eigen::Vector3d& attitude_rate_sp, float thrust_sp);
  private:
    ros::NodeHandle offboard_nh_;
    ros::Publisher mavros_setpoint_pos_pub_;
    ros::Publisher mavros_setpoint_local_pos_pub_;
    ros::Publisher mavros_setpoint_global_pos_pub_;
    ros::Publisher actuator_setpoint_pub_;
    ros::Publisher setpoint_raw_attitude_pub_;

};

//发送xyz速度期望值以及期望偏航角至飞控
void OffboardControl::send_velxyz_setpoint(const Eigen::Vector3d& vel_sp, float yaw_sp)
{
    mavros_msgs::PositionTarget pos_setpoint;
    //Bitmask toindicate which dimensions should be ignored (1 means ignore,0 means not ignore; Bit 10 must set to 0)
    //Bit 1:x, bit 2:y, bit 3:z, bit 4:vx, bit 5:vy, bit 6:vz, bit 7:ax, bit 8:ay, bit 9:az, bit 10:is_force_sp, bit 11:yaw, bit 12:yaw_rate
    //Bit 10 should set to 0, means is not force sp
    pos_setpoint.type_mask = 1 + 2 + 4 +/* 8 + 16 + 32 +*/ 64 + 128 + 256 + 512 + /*1024 +*/ 2048;
    pos_setpoint.coordinate_frame = 1;

    pos_setpoint.velocity.x = vel_sp[0];
    pos_setpoint.velocity.y = vel_sp[1];
    pos_setpoint.velocity.z = vel_sp[2];
    pos_setpoint.yaw = yaw_sp;
    mavros_setpoint_pos_pub_.publish(pos_setpoint);
}
//发送xy速度期望值以及高度z期望值至飞控（输入：期望xy,期望高度）
void OffboardControl::send_velxy_posz_setpoint(const Eigen::Vector3d& vel_sp, float desire_z)
{
    mavros_msgs::PositionTarget pos_setpoint;
    //Bitmask toindicate which dimensions should be ignored (1 means ignore,0 means not ignore; Bit 10 must set to 0)
    //Bit 1:x, bit 2:y, bit 3:z, bit 4:vx, bit 5:vy, bit 6:vz, bit 7:ax, bit 8:ay, bit 9:az, bit 10:is_force_sp, bit 11:yaw, bit 12:yaw_rate
    //Bit 10 should set to 0, means is not force sp
    pos_setpoint.type_mask = 1 + 2 + /*4 + 8 + 16 +*/ 32 + 64 + 128 + 256 + 512 + 1024 + 2048;
    pos_setpoint.coordinate_frame = 1;

    pos_setpoint.velocity.x = vel_sp[0];
    pos_setpoint.velocity.y = vel_sp[1];
    pos_setpoint.position.z = desire_z;

    mavros_setpoint_pos_pub_.publish(pos_setpoint);
}
//发送位置期望值至飞控（输入：期望xyz,期望yaw）
void OffboardControl::send_pos_setpoint(const Eigen::Vector3d& pos_sp, float yaw_sp)
{
    mavros_msgs::PositionTarget pos_setpoint;
    //Bitmask toindicate which dimensions should be ignored (1 means ignore,0 means not ignore; Bit 10 must set to 0)
    //Bit 1:x, bit 2:y, bit 3:z, bit 4:vx, bit 5:vy, bit 6:vz, bit 7:ax, bit 8:ay, bit 9:az, bit 10:is_force_sp, bit 11:yaw, bit 12:yaw_rate
    //Bit 10 should set to 0, means is not force sp
    pos_setpoint.type_mask = /*1 + 2 + 4 */+ 8 + 16 + 32 + 64 + 128 + 256 + 512 /*+ 1024*/ + 2048;
    pos_setpoint.coordinate_frame = 1;

    pos_setpoint.position.x = pos_sp[0];
    pos_setpoint.position.y = pos_sp[1];
    pos_setpoint.position.z = pos_sp[2];
    pos_setpoint.yaw = yaw_sp;
    mavros_setpoint_pos_pub_.publish(pos_setpoint);
}

//通过/mavros/setpoint_position/local这个topic发布位置控制至飞控
void OffboardControl::send_local_pos_setpoint(const Eigen::Vector3d& pos_sp)
{
    geometry_msgs::PoseStamped pos_target;
    pos_target.pose.position.x = pos_sp[0];
    pos_target.pose.position.y = pos_sp[1];
    pos_target.pose.position.z = pos_sp[2];
    mavros_setpoint_local_pos_pub_.publish(pos_target);
}

//通过/mavros/setpoint_position/global这个topic发布速度至飞控
void OffboardControl::send_global_velxy_setpoint(const Eigen::Vector3d& vel_sp)
{
    mavros_msgs::GlobalPositionTarget global_setpoint;

/*	uint16 IGNORE_LATITUDE=1
	uint16 IGNORE_LONGITUDE=2
	uint16 IGNORE_ALTITUDE=4
	uint16 IGNORE_VX=8
	uint16 IGNORE_VY=16
	uint16 IGNORE_VZ=32
	uint16 IGNORE_AFX=64
	uint16 IGNORE_AFY=128
	uint16 IGNORE_AFZ=256
	uint16 FORCE=512
	uint16 IGNORE_YAW=1024
	uint16 IGNORE_YAW_RATE=2048
*/
    global_setpoint.type_mask = 1 + 2 + 4 + /*8 + 16 + 32*/ + 64 + 128 + 256 + 512 + 1024 + 2048;
    global_setpoint.coordinate_frame = 1;

    global_setpoint.velocity.x = vel_sp[0];
    global_setpoint.velocity.y = vel_sp[1];
    global_setpoint.velocity.z = vel_sp[2];
    mavros_setpoint_global_pos_pub_.publish(global_setpoint);
}

//发送底层至飞控（输入：MxMyMz,期望推力）
void OffboardControl::send_actuator_setpoint(const Eigen::Vector4d& actuator_sp)
{
    mavros_msgs::ActuatorControl actuator_setpoint;

    actuator_setpoint.group_mix = 0;
    actuator_setpoint.controls[0] = actuator_sp(0);
    actuator_setpoint.controls[1] = actuator_sp(1);
    actuator_setpoint.controls[2] = actuator_sp(2);
    actuator_setpoint.controls[3] = actuator_sp(3);
    actuator_setpoint.controls[4] = 0.0;
    actuator_setpoint.controls[5] = 0.0;
    actuator_setpoint.controls[6] = 0.0;
    actuator_setpoint.controls[7] = 0.0;

    actuator_setpoint_pub_.publish(actuator_setpoint);
}

//发送角度期望值至飞控（输入：期望角度-四元数,期望推力）
void OffboardControl::send_attitude_setpoint(const Eigen::Vector4d& _AttitudeReference,float thrust_sp)
{
    mavros_msgs::AttitudeTarget att_setpoint;

    //Mappings: If any of these bits are set, the corresponding input should be ignored:
    //bit 1: body roll rate, bit 2: body pitch rate, bit 3: body yaw rate. bit 4-bit 6: reserved, bit 7: throttle, bit 8: attitude

    att_setpoint.type_mask = 0b00000111;

    att_setpoint.orientation.x = _AttitudeReference[0];
    att_setpoint.orientation.y = _AttitudeReference[1];
    att_setpoint.orientation.z = _AttitudeReference[2];
    att_setpoint.orientation.w = _AttitudeReference[3];

    att_setpoint.thrust = thrust_sp;

    setpoint_raw_attitude_pub_.publish(att_setpoint);

}

//发送角速度期望值至飞控（输入：期望角速度,期望推力）
void OffboardControl::send_attitude_rate_setpoint(const Eigen::Vector3d& attitude_rate_sp, float thrust_sp)
{
    mavros_msgs::AttitudeTarget att_setpoint;

    //Mappings: If any of these bits are set, the corresponding input should be ignored:
    //bit 1: body roll rate, bit 2: body pitch rate, bit 3: body yaw rate. bit 4-bit 6: reserved, bit 7: throttle, bit 8: attitude

    att_setpoint.type_mask = 0b10000000;

    att_setpoint.body_rate.x = attitude_rate_sp[0];
    att_setpoint.body_rate.y = attitude_rate_sp[1];
    att_setpoint.body_rate.z = attitude_rate_sp[2];

    att_setpoint.thrust = thrust_sp;

    setpoint_raw_attitude_pub_.publish(att_setpoint);

}


