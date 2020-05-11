#!/usr/bin/env python

import serial
import time
from struct import pack
from math import pi

import rospy
from std_msgs.msg import Float64 
from sensor_msgs.msg import Joy


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
    speed_x = -joy.axes[0] * (1-joy.axes[5]) * 300.0
    speed_y = +joy.axes[1] * (1-joy.axes[5]) * 300.0
    speed_w = (1-joy.axes[2]) * (1-joy.axes[5]) * 0.15 * pi
    float_bytes = pack('fff', speed_x, speed_y, speed_w)
    ser.write(float_bytes)


ser = serial_init()

rospy.init_node("commander_joy", anonymous=False)
rospy.Subscriber('joy', Joy, callback_Joy)

rate = rospy.Rate(25)
rospy.spin()

