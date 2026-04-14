#pragma once

#include <rclcpp/rclcpp.hpp>
#include <inspire_hand_msgs/msg/inspire_hand_ctrl.hpp>
#include <inspire_hand_msgs/msg/inspire_hand_state.hpp>
// #include <inspire_hand_msgs/msg/inspire_hand_touch.hpp>  // touch 활성화 시 해제
#include "inspire_hand_driver/modbus_client.hpp"

#include <mutex>
#include <memory>
#include <optional>

namespace inspire_hand_driver
{

namespace reg
{
  // write
  constexpr int POS_SET   = 1474;
  constexpr int ANGLE_SET = 1486;
  constexpr int FORCE_SET = 1498;
  constexpr int SPEED_SET = 1522;

  // read (각 필드 개별 read — 빈 레지스터 오염 방지)
  constexpr int POS_ACT   = 1534;
  constexpr int ANGLE_ACT = 1546;
  constexpr int FORCE_ACT = 1582;
  constexpr int CURRENT   = 1594;
  constexpr int ERR       = 1606;
  constexpr int STATUS    = 1612;
  constexpr int TEMP      = 1618;

  constexpr int RESET_ERR = 1004;

  // touch 레지스터 (3000번대) — 활성화 시 사용
  // constexpr int TOUCH_FINGERONE_TIP    = 3000;   // 18 reg →  9 values
  // constexpr int TOUCH_FINGERONE_TOP    = 3018;   // 192 reg → 96 values
  // constexpr int TOUCH_FINGERONE_PALM   = 3210;   // 160 reg → 80 values
  // constexpr int TOUCH_FINGERTWO_TIP    = 3370;
  // constexpr int TOUCH_FINGERTWO_TOP    = 3388;
  // constexpr int TOUCH_FINGERTWO_PALM   = 3580;
  // constexpr int TOUCH_FINGERTHREE_TIP  = 3740;
  // constexpr int TOUCH_FINGERTHREE_TOP  = 3758;
  // constexpr int TOUCH_FINGERTHREE_PALM = 3950;
  // constexpr int TOUCH_FINGERFOUR_TIP   = 4110;
  // constexpr int TOUCH_FINGERFOUR_TOP   = 4128;
  // constexpr int TOUCH_FINGERFOUR_PALM  = 4320;
  // constexpr int TOUCH_FINGERFIVE_TIP    = 4480;
  // constexpr int TOUCH_FINGERFIVE_TOP    = 4498;
  // constexpr int TOUCH_FINGERFIVE_MIDDLE = 4690;
  // constexpr int TOUCH_FINGERFIVE_PALM   = 4708;
  // constexpr int TOUCH_PALM              = 4900;   // 224 reg → 112 values
}

struct HandConfig {
  std::string ip;
  std::string side;  // "l" or "r"
  int port;
  int device_id;
};

class InspireHandDriver : public rclcpp::Node
{
public:
  explicit InspireHandDriver(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());
  ~InspireHandDriver() override;

private:
  using CtrlMsg  = inspire_hand_msgs::msg::InspireHandCtrl;
  using StateMsg = inspire_hand_msgs::msg::InspireHandState;
  // using TouchMsg = inspire_hand_msgs::msg::InspireHandTouch;  // touch 활성화 시 해제

  struct Hand {
    HandConfig config;
    std::unique_ptr<ModbusClient> modbus;

    // ctrl 최신 명령 캐시 (callback → timer 전달)
    std::mutex cache_mutex;
    std::optional<CtrlMsg> cached_ctrl;

    rclcpp::Subscription<CtrlMsg>::SharedPtr sub;
    rclcpp::Publisher<StateMsg>::SharedPtr state_pub;
    // rclcpp::Publisher<TouchMsg>::SharedPtr touch_pub;  // touch 활성화 시 해제
  };

  Hand left_;
  Hand right_;

  rclcpp::TimerBase::SharedPtr timer_;

  void init_hand(Hand & hand);
  void tick(Hand & hand);  // 타이머에서 호출: write + read + publish
  void do_write(Hand & hand, const CtrlMsg & msg);
  void do_read_publish(Hand & hand);
  // void do_touch_read_publish(Hand & hand);  // touch 활성화 시 해제
  bool ensure_connected(Hand & hand);
};

}  // namespace inspire_hand_driver
