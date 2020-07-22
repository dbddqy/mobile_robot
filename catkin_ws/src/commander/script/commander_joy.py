#!/usr/bin/env python

# =============================================================
# this node listen to a /joy and publish /vel_command 
# =============================================================

from math import pi

import rospy
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist


MIN_SPEED = 0.15 # min: 100 mm/s (or 0.1*pi rad/s)
MAX_SPEED = 0.25 # max: 250 mm/s (or 0.25*pi rad/s)

ITERATION_TO_CONTINUE = 0

speed_x = 0.0
speed_y = 0.0
speed_w = 0.0

def callback_Joy(joy):
    global speed_x, speed_y, speed_w, ITERATION_TO_CONTINUE
    ITERATION_TO_CONTINUE = 25
    speed_x = -joy.axes[0] * ((0.5-0.5*joy.axes[5]) * (MAX_SPEED - MIN_SPEED) + MIN_SPEED)
    speed_y = +joy.axes[1] * ((0.5-0.5*joy.axes[5]) * (MAX_SPEED - MIN_SPEED) + MIN_SPEED)
    speed_w = +joy.axes[3] * ((0.5-0.5*joy.axes[5]) * (MAX_SPEED - MIN_SPEED) + MIN_SPEED)


rospy.init_node("commander_joy", anonymous=False)

pub = rospy.Publisher('vel_command', Twist , queue_size=10)
rospy.Subscriber("joy", Joy, callback_Joy)

rate = rospy.Rate(25)
while not rospy.is_shutdown():
    if ITERATION_TO_CONTINUE > 0:
        ITERATION_TO_CONTINUE -= 1
    else:
        speed_x = 0.0
        speed_y = 0.0
        speed_w = 0.0
    msg = Twist()
    msg.linear.x = speed_x
    msg.linear.y = speed_y
    msg.angular.z = speed_w
    pub.publish(msg)
    rate.sleep()

