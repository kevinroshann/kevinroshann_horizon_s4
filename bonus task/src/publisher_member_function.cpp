// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"


using std::placeholders::_1;

class AskerNode : public rclcpp::Node {
public:
    AskerNode() : Node("asker_node") {
        publisher_ = this->create_publisher<std_msgs::msg::String>("question", 10);
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "answer", 10, std::bind(&AskerNode::answer_callback, this, _1));
        timer_ = this->create_wall_timer(
            std::chrono::seconds(2), std::bind(&AskerNode::publish_question, this));
    }

private:
    void publish_question() {
        if (stop_asking) return;  // Stop if "Yes" is received
        auto message = std_msgs::msg::String();
        message.data = "Are you Kevin?";
        RCLCPP_INFO(this->get_logger(), "Publishing: %s", message.data.c_str());
        publisher_->publish(message);
    }

    void answer_callback(const std_msgs::msg::String::SharedPtr msg) {
        if (msg->data == "Yes") {
            RCLCPP_INFO(this->get_logger(), "Hi Kevin! Exiting...");
            stop_asking = true;
            rclcpp::shutdown();
        } else {
            RCLCPP_INFO(this->get_logger(), "Received: %s, asking again...", msg->data.c_str());
        }
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
    bool stop_asking = false;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<AskerNode>());
    rclcpp::shutdown();
    return 0;
}
