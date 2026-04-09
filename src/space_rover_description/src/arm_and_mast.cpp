#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "std_srvs/srv/set_bool.hpp"

class ArmAndMastController : public rclcpp::Node
{
public:
  ArmAndMastController()
  : Node("curiosity_rover_arm_and_mast"),
    controller_ns_(declare_parameter("controller_ns", std::string("curiosity_mars_rover")))
  {
    mast_p_pub_ = makeFloatPublisher("mast_p_joint_position_controller");
    mast_02_pub_ = makeFloatPublisher("mast_02_joint_position_controller");
    mast_cameras_pub_ = makeFloatPublisher("mast_cameras_joint_position_controller");
    arm_01_pub_ = makeFloatPublisher("arm_01_joint_position_controller");
    arm_02_pub_ = makeFloatPublisher("arm_02_joint_position_controller");
    arm_03_pub_ = makeFloatPublisher("arm_03_joint_position_controller");
    arm_04_pub_ = makeFloatPublisher("arm_04_joint_position_controller");
    arm_tools_pub_ = makeFloatPublisher("arm_tools_joint_position_controller");

    setArmPose(false);
    setMastPose(false);

    arm_service_ = create_service<std_srvs::srv::SetBool>(
      "/" + controller_ns_ + "/arm_mode",
      std::bind(&ArmAndMastController::handleArmMode, this, std::placeholders::_1, std::placeholders::_2));
    mast_service_ = create_service<std_srvs::srv::SetBool>(
      "/" + controller_ns_ + "/mast_mode",
      std::bind(&ArmAndMastController::handleMastMode, this, std::placeholders::_1, std::placeholders::_2));
  }

private:
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr makeFloatPublisher(const std::string & controller_name)
  {
    const std::string topic = "/" + controller_ns_ + "/" + controller_name + "/command";
    return create_publisher<std_msgs::msg::Float64>(topic, 10);
  }

  void publishFloat(const rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr & publisher, double value)
  {
    std_msgs::msg::Float64 msg;
    msg.data = value;
    publisher->publish(msg);
  }

  bool setArmPose(bool open)
  {
    if (open) {
      publishFloat(arm_01_pub_, 0.0);
      publishFloat(arm_02_pub_, 0.0);
      publishFloat(arm_03_pub_, 0.0);
      publishFloat(arm_04_pub_, 0.0);
      publishFloat(arm_tools_pub_, 0.0);
    } else {
      publishFloat(arm_01_pub_, -1.57);
      publishFloat(arm_02_pub_, -0.4);
      publishFloat(arm_03_pub_, -1.1);
      publishFloat(arm_04_pub_, -1.57);
      publishFloat(arm_tools_pub_, -1.57);
    }
    return true;
  }

  bool setMastPose(bool open)
  {
    if (open) {
      publishFloat(mast_p_pub_, 0.0);
      publishFloat(mast_02_pub_, -0.5);
      publishFloat(mast_cameras_pub_, 0.0);
    } else {
      publishFloat(mast_p_pub_, 1.57);
      publishFloat(mast_02_pub_, -1.57);
      publishFloat(mast_cameras_pub_, 0.0);
    }
    return true;
  }

  void handleArmMode(
    const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
    std::shared_ptr<std_srvs::srv::SetBool::Response> response)
  {
    response->success = setArmPose(request->data);
    response->message = request->data ? "Executed arm mode=open" : "Executed arm mode=close";
  }

  void handleMastMode(
    const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
    std::shared_ptr<std_srvs::srv::SetBool::Response> response)
  {
    response->success = setMastPose(request->data);
    response->message = request->data ? "Executed mast mode=open" : "Executed mast mode=close";
  }

  std::string controller_ns_;

  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr mast_p_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr mast_02_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr mast_cameras_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr arm_01_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr arm_02_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr arm_03_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr arm_04_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr arm_tools_pub_;

  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr arm_service_;
  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr mast_service_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ArmAndMastController>());
  rclcpp::shutdown();
  return 0;
}
