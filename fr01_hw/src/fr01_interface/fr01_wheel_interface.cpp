#include <ros/ros.h>
#include <fr01_interface/fr01_wheel_interface.hpp>

#include <angles/angles.h>
#include <joint_limits_interface/joint_limits_urdf.h>
#include <joint_limits_interface/joint_limits_rosparam.h>


Fr01WheelInterface::Fr01WheelInterface(std::vector<std::string> joint_names)
  : joint_names_(joint_names)
{
  ROS_INFO_STREAM_NAMED("Fr01WheelInterface", "Fr01WheelInterface constructor is called.");
  ros::NodeHandle n("~");
  n_dof_ = joint_names_.size();
  this->cleanup();
  this->resize();
  commands_.name = joint_names_;
  wheel_vel_pub_ = nh_.advertise<sensor_msgs::JointState>(n.param<std::string>("wheel_cmd_topic_name","/wheel_vel_cmd"), 1);
}

void Fr01WheelInterface::register_interface(hardware_interface::JointStateInterface &joint_state_interface,
					    hardware_interface::VelocityJointInterface &vel_joint_interface)
{
  // Hardware interfaces
  for (size_t i = 0; i < n_dof_; ++i) {
    hardware_interface::JointStateHandle state_handle(joint_names_[i],
						      &joint_pos_[i],
						      &joint_vel_[i],
						      &joint_eff_[i]);
    joint_state_interface.registerHandle(state_handle);
    hardware_interface::JointHandle vel_handle(joint_state_interface.getHandle(joint_names_[i]),
					       &joint_vel_cmd_[i]);
    vel_joint_interface.registerHandle(vel_handle);

    ROS_INFO_STREAM("Registered joint '" << joint_names_[i] << " ' in the VelocityJointInterface");
  }
}

void Fr01WheelInterface::cleanup()
{
  // Cleanup
  joint_pos_.clear();
  joint_vel_.clear();
  joint_eff_.clear();
  joint_vel_cmd_.clear();
  commands_.velocity.clear();
  commands_.position.clear();
  commands_.effort.clear();
}

void Fr01WheelInterface::resize()
{
  joint_pos_.resize(n_dof_);
  joint_vel_.resize(n_dof_);
  joint_eff_.resize(n_dof_);
  joint_vel_cmd_.resize(n_dof_);
  commands_.velocity.resize(n_dof_);
  commands_.position.resize(n_dof_);
  commands_.effort.resize(n_dof_);
}

void Fr01WheelInterface::write()
{
  commands_.header.stamp = ros::Time::now();
  commands_.velocity = joint_vel_cmd_;
  wheel_vel_pub_.publish(commands_);
}

void Fr01WheelInterface::read(sensor_msgs::JointStateConstPtr &state)
{
  for (size_t i = 0; i < n_dof_; ++i) {
    joint_pos_[i] = state->position[i];
    joint_vel_[i] = state->velocity[i];
    //joint_eff_[i] = state->effort[i];
  }
}
