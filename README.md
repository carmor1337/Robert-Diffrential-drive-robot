# 2D Robot Robert
My personal project on a 2D robot for fun

## What
```mermaid
flowchart LR
    subgraph Robert
        direction TD

        TOF("TOF distance measurment")
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
        TOF --> RPI
            
        MCU -->|"UART: fused IMU + encoder"| RPI


        MotorControl --> BBL298
        BBL298 --> Motors
    end
    

    IMU -->|"Gyro & Accel"| MCU
    Encoder -->|"Wheel tics"| MCU

    Safety --> MotorControl
    SensorRead -->|"IMU + Encoder ticks"| SensorFusion
    SensorFusion -->|"(v, w) estimate"| MotorControl


    Navigation -->|"UART: (v,w) command"|MotorControl
    Vision -->|"Feature points"| SLAM -->|"Updated Map"| Navigation

   
```

## Why 

## Hardware
TODO add links to the products
- **Main compute:** [ Raspberry Pi 5 16GB](https://www.raspberrypi.com/products/raspberry-pi-5/)
- **Camera** [Raspberry Pi camera module 3](https://www.raspberrypi.com/products/camera-module-3/) 
- **MCU:**  [Raspberry Pi Pico 2W](https://www.raspberrypi.com/products/raspberry-pi-pico-2/)
- **IMU:** [LSM6DSO](https://www.st.com/en/mems-and-sensors/lsm6dso.html)
- **TOF:** [VL53L4CD](https://www.st.com/en/imaging-and-photonics-solutions/vl53l4cd.html)
- **Encoder:** [Pololu magnetic encoders]()
- **Motor driver:** [BBL298](https://www.olimex.com/Products/Robot-CNC-Parts/MotorDrivers/BB-L298/open-source-hardware)
- **Motor:**  [2 Micro motors 75:1, 6V, 1.5A stall current, \#3064](https://www.pololu.com/product/3064)
- **Wheels:** [Pololu 80mm diameter](https://www.pololu.com/product/1431)
- **Chassi** 2 [Olimex plates stacked](https://www.olimex.com/Products/Robot-CNC-Parts/Chassis/ROBOT-3-WHEEL-KIT/)

## Progress

| Robert feature | Status |
|------|-------------|
| SLAM | To be implemented |
| Path planning | Algorithm done | 
| Path tracking| To be implemented |
| MCU | Under construction  |


## Diagrams


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
        MCU["Mcu"]
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

Some explanatory text can go here too.

</details>




