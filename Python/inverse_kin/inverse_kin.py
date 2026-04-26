"""Module example for only inverse kinematics"""
from math import acos, sqrt, pi, atan2

#Length upper and lower globals
LEN_U_L = 60
LEN_L_L = 112

#Inverse kinematics calculations
def servo_calc(x_co_ord, y_co_ord, z_co_ord):
    #Distance to tip
    dist_d = sqrt(x_co_ord**2 + y_co_ord**2)

    #Hypotenuse distance
    hypotenuse = sqrt(dist_d**2 + z_co_ord**2)

    #Angle calculations
    hip_a = atan2(y_co_ord, x_co_ord) * 180/pi
    alpha = atan2(dist_d, -z_co_ord) * 180/pi
    beta = acos((LEN_U_L**2 + hypotenuse**2 - LEN_L_L**2)/(2*LEN_U_L*hypotenuse)) * 180/pi
    shoulder_a = alpha + beta
    elbow_a = 180 - acos((LEN_U_L**2 + LEN_L_L**2 - hypotenuse**2)/(2*LEN_U_L*LEN_L_L)) * 180/pi

    print(f"Theta 1 (hip) is: {hip_a} Theta 2 (shoulder) is: {shoulder_a} Theta 3 (elbow) is: {elbow_a} ")


#X, Y Z is passed or hip, horizontal and vertical
servo_calc(0,100, -61) 
