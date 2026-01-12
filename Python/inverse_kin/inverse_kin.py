"""Module example for only inverse kinematics"""
from math import acos,atan, sqrt, pi, atan2

#Length upper and lower here
LEN_U_L = 60
a = 60
b = 112
LEN_L_L = 112

#Beginning of inverse kinematics calculations
def servo_calc(x, y, z):
    d = sqrt(x**2 + y**2)
    c = sqrt(d**2 + z**2)
    theta1 = atan2(y, x) * 180/pi
    alpha = atan2(d, -z) * 180/pi
    beta = acos((a**2 + c**2 - b**2)/(2*a*c)) * 180/pi
    theta2 = alpha + beta
    theta3 = 180 - acos((a**2 + b**2 - c**2)/(2*a*b)) * 180/pi
    print(f"Theta 1 is: {theta1} Theta 2 is: {theta2} Theta 3 is: {theta3} ")


#X, Y Z is passed or hip, horizontal and vertical
servo_calc(0,47, -46) 
