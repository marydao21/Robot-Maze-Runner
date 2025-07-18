# Robot Maze Runner

An Arduino-based autonomous robot that navigates through mazes using IR sensors and servo motors. The robot is designed to avoid walls and stop when there's no clear path available.

## Features

- **Three IR Sensors**: Left, right, and middle sensors for comprehensive wall detection
- **LED Indicators**: Visual feedback showing which sensors detect walls
- **Smart Navigation**: Advanced algorithms to prevent getting stuck
- **End-of-Maze Detection**: Automatically stops when no path is available
- **Serial Debugging**: Real-time sensor data output for troubleshooting
- **Robust Recovery**: Multiple strategies to handle stuck situations

## Hardware Requirements

### Components
- Arduino Uno or compatible board
- 2x Servo motors (for left and right wheels)
- 3x IR LED and receiver pairs
- 3x Red LEDs (for visual indicators)
- Robot chassis with wheels
- Power supply (battery pack recommended)

### Pin Connections

| Component | Pin | Description |
|-----------|-----|-------------|
| Left Servo | 8 | Left wheel motor control |
| Right Servo | 9 | Right wheel motor control |
| Left IR LED | 10 | Left IR transmitter |
| Left IR Receiver | 11 | Left IR receiver |
| Left Red LED | A2 | Left sensor indicator |
| Right IR LED | 2 | Right IR transmitter |
| Right IR Receiver | 3 | Right IR receiver |
| Right Red LED | A0 | Right sensor indicator |
| Middle IR LED | 6 | Middle IR transmitter |
| Middle IR Receiver | 7 | Middle IR receiver |
| Middle Red LED | A1 | Middle sensor indicator |

## Code Features

### 1. **Improved Sensor Accuracy**
- `getAverageDistance()` function takes 3 samples and averages them
- Reduces false readings and improves reliability
- Configurable sample count

### 2. **Enhanced Safety Parameters**
- `maxDistance = 4` for better wall avoidance
- `minDistance = 2` for critical situations
- Configurable delays for different movement types

### 3. **Smart Stuck Detection**
- Monitors when all sensors detect walls simultaneously
- Implements recovery strategies with multiple attempts
- Prevents infinite loops and damage to the robot

### 4. **End-of-Maze Detection**
- Tracks consecutive "no path" situations
- Automatically stops after 5 consecutive blocked paths
- Prevents the robot from running indefinitely

### 5. **Visual Feedback**
- LED indicators show which sensors detect walls
- Real-time serial output for debugging
- Clear status messages for different situations

### 6. **Robust Movement Control**
- Separate functions for different turn types
- Configurable servo control values
- Proper delays for smooth movement

## Configuration Parameters

These values are defined in the code and can be adjusted:

```cpp
const int maxDistance = 4;        // Maximum distance to avoid walls
const int minDistance = 2;        // Minimum distance threshold
const int turnDelay = 800;        // Delay for 90-degree turns
const int slightTurnDelay = 200;  // Delay for slight turns
const int moveDelay = 150;        // Delay for forward/backward movement
const int sensorDelay = 50;       // Delay for sensor readings
const int maxStuckCount = 10;     // Maximum stuck recovery attempts
const int maxNoPathCount = 5;     // Maximum consecutive no-path situations
```

## Servo Control Values

```cpp
const int servoStop = 1500;
const int servoForwardLeft = 1400;
const int servoForwardRight = 1600;
const int servoBackward = 1400;
const int servoTurnLeft = 1400;
const int servoTurnRight = 1600;
```

## How It Works

### 1. **Sensor Reading**
The robot continuously reads from all three IR sensors using frequency sweeps (38kHz to 42kHz) and averages multiple readings for accuracy.

### 2. **Navigation Logic**
- **Clear Path**: If middle sensor shows clear path, move forward
- **Wall Ahead**: If middle sensor detects wall, stop, back up, and turn toward the clearer side
- **Side Walls**: If left/right sensors detect walls, make slight turns to avoid them
- **Complete Block**: If all sensors detect walls, attempt recovery maneuvers

### 3. **Stuck Recovery**
When the robot gets stuck:
1. Move backward
2. Turn in alternating directions
3. Try up to 10 recovery attempts
4. Stop if recovery fails

### 4. **End Detection**
The robot stops when:
- All sensors consistently detect walls for 5 consecutive cycles
- Maximum stuck recovery attempts are reached
- No clear path is available

## Setup Instructions

1. **Hardware Assembly**
   - Mount the Arduino on the robot chassis
   - Connect servos to pins 8 and 9
   - Install IR sensors at the front (left, right, middle)
   - Connect red LEDs for visual feedback
   - Ensure proper power supply

2. **Software Setup**
   - Install Arduino IDE
   - The Servo library is included by default
   - Upload the `Robot_Maze_Runner.ino` file
   - Open Serial Monitor at 9600 baud for debugging

3. **Calibration**
   - Place the robot in a test environment
   - Adjust `maxDistance` value based on your maze wall distance
   - Fine-tune servo values if needed for your specific motors
   - Test in a simple maze first

## Troubleshooting

### Common Issues

1. **Robot hits walls**
   - Increase `maxDistance` value
   - Check IR sensor alignment
   - Verify sensor connections

2. **Robot gets stuck frequently**
   - Increase `maxStuckCount`
   - Adjust turn delays
   - Check for mechanical issues

3. **Sensors not responding**
   - Verify wiring connections
   - Check IR LED and receiver pairs
   - Ensure proper power supply

4. **Erratic movement**
   - Calibrate servo center positions
   - Adjust movement delays
   - Check for loose connections

### Debug Information

The Serial Monitor provides real-time information:
- Sensor distance readings
- Movement decisions
- Stuck detection
- End-of-maze status

## Safety Features

- **Automatic Stop**: Prevents damage when no path is available
- **Stuck Recovery**: Multiple strategies to handle difficult situations
- **LED Indicators**: Visual feedback for troubleshooting
- **Configurable Limits**: Prevents infinite loops

## Future Enhancements

- **Path Memory**: Remember visited locations to avoid loops
- **Speed Control**: Variable speed based on sensor readings
- **Battery Monitoring**: Low battery detection and safe shutdown
- **Remote Control**: Manual override capability
- **Maze Mapping**: Create a map of the explored maze

## License

This project is open source and available under the MIT License.

## Contributing

Feel free to submit issues, feature requests, or pull requests to improve the robot's performance and capabilities.