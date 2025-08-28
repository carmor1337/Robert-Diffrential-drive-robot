import pygame
import time


pygame.init()
pygame.joystick.init()

joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"Controller connected: {joystick.get_name()}")


while True:
    pygame.event.pump()
    lx = joystick.get_axis(0)
    ly = joystick.get_axis(1)
    rx = joystick.get_axis(2)
    ry = joystick.get_axis(5)
    
    x_button = joystick.get_button(0)
    circle_button = joystick.get_button(1)
    print(f"LX:{lx:.2f} LY:{ly:.2f} RX:{rx:.2f} RY:{ry:.2f} X:{x_button} O:{circle_button}")
    time.sleep(0.05)
    