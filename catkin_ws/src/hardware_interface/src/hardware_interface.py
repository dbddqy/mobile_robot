#!/usr/bin/env python

# ===============================================================================
# this node listen to /vel_command and send velocity commands to the robot
#           listen to the robot feedback and pub to /vel_feedback
# ===============================================================================

import serial
import time
from struct import pack, unpack
from math import pi

import rospy
from geometry_msgs.msg import Twist


# STRAT_BYTE = b'\x2F'
# END_BYTE = b'\x3F'

CHASSIS_LENGTH = 431.0 # mm
CHASSIS_WIDTH = 303.5
CHASSIS_WHEEL_DIAMETER = 152.0 # mm
CHASSIS_WHEEL_RADIUS = (CHASSIS_WHEEL_DIAMETER/2.0)
CHASSIS_REDUCTION = 36

ITERATION_TO_CONTINUE = 0
MIN_SPEED = 100.0 # min: 100 mm/s (or 0.1*pi rad/s)
MAX_SPEED = 250.0 # max: 250 mm/s (or 0.25*pi rad/s)

speed_x = 0.0
speed_y = 0.0
speed_w = 0.0


def serial_init():
    ser = serial.Serial("/dev/ttyAMA0",74800)

    if not ser.isOpen():
        rospy.logfatal("serial port open failed")
    else:
        rospy.loginfo("serial port open success: ")
        rospy.loginfo(ser)
    time.sleep(2)
    return ser


def limit_speed(speed):
    global MIN_SPEED, MAX_SPEED
    if abs(speed) < MIN_SPEED:
        return 0.
    elif abs(speed) > MAX_SPEED:
        return MAX_SPEED
    else:
        return speed


def constructVelocity(w1, w2, w3, w4):
    factor = pi * CHASSIS_WHEEL_DIAMETER / CHASSIS_REDUCTION / 60.0 
    vx = factor * (w2 - w3 + w1 - w4) * 0.25 # mm/s
    vy = factor * (w2 - w1 + w3 - w4) * 0.25 # mm/s
    w = factor * (-w2 - w4 - w1 - w3) * 0.5 / (CHASSIS_LENGTH + CHASSIS_WIDTH) # rad/s
    msg = Twist()
    msg.linear.x = vx * 0.001
    msg.linear.y = vy * 0.001
    msg.angular.z = w
    return msg


def callback_Twist(twist):
    global speed_x, speed_y, speed_w, ITERATION_TO_CONTINUE
    ITERATION_TO_CONTINUE = 25 # run 1 second without input
    speed_x = limit_speed(twist.linear.x * 1000.0) # m/s -> mm/s
    speed_y = limit_speed(twist.linear.y * 1000.0) # m/s -> mm/s
    speed_w = limit_speed(twist.angular.z * 1000.0) * 0.001 * pi # rad/s -> 0.001rad/s


rospy.init_node("hardware_interface", anonymous=False)
ser = serial_init()

pub = rospy.Publisher('vel_feedback', Twist , queue_size=10)
rospy.Subscriber("vel_command", Twist, callback_Twist)

rate = rospy.Rate(25) # 25Hz
while not rospy.is_shutdown():
    # send control command
    if ITERATION_TO_CONTINUE > 0:
        ITERATION_TO_CONTINUE -= 1
    else:
        speed_x = 0.0
        speed_y = 0.0
        speed_w = 0.0
    # rospy.loginfo("x: %f  y: %f  w: %f", speed_x, speed_y, speed_w)
    float_bytes = pack("fff", speed_x, speed_y, speed_w)
    # ser.write(STRAT_BYTE)
    ser.write(float_bytes)
    # ser.write(END_BYTE)
    # receive wheel states
    ser.flushInput()
    while (ser.inWaiting() < 8):
        pass
    recv_bytes = ser.read(8)
    recv_bytes_ordered = recv_bytes[::-1]
    w4, w3, w2, w1 = unpack("hhhh", recv_bytes_ordered)
    msg = constructVelocity(w1, w2, w3, w4)
    pub.publish(msg)
    # rospy.loginfo("vx: %f  vy: %f  w: %f", vx, vy, w)
    rate.sleep()
