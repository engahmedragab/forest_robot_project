#ifndef FR01INTERFACE_H
#define FR01INTERFACE_H

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Twist.h>

#include <controller_manager/controller_manager.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>
#include <realtime_tools/realtime_buffer.h>

#ifndef MIN
#define MIN(a,b) ((a < b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a > b) ? (a) : (b))
#endif
#ifndef NORMALIZE
#define NORMALIZE(z) atan2(sin(z), cos(z))
#endif

class Fr01Interface
  : public hardware_interface::RobotHW
{
 public:
  Fr01Interface();
  ~Fr01Interface();

  void calculateOdometry(const sensor_msgs::JointStateConstPtr& wheel_state,
			 const sensor_msgs::JointStateConstPtr& steer_state);
  void setParams(std::vector<double> wheel_diameters,
		 std::vector<double> tred_width);
  
  void drive(double linear_speed, double angular_speed, 
	     sensor_msgs::JointState& wheel_input,
	     sensor_msgs::JointState& steer_input,
	     bool isPivotTurn);
  
  void read();

  void write();
  
 protected:
  unsigned int wheel_n_dof_;
  unsigned int steer_n_dof_;
  
  // 0 : front, 1 : middle, 2 : rear
  std::vector<double> tred_width_;
  
  // 0 : right front, 1 : left front, 2 : right rear, 3 : left rear
  std::vector<double> tred_length_;
  
  // 0 : right front, 1 : left front, 2 : right middle, 3 : left middle
  // 4 : right rear, 5 : left rear
  std::vector<double> wheel_diameters_;

  bool two_steer_mode_;
  double angular_limit_max_;
  double angular_limit_min_;
  double linear_limit_max_;
  double linear_limit_min_;

  std::vector<std::string> transmission_names_;

  std::vector<std::string> wheel_joint_names_;
  std::vector<double> wheel_joint_pos_;
  std::vector<double> wheel_joint_vel_;
  std::vector<double> wheel_joint_eff_;
  std::vector<double> wheel_joint_vel_cmd_;
  std::vector<std::string> steer_joint_names_;
  std::vector<double> steer_joint_pos_;
  std::vector<double> steer_joint_vel_;
  std::vector<double> steer_joint_eff_;
  std::vector<double> steer_joint_pos_cmd_;
  
  hardware_interface::JointStateInterface wheel_joint_state_interface_;
  hardware_interface::JointStateInterface steer_joint_state_interface_;
  
  hardware_interface::VelocityJointInterface wheel_joint_vel_interface_;
  hardware_interface::VelocityJointInterface steer_joint_pos_interface_;
  
};


#endif /* FR01INTERFACE_H */
