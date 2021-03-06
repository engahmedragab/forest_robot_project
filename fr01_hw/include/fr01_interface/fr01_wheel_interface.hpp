#ifndef FR01_WHEEL_INTERFACE_H
#define FR01_WHEEL_INTERFACE_H

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Twist.h>

#include <controller_manager/controller_manager.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>
#include <realtime_tools/realtime_buffer.h>

class Fr01WheelInterface
  : public hardware_interface::RobotHW
{
 public:
  Fr01WheelInterface(std::vector<std::string> joint_names);
  void cleanup();
  void resize();
  void register_interface(hardware_interface::JointStateInterface &joint_state_interface,
			  hardware_interface::VelocityJointInterface &vel_joint_interface);
  void write();
  void read(sensor_msgs::JointStateConstPtr &state);
 protected:
  unsigned int n_dof_;

  std::vector<std::string> joint_names_;
  std::vector<double> joint_pos_;
  std::vector<double> joint_vel_;
  std::vector<double> joint_eff_;
  std::vector<double> joint_vel_cmd_;
  ros::NodeHandle nh_;

  sensor_msgs::JointState commands_;
  ros::Publisher wheel_vel_pub_;
};

#endif /* FR01_WHEEL_INTERFACE_H */
