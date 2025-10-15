from math import acos,atan, pow, sqrt, pi

#Globals
#Length upper and lower here
LENGTH_UPPER_LEG = 10
LENGTH_LOWER_LEG = 20
STARTING_X = 0
STARTING_Y = 0
STARTING_Z = 0
ANGLE_C = 30
#Beginning of inverse kinematics calculations
#Check if I need to change to degrees instead of radians
def ServoCalculations(X_Coord, Y_Coord, Z_Coord):
    S1 = J1Calculations(X_Coord+STARTING_X, Y_Coord+STARTING_Y)
    S2 = J2Calculations(X_Coord+STARTING_X, Y_Coord+STARTING_Y, Z_Coord+STARTING_Z)
    S3 = J3Calculations(X_Coord+STARTING_X, Y_Coord+STARTING_Y, Z_Coord+STARTING_Z)

    print(S1)
    print(S2)
    print(S3)
    #Pyserial sent below

def J1Calculations(X_Length, Y_Length):
    if(X_Length == 0):
        J1 = 0
        return J1
    else:
        J1 = atan(X_Length/Y_Length)
        J1 = (J1*180)/pi
        return J1

def J2Calculations(X_Length,Y_Length,Z_Length):
    HypotenuseTop = HypotenuseTopCalculation(X_Length, Y_Length)
    HypotenuseSide = HypotenuseSideCalculation(HypotenuseTop, Z_Length)
    B = acos((pow(HypotenuseSide,2)+pow(LENGTH_UPPER_LEG,2)-pow(LENGTH_LOWER_LEG,2))/(2*HypotenuseSide*LENGTH_UPPER_LEG)) 
    A = atan(Z_Length/HypotenuseTop)

    #Addition since A is negative - double check

    J2 = B + A 
    J2 = (J2*180)/pi
    return J2

def J3Calculations(X_Length, Y_Length, Z_Length):
    HypotenuseTop = HypotenuseTopCalculation(X_Length, Y_Length)
    HypotenuseSide = HypotenuseSideCalculation(HypotenuseTop, Z_Length)
    J3 = acos((pow(LENGTH_UPPER_LEG,2)+pow(LENGTH_LOWER_LEG,2)-pow(HypotenuseSide,2))/(2*LENGTH_UPPER_LEG*LENGTH_LOWER_LEG)) + ANGLE_C
    J3 = (J3*180)/pi
    #Double check this
    J3 = J3 - 90
    return J3

def HypotenuseTopCalculation(X_Length, Y_Length):
    return sqrt((pow(X_Length,2)+ pow(Y_Length,2)))

def HypotenuseSideCalculation(HypotenuseTop, Z_Length):
    return sqrt((pow(HypotenuseTop,2)+ pow(Z_Length,2)))

ServoCalculations(10,20, -20)



