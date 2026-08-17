# 2D Robot Robert
Robert is a 2D differential-drive robot using a Raspberry pi 5 as a main compute and a Raspberry pi Pico 2W as a low level/real-time controller.


## System Architecture
```mermaid
flowchart LR
    subgraph Robert
        direction TD

        ToF("ToF distance measurement")
        Camera("Camera")

        IMU["IMU"]
        Encoder("Wheel encoders")
        
        subgraph MCU["Microcontroller"]
            direction LR
            Safety["Safety / Real-time Logic"]
            SensorFusion["Sensor Fusion"]
            SensorRead["Sensor Reading"]
            MotorControl["Motor Control"]
        end
        
        subgraph RPI["Raspberry Pi 5"]
            direction LR
            SLAM("Visual SLAM")
            Vision["Computer Vision"]
            Navigation["Navigation"]
        end

        Camera -->|"Greyscale 640x480"| RPI
        ToF --> RPI
            
        MCU -->|"UART: fused IMU + encoder"| RPI


        MotorControl --> BBL298
        BBL298 --> Motors
    end
    

    IMU -->|"Gyro & Accel"| MCU
    Encoder -->|"Wheel ticks"| MCU

    Safety --> MotorControl
    SensorRead -->|"IMU + Encoder ticks"| SensorFusion
    SensorFusion -->|"(v, w) estimate"| MotorControl


    Navigation -->|"UART: (v,w) command"|MotorControl
    Vision -->|"Feature points"| SLAM -->|"Updated Map"| Navigation

   
```

## Motivation 
I started this project after having participated in the course [DD2419](https://www.kth.se/student/kurser/kurs/DD2419?l=en) at KTH and had a lot of fun so I wanted to make my own and do it all myself. In the future I would like to move to more advanced robotics platforms. 

## Goals
The goal of Robert is not only to build a working robot, but to implement the major robotics components myself and use the project to explore:

-Embedded systems
-Real-time control
-State estimation and sensor fusion
-Computer vision
-Visual-inertial SLAM
-Path planning and tracking
-Robot kinematics and dynamics

# Relationship with ROS 2
Robert is intentionally being developed without ROS for the core robotics stack. The goal of the project is to understand and implement the underlying systems myself rather than primarily integrating existing robotics frameworks. ROS will be used for intra-robot modules and communication but I will try to modulazie it as much as possible to make ROS as a communications layer.

## Hardware
Compute
- **Main compute:** [ Raspberry Pi 5 16GB](https://www.raspberrypi.com/products/raspberry-pi-5/)
- **MCU:**  [Raspberry Pi Pico 2W](https://www.raspberrypi.com/products/raspberry-pi-pico-2/)

Sensors
- **Camera** [Raspberry Pi camera module 3](https://www.raspberrypi.com/products/camera-module-3/) 
- **IMU:** [LSM6DSO](https://www.st.com/en/mems-and-sensors/lsm6dso.html)
- **ToF:** [VL53L4CD](https://www.st.com/en/imaging-and-photonics-solutions/vl53l4cd.html)
- **Encoder:** [Pololu magnetic encoders](https://www.pololu.com/product/4760)

Actuation
- **Motor driver:** [BBL298](https://www.olimex.com/Products/Robot-CNC-Parts/MotorDrivers/BB-L298/open-source-hardware)
- **Motor:**  [2 micro motors 75:1, 6V, 1.5A stall current, \#3064](https://www.pololu.com/product/3064)
- 
Mechanical
- **Wheels:** [Pololu 80mm diameter](https://www.pololu.com/product/1431)
- **Chassis** 2 [Olimex plates stacked](https://www.olimex.com/Products/Robot-CNC-Parts/Chassiss/ROBOT-3-WHEEL-KIT/)




## Progress

| Component | Status |
|------|-------------|
| Hardware | In progress |
| MCU firmware | In progress | 
| Sensor reading | In progress |
| Sensor fusion | Not implemented |
| Motor control | In progress |
| Path planning | Implemented |
| Path tracking | Not implemented |
| Visual SLAM | Not implemented |
| Computer Vision | Not implemented |

<details>
<summary> MCU Tasks and progress </summary>

| Motor control | Status |
|------|-------------|
| Hardware | In progress |


| Sensor fusion | Status |
|------|-------------|
| Hardware | In progress |

| Drivers | Status |
|------|-------------|
| Hardware | In progress |

| RPI 5 comms | Status |
|------|-------------|
| Hardware | In progress |

</details>


## Software diagram


<details>
    
<summary>Robert overview </summary>

```mermaid
sequenceDiagram

participant User
participant SLAM
participant Navigation
participant MCU

User ->> Navigation: Given waypoint
Navigation ->> SLAM: Access map
SLAM -->> Navigation: Return map
Navigation -->> Navigation: Convert global waypoint to robot frame

    Navigation --> Navigation: Plan path
loop Path tracking
    Navigation ->> SLAM: Access pose
    SLAM -->> Navigation: Return pose
    Navigation -->> Navigation: Track path
    Navigation ->> MCU: (v,w) command
end

```
</details>

<details>
<summary>RPI 5 Overview</summary>


```mermaid
---
title: Node
---
flowchart TD
    subgraph Navigation
        Waypoint["Waypoint request"]
        PP["Path planning"]
        PT["Path tracking"]
        MCU["MCU"]
    end

    subgraph Visual SLAM
        Frontend["Visual frontend"]
        Backend["Visual backend"]
    end

    Waypoint --World frame-->PP-- Path--> PT --Track path--> MCU
    Frontend --  VIO --> Backend

    Backend -- Map data-->PP
    Backend -- Pose data-->PT

```
</details>


## MCU diagram

<details>
<summary>MCU Overview</summary>

```mermaid
sequenceDiagram

participant RPI 5
participant Encoders
participant IMU
participant MCU
participant Motor driver

    RPI 5 ->> MCU: (v,w) command
    IMU -->> MCU: Accel and Gyro data
    Encoders -->> MCU: Wheel ticks
    MCU -->> MCU: Sensor Fusion
    MCU ->> Motor driver: PWM command

```


</details>




