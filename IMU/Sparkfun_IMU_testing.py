#!/usr/bin/env python
from smbus2 import SMBus
import time
"""
name
INT1_CTRL = 0D
Gyroscope type registers

OUTX_L_G r 22h
OUTX_H_G r 23h 
OUTY_L_G r 24h 
OUTY_H_G r 25h 
OUTZ_L_G r 26h 
OUTZ_H_G r 27h 
OUTX_L_XL r 28h 

Accelerometer output register

OUTX_H_XL r 29 00101001 output
OUTY_L_XL r 2A 00101010 output
OUTY_H_XL r 2B 00101011 output
OUTZ_L_XL r 2C 00101100 output
OUTZ_H_XL r 2D 00101101 

STATUS_REG r 1E
"""
# Gyroscope data registers
OUTX_L_G = 0x22
OUTX_H_G = 0x23
OUTY_L_G = 0x24 
OUTY_H_G = 0x25 
OUTZ_L_G = 0x26 
OUTZ_H_G = 0x27 
# Accelerometer data registers
OUTX_L_XL = 0x28 
OUTX_H_XL = 0x29 
OUTY_L_XL = 0x2A 
OUTY_H_XL = 0x2B 
OUTZ_L_XL = 0x2C 
OUTZ_H_XL = 0x2D 

# Register for status of accelerometer,gyroscope and temperature

STATUS_REG = 0x1E
# Accelerometer Control register
CTRL1_XL = 0x10
# Gyroscope Control register: 
CTRL2_G= 0x11 
# interrupt controll
INT1_CTRL = 0x0D
INT2_CTRL = 0x0E

WHO_AM_I = 0x0F

I2C_ADDRESS = 0x6b

def activate(bus):
    bus.write_byte_data(I2C_ADDRESS, CTRL1_XL,0x40) # Activate accelerometer
    bus.write_byte_data(I2C_ADDRESS, CTRL2_G,0x40) # Activate gyroscope
    
    # Maybe need to verify that the bits are set correctly
    
    # Tror att int_ctrl = 03h 
    # Accelerometer
    #1. Write INT1_CTRL = 01h // Acc data-ready interrupt on INT1
    #2. Write CTRL1_XL = 40h
    # Gyroskope
    # 1. Write INT1_CTRL = 02h // Gyro data-ready interrupt on INT1
    # 2. Write CTRL2_G = 40h // Gyro = 417 Hz (High-Performance mode)
    
    
# 400khz i2c speed
def read_status_reg_and_fetch_data(bus):
    status_reg_read = bus.read_byte_data(I2C_ADDRESS, STATUS_REG)
    print(f"status bit is {bin(status_reg_read)}")
    accelerometer_bit = (status_reg_read >> 0) & 1
    gyroscope_bit = (status_reg_read >> 1) & 1
    tempreature_bit = (status_reg_read >> 2) & 1
    if accelerometer_bit and gyroscope_bit:
        accel_data = get_accelerometer_data(bus)
        gyro_data = get_gyro_data(bus)
        return [accel_data,gyro_data]
    """
    if accelerometer_bit:
        accel_data = get_accelerometer_data(bus)
        return accel_data
    
    if gyroscope_bit:
        gyro_data = get_gyro_data(bus)
        return gyro_data"""
    return []
        
    

def get_accelerometer_data(bus):
    x_l = bus.read_byte_data(I2C_ADDRESS,OUTX_L_XL)
    x_H = bus.read_byte_data(I2C_ADDRESS,OUTX_H_XL)
    accel_x = combine_data(x_H,x_l)
    
    y_l = bus.read_byte_data(I2C_ADDRESS,OUTY_L_XL)
    y_H = bus.read_byte_data(I2C_ADDRESS,OUTY_H_XL)
    accel_y = combine_data(y_H,y_l)
    
    z_l = bus.read_byte_data(I2C_ADDRESS,OUTZ_L_XL)
    z_H = bus.read_byte_data(I2C_ADDRESS,OUTZ_H_XL)
    accel_z = combine_data(z_H,z_l)
    
    return [accel_x,accel_y,accel_z]

def get_gyro_data(bus):
    x_l = bus.read_byte_data(I2C_ADDRESS,OUTX_L_G)
    x_H = bus.read_byte_data(I2C_ADDRESS,OUTX_H_G)
    gyro_x = combine_data(x_H,x_l)
    
    y_l = bus.read_byte_data(I2C_ADDRESS,OUTY_L_G)
    y_H = bus.read_byte_data(I2C_ADDRESS,OUTY_H_G)
    gyro_y = combine_data(y_H,y_l)
    
    z_l = bus.read_byte_data(I2C_ADDRESS,OUTZ_L_G)
    z_H = bus.read_byte_data(I2C_ADDRESS,OUTZ_H_G)
    gyro_z = combine_data(z_H,z_l)
    
    return [gyro_x,gyro_y,gyro_z]

def connection_test(bus):
    test_byte = bus.read_byte_data(I2C_ADDRESS,WHO_AM_I)
    if test_byte == 0x0F:
        print("Connection test Sucessfull")
    else:
        print("Connection test Failed")
        
def combine_data(MSB,LSB):
    return (MSB << 8) | LSB

def main():
    '''
    Main program function
    '''
    gyro_printing_list = ["\nGyro x: ","Gyro y: ","Gyro z: "]
    accel_printing_list = ["Accel x: ","Accel y: ","Accel z: "]
    addr = 1
    i2cbus = SMBus(1)
    
    test = i2cbus.read_byte_data(I2C_ADDRESS,WHO_AM_I)
    print(test)
    print(bin(test))
    activate(i2cbus)
    old_data = 0
    connection_test(i2cbus)
    while True:
        new_data = read_status_reg_and_fetch_data(i2cbus)
        if not new_data:
            print(f'Accel x: {new_data[0][0]},Accel y: {new_data[0][1]} Accel z: {new_data[0][2]}\n')
            print(f'Gyro x: {new_data[1][0]},Gyro y: {new_data[1][1]} Gyro z: {new_data[1][2]}\n')
        else:
            print(' No new data')
        time.sleep(1)
                
        
        


if __name__ == "__main__":
    main()

