# inspire_hand

## Dependencies

- ROS2 Humble
- libmodbus: `sudo apt install libmodbus-dev`

## Build

```bash
source /opt/ros/humble/setup.bash
colcon build
source install/setup.bash
```

## Run

```bash
ros2 run inspire_hand_driver inspire_hand_driver
```

파라미터 오버라이드:

```bash
ros2 run inspire_hand_driver inspire_hand_driver \
  --ros-args \
  -p right_ip:=<RIGHT_HAND_IP> \
  -p left_ip:=<LEFT_HAND_IP> \
  -p state_hz:=50.0
```

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `right_ip` | — | 오른손 Modbus TCP IP |
| `left_ip` | — | 왼손 Modbus TCP IP |
| `modbus_port` | `6000` | Modbus TCP 포트 |
| `device_id` | `1` | Modbus device ID |
| `state_hz` | `50.0` | 상태 read/publish 주기 (Hz) |

## Topics

| Topic | Type | Direction |
|-------|------|-----------|
| `/rt/inspire_hand/ctrl/r` | `InspireHandCtrl` | subscribe |
| `/rt/inspire_hand/ctrl/l` | `InspireHandCtrl` | subscribe |
| `/rt/inspire_hand/state/r` | `InspireHandState` | publish |
| `/rt/inspire_hand/state/l` | `InspireHandState` | publish |
