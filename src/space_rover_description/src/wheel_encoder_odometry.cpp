#include <cmath>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "geometry_msgs/msg/quaternion.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "tf2_ros/transform_broadcaster.h"

class WheelEncoderOdometry : public rclcpp::Node
{
public:
  WheelEncoderOdometry()
  : Node("wheel_encoder_odometry"),
    wheel_radius_(declare_parameter("wheel_radius", 0.242647)),
    track_width_(declare_parameter("track_width", 1.53774)),
    odom_frame_(declare_parameter("odom_frame", std::string("odom"))),
    base_frame_(declare_parameter("base_frame", std::string("base_footprint"))),
    publish_tf_(declare_parameter("publish_tf", true))
  {
    left_joints_ = {"front_wheel_L_joint", "middle_wheel_L_joint", "back_wheel_L_joint"};
    right_joints_ = {"front_wheel_R_joint", "middle_wheel_R_joint", "back_wheel_R_joint"};

    odom_pub_ = create_publisher<nav_msgs::msg::Odometry>("/wheel_odom", 10);
    if (publish_tf_) {
      tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
    }

    joint_state_sub_ = create_subscription<sensor_msgs::msg::JointState>(
      "/joint_states", 50,
      std::bind(&WheelEncoderOdometry::jointStateCb, this, std::placeholders::_1));
  }

private:
  static geometry_msgs::msg::Quaternion yawToQuaternion(double yaw)
  {
    geometry_msgs::msg::Quaternion q;
    q.z = std::sin(yaw * 0.5);
    q.w = std::cos(yaw * 0.5);
    return q;
  }

  static int indexOf(
    const std::vector<std::string> & names,
    const std::string & target)
  {
    for (size_t i = 0; i < names.size(); ++i) {
      if (names[i] == target) {
        return static_cast<int>(i);
      }
    }
    return -1;
  }

  static bool averageSide(
    const sensor_msgs::msg::JointState & msg,
    const std::vector<std::string> & joints,
    double & average)
  {
    double sum = 0.0;
    int count = 0;
    for (const auto & joint : joints) {
      const int idx = indexOf(msg.name, joint);
      if (idx >= 0 && static_cast<size_t>(idx) < msg.position.size()) {
        sum += msg.position[static_cast<size_t>(idx)];
        ++count;
      }
    }

    if (count == 0) {
      return false;
    }

    average = sum / static_cast<double>(count);
    return true;
  }

  void jointStateCb(const sensor_msgs::msg::JointState::SharedPtr msg)
  {
    if (msg->position.empty()) {
      return;
    }

    double left_pos = 0.0;
    double right_pos = 0.0;
    if (!averageSide(*msg, left_joints_, left_pos) || !averageSide(*msg, right_joints_, right_pos)) {
      return;
    }

    // The rover's right wheel joints spin with the opposite sign convention.
    right_pos = -right_pos;

    const rclcpp::Time stamp(msg->header.stamp);
    if (!last_stamp_.has_value()) {
      last_stamp_ = stamp;
      last_left_pos_ = left_pos;
      last_right_pos_ = right_pos;
      return;
    }

    const double dt = (stamp - *last_stamp_).seconds();
    if (dt <= 0.0) {
      return;
    }

    const double left_delta = (left_pos - last_left_pos_) * wheel_radius_;
    const double right_delta = (right_pos - last_right_pos_) * wheel_radius_;

    const double linear_delta = 0.5 * (left_delta + right_delta);
    const double angular_delta = (right_delta - left_delta) / track_width_;

    const double heading_mid = yaw_ + 0.5 * angular_delta;
    x_ += linear_delta * std::cos(heading_mid);
    y_ += linear_delta * std::sin(heading_mid);
    yaw_ += angular_delta;

    const double linear_velocity = linear_delta / dt;
    const double angular_velocity = angular_delta / dt;

    nav_msgs::msg::Odometry odom;
    odom.header.stamp = msg->header.stamp;
    odom.header.frame_id = odom_frame_;
    odom.child_frame_id = base_frame_;
    odom.pose.pose.position.x = x_;
    odom.pose.pose.position.y = y_;
    odom.pose.pose.orientation = yawToQuaternion(yaw_);
    odom.twist.twist.linear.x = linear_velocity;
    odom.twist.twist.angular.z = angular_velocity;
    odom_pub_->publish(odom);

    if (tf_broadcaster_) {
      geometry_msgs::msg::TransformStamped tf_msg;
      tf_msg.header = odom.header;
      tf_msg.child_frame_id = base_frame_;
      tf_msg.transform.translation.x = x_;
      tf_msg.transform.translation.y = y_;
      tf_msg.transform.rotation = odom.pose.pose.orientation;
      tf_broadcaster_->sendTransform(tf_msg);
    }

    last_stamp_ = stamp;
    last_left_pos_ = left_pos;
    last_right_pos_ = right_pos;
  }

  double wheel_radius_;
  double track_width_;
  std::string odom_frame_;
  std::string base_frame_;
  bool publish_tf_;

  std::vector<std::string> left_joints_;
  std::vector<std::string> right_joints_;

  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

  std::optional<rclcpp::Time> last_stamp_;
  double last_left_pos_{0.0};
  double last_right_pos_{0.0};
  double x_{0.0};
  double y_{0.0};
  double yaw_{0.0};
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<WheelEncoderOdometry>());
  rclcpp::shutdown();
  return 0;
}
