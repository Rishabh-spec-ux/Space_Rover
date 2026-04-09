#include <chrono>
#include <cmath>
#include <functional>
#include <memory>
#include <string>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"

class Ackerman6Wheels : public rclcpp::Node
{
public:
  Ackerman6Wheels()
  : Node("curiosity_rover_ackerman"),
    wheel_command_topic_(declare_parameter(
        "wheel_command_topic", std::string("/wheel_velocity_controller/commands"))),
    steering_command_topic_(declare_parameter(
        "steering_command_topic", std::string("/steering_position_controller/commands"))),
    suspension_command_topic_(declare_parameter(
        "suspension_command_topic", std::string("/suspension_position_controller/commands"))),
    wheel_radius_(declare_parameter("wheel_radius", 0.242647)),
    track_width_(declare_parameter("track_width", 1.53774)),
    front_center_distance_(declare_parameter("front_center_distance", 0.5)),
    back_center_distance_(declare_parameter("back_center_distance", 0.5)),
    suspension_front_angle_(declare_parameter("suspension_front_angle", 0.2)),
    suspension_back_angle_(declare_parameter("suspension_back_angle", -0.2))
  {
    cmd_vel_sub_ = create_subscription<geometry_msgs::msg::Twist>(
      "/cmd_vel", 10,
      std::bind(&Ackerman6Wheels::cmdVelCb, this, std::placeholders::_1));

    wheel_pub_ = create_publisher<std_msgs::msg::Float64MultiArray>(wheel_command_topic_, 10);
    steering_pub_ = create_publisher<std_msgs::msg::Float64MultiArray>(steering_command_topic_, 10);
    suspension_pub_ = create_publisher<std_msgs::msg::Float64MultiArray>(suspension_command_topic_, 10);

    setSuspensionMode();
    stopMotion();
    timer_ = create_wall_timer(
      std::chrono::milliseconds(50),
      std::bind(&Ackerman6Wheels::publishCommands, this));
  }

private:
  void cmdVelCb(const geometry_msgs::msg::Twist::SharedPtr msg)
  {
    last_cmd_ = *msg;
  }

  void setSuspensionMode()
  {
    std_msgs::msg::Float64MultiArray msg;
    msg.data = {
      suspension_back_angle_,
      suspension_back_angle_,
      suspension_back_angle_,
      suspension_back_angle_,
      suspension_front_angle_,
      suspension_front_angle_,
    };
    suspension_pub_->publish(msg);
  }

  void stopMotion()
  {
    last_cmd_ = geometry_msgs::msg::Twist();
    publishWheelVelocities(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    publishSteeringAngles(0.0, 0.0, 0.0, 0.0);
  }

  void publishCommands()
  {
    const double linear = last_cmd_.linear.x;
    const double angular = last_cmd_.angular.z;

    if (std::fabs(linear) < 1e-6 && std::fabs(angular) < 1e-6) {
      publishWheelVelocities(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
      publishSteeringAngles(0.0, 0.0, 0.0, 0.0);
      return;
    }

    if (std::fabs(angular) < 1e-6) {
      const double wheel_speed = linear / wheel_radius_;
      publishWheelVelocities(
        wheel_speed, -wheel_speed,
        wheel_speed, -wheel_speed,
        wheel_speed, -wheel_speed);
      publishSteeringAngles(0.0, 0.0, 0.0, 0.0);
      return;
    }

    const double front_left_angle = wheelHeading(front_center_distance_, track_width_ * 0.5, linear, angular);
    const double front_right_angle = wheelHeading(front_center_distance_, -track_width_ * 0.5, linear, angular);
    const double back_left_angle = wheelHeading(-back_center_distance_, track_width_ * 0.5, linear, angular);
    const double back_right_angle = wheelHeading(-back_center_distance_, -track_width_ * 0.5, linear, angular);

    const double front_left_speed = wheelAngularVelocity(front_center_distance_, track_width_ * 0.5, linear, angular);
    const double front_right_speed = wheelAngularVelocity(front_center_distance_, -track_width_ * 0.5, linear, angular);
    const double middle_left_speed = wheelAngularVelocity(0.0, track_width_ * 0.5, linear, angular);
    const double middle_right_speed = wheelAngularVelocity(0.0, -track_width_ * 0.5, linear, angular);
    const double back_left_speed = wheelAngularVelocity(-back_center_distance_, track_width_ * 0.5, linear, angular);
    const double back_right_speed = wheelAngularVelocity(-back_center_distance_, -track_width_ * 0.5, linear, angular);

    publishSteeringAngles(back_left_angle, back_right_angle, front_left_angle, front_right_angle);
    publishWheelVelocities(
      back_left_speed, -back_right_speed,
      front_left_speed, -front_right_speed,
      middle_left_speed, -middle_right_speed);
  }

  double wheelHeading(double x, double y, double linear, double angular) const
  {
    const double vx = linear - angular * y;
    const double vy = angular * x;
    return std::atan2(vy, vx);
  }

  double wheelAngularVelocity(double x, double y, double linear, double angular) const
  {
    const double vx = linear - angular * y;
    const double vy = angular * x;
    const double speed = std::hypot(vx, vy);
    const double direction = (vx >= 0.0) ? 1.0 : -1.0;
    return direction * speed / wheel_radius_;
  }

  void publishSteeringAngles(double back_left, double back_right, double front_left, double front_right)
  {
    std_msgs::msg::Float64MultiArray msg;
    msg.data = {back_left, back_right, front_left, front_right};
    steering_pub_->publish(msg);
  }

  void publishWheelVelocities(
    double back_left, double back_right,
    double front_left, double front_right,
    double middle_left, double middle_right)
  {
    std_msgs::msg::Float64MultiArray msg;
    msg.data = {
      back_left,
      back_right,
      front_left,
      front_right,
      middle_left,
      middle_right,
    };
    wheel_pub_->publish(msg);
  }

  std::string wheel_command_topic_;
  std::string steering_command_topic_;
  std::string suspension_command_topic_;
  double wheel_radius_;
  double track_width_;
  double front_center_distance_;
  double back_center_distance_;
  double suspension_front_angle_;
  double suspension_back_angle_;

  geometry_msgs::msg::Twist last_cmd_;

  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_sub_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr wheel_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr steering_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr suspension_pub_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Ackerman6Wheels>());
  rclcpp::shutdown();
  return 0;
}
