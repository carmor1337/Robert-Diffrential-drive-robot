import serial
import time
import pygame

# Initialize serial
ser = serial.Serial('COM3', 115200)  # Change COM port to your USB port

# Initialize controller
def initialize_controller():
    pygame.init()
    pygame.joystick.init()
    joystick = pygame.joystick.Joystick(0)
    joystick.init()
    return joystick

while True:
    joystick = initialize_controller
    pygame.event.pump()

    
    
    lx = joystick.get_axis(0)
    ly = joystick.get_axis(1)
    rx = joystick.get_axis(2)
    ry = joystick.get_axis(5)
    
    x_button = joystick.get_button(0)
    circle_button = joystick.get_button(1)
    print(f"LX:{lx:.2f} LY:{ly:.2f} RX:{rx:.2f} RY:{ry:.2f} X:{x_button} O:{circle_button}")
    time.sleep(0.05)
    
    # Map float (-1.0 to 1.0) to integer speed (-255 to 255)
    left_speed = int(ly * 255)
    right_speed = int((ly + lx) * 255)

    # Send via serial in simple CSV format
    ser.write(f"{left_speed},{right_speed}\n".encode())

    time.sleep(0.05)
