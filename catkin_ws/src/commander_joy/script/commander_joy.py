#!/usr/bin/env python

import serial
import time
from struct import pack
from math import pi

import rospy
from std_msgs.msg import Float64 
from sensor_msgs.msg import Joy


MIN_SPEED = 100.0 # min: 100 mm/s (or 0.1*pi rad/s)
MAX_SPEED = 250.0 # max: 600 mm/s (or 0.6*pi rad/s)
ITERATION_TO_CONTINUE = 0


def serial_init():
    ser = serial.Serial("/dev/ttyAMA0",74880)

    if not ser.isOpen():
        rospy.logfatal("serial port open failed")
    else:
        rospy.loginfo("serial port open success: ")
        rospy.loginfo(ser)
    time.sleep(2)
    return ser


def callback_Joy(joy):
    global ser
    global speed_x, speed_y, speed_w, ITERATION_TO_CONTINUE
    ITERATION_TO_CONTINUE = 200
    speed_x = -joy.axes[0] * ((0.5-0.5*joy.axes[5]) * (MAX_SPEED - MIN_SPEED) + MIN_SPEED)
    speed_y = +joy.axes[1] * ((0.5-0.5*joy.axes[5]) * (MAX_SPEED - MIN_SPEED) + MIN_SPEED)
    speed_w = +joy.axes[3] * ((0.5-0.5*joy.axes[5]) * (MAX_SPEED - MIN_SPEED) + MIN_SPEED) * 0.001 * pi # max: 0.6*pi rad/s
    

speed_x = 0.0
speed_y = 0.0
speed_w = 0.0

rospy.init_node("commander_joy", anonymous=False)
ser = serial_init()

rospy.Subscriber('joy', Joy, callback_Joy)

rate = rospy.Rate(25)
while not rospy.is_shutdown():
    if ITERATION_TO_CONTINUE > 0:
        ITERATION_TO_CONTINUE -= 1
    else:
        speed_x = 0.0
        speed_y = 0.0
        speed_w = 0.0
    rospy.loginfo("x: %f  y: %f  w: %f", speed_x, speed_y, speed_w)
    float_bytes = pack('fff', speed_x, speed_y, speed_w)
    ser.write(float_bytes)
    rate.sleep()

