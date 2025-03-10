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

#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <random>

using std::placeholders::_1;

class ResponderNode : public rclcpp::Node {
public:
    ResponderNode() : Node("responder_node") {
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "question", 10, std::bind(&ResponderNode::question_callback, this, _1));
        publisher_ = this->create_publisher<std_msgs::msg::String>("answer", 10);
    }

private:
    void question_callback(const std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Received: %s", msg->data.c_str());
        
        auto message = std_msgs::msg::String();
        message.data = (random_decision() ? "Yes" : "No");
        
        RCLCPP_INFO(this->get_logger(), "Responding: %s", message.data.c_str());
        publisher_->publish(message);
    }

    bool random_decision() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dist(0, 1);
        return dist(gen) == 1;
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ResponderNode>());
    rclcpp::shutdown();
    return 0;
}
