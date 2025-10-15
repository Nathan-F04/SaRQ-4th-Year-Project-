"""Module example for only inverse kinematics"""
from math import acos,atan, sqrt, pi

#Globals
#Length upper and lower here
LEN_U_L = 57
LEN_L_L = 110
STARTING_Y = 70
STARTING_Z = -80
ANG_C = 30
#Beginning of inverse kinematics calculations
#Check if I need to change to degrees instead of radians
def servo_calc(x_coord, y_coord, z_coord):
    """Calls methods to get servo angles before sending them over serial"""
    s1 = j1_calc(x_coord, y_coord+STARTING_Y)
    s2 = j2_calc(x_coord, y_coord+STARTING_Y, z_coord+STARTING_Z)
    s3 = j3_calc(x_coord, y_coord+STARTING_Y, z_coord+STARTING_Z)

    print(s1)
    print(s2)
    print(s3)
    #Pyserial will eventually be sent below

def j1_calc(x_len, y_len):
    """Calculates hip servo angle"""
    if x_len == 0:
        j1 = 0
        return j1
    else:
        j1 = atan(x_len/y_len)
        j1 = (j1*180)/pi
        return j1

def j2_calc(x_len,y_len,z_len):
    """Calculates shoulder servo angle"""
    hyp_top = hyp_top_calc(x_len, y_len)
    hyp_side = hyp_side_calc(hyp_top, z_len)
    b = acos((pow(hyp_side,2)+pow(LEN_U_L,2)-pow(LEN_L_L,2))/(2*hyp_side*LEN_U_L))
    b = b *(180)/pi
    a = atan(z_len/hyp_top)
    a = a *(180)/pi

    #Addition since A is negative - double check

    j2 = b + a
    return j2

def j3_calc(x_len, y_len, z_len):
    """Calculates elbow servo angle"""
    hyp_top = hyp_top_calc(x_len, y_len)
    hyp_side = hyp_side_calc(hyp_top, z_len)
    j3 = acos((pow(LEN_U_L,2)+pow(LEN_L_L,2)-pow(hyp_side,2))/(2*LEN_U_L*LEN_L_L))
    j3 = (j3*180)/pi
    #Double check this
    j3 = j3 - 90 + ANG_C
    return j3

def hyp_top_calc(x_len, y_len):
    """Calculates top hypotenuse"""
    return sqrt((pow(x_len,2)+ pow(y_len,2)))

def hyp_side_calc(hyp_top, z_len):
    """Calculates side hypotenuse"""
    return sqrt((pow(hyp_top,2)+ pow(z_len,2)))

servo_calc(0,50, 70)
