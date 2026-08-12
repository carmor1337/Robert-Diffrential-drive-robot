# 2D Robot Robert
My personal project on a 2D robot for fun

## What
```mermaid
flowchart LR
    subgraph Robert
        direction LR

        

        subgraph MCU["Microcontroller"]
            direction LR
            Safety["Safety / Real-time Logic"]
            SensorFusion["Sensor Fusion"]
            SensorRead["Sensor Reading"]
            MotorControl["Motor Control"]
        end
        
        subgraph RPI["Raspberry Pi 5"]
            direction TD
            SLAM("Visual SLAM")
            Vision["Computer Vision"]
            Navigation["Navigation"]
            BehaviourTree["BehaviourTree"]
            MCUData("MCU data")
        end

        Camera --> RPI
        TOF --> RPI

        IMU["IMU"]
        Encoder("Wheel encoders")

        TOF("TOF distance measurment")
        Camera("Camera")
            
        MCU <-->|"UART"| RPI
        MCU -->|"IMU + Encoder data"| SLAM

        MotorControl --> BBL298
        BBL298 --> Motors
    end
    

    IMU -->|"Gyro & Accel"| MCU
    Encoder -->|"Wheel tics"| MCU

    Safety --> MotorControl
    SensorRead --> SensorFusion
    SensorFusion -->|"(v, w) estimate"| MotorControl


    MCUData -->|"IMU + Encoder"|SLAM
    Navigation -->|"(v,w) command"|MotorControl
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



## Diagrams
<details>
    <summary>High level interaction</summary>
    <p>Lorem ipsum dolor sit amet, ...</p>
</details>

<details>
    <summary>RPI 5 and MCU</summary>
    <p>Lorem ipsum dolor sit amet, ...</p>
</details>

<details>
    <summary>MCU Intercation</summary>
    <p>Lorem ipsum dolor sit amet, ...</p>
</details>

<details>
    <summary>High level interaction</summary>
    <p>```mermaid
graph TD
    A[Start] --> B[Build]
    B --> C[Test]
    C --> D[Deploy]
```</p>
</details>

