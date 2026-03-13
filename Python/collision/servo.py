import pigpio
from time import sleep

# connect to the 
pi = pigpio.pi()

# loop forever
while True:

    pi.set_servo_pulsewidth(18, 500)    # off
    sleep(1)
    pi.set_servo_pulsewidth(18, 1500)    # off
    sleep(1)
    pi.set_servo_pulsewidth(18, 2500)    # off
    sleep(1)