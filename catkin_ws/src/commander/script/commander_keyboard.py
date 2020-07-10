#!/usr/bin/env python

# =============================================================
# this node listen to a joy topic and publish topic vel_command 
# =============================================================

from math import pi

import rospy
from geometry_msgs.msg import Twist


MIN_SPEED = 100.0 # min: 100 mm/s (or 0.1*pi rad/s)
MAX_SPEED = 250.0 # max: 250 mm/s (or 0.25*pi rad/s)

speed_x = 0.0
speed_y = 0.0
speed_w = 0.0

def callback_Twist(twist):
    global speed_x, speed_y, speed_w
    if twist.linear.x < 1.0:
        speed_x = twist.linear.x * MAX_SPEED
    else:
        speed_x = MAX_SPEED
    if twist.linear.y < 1.0:
        speed_y = twist.linear.y * MAX_SPEED
    else:
        speed_y = MAX_SPEED
    if twist.angular.z < 2.0:
        speed_w = twist.angular.z * 0.5 * MAX_SPEED * 0.001 * pi
    else:
        speed_w = MAX_SPEED * 0.001 * pi


rospy.init_node("commander_keyboard", anonymous=False)

pub = rospy.Publisher("vel_command", Twist , queue_size=10)
rospy.Subscriber("cmd_vel", Twist, callback_Twist)

rate = rospy.Rate(5)
while not rospy.is_shutdown():
    msg = Twist()
    msg.linear.x = speed_x
    msg.linear.y = speed_y
    msg.angular.z = speed_w
    pub.publish(msg)
    rate.sleep()

