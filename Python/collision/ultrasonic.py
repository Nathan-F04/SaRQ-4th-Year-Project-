from gpiozero import DistanceSensor
from time import sleep

sensor = DistanceSensor(23,24)

while True:
    print(f"Distance from sensor: {sensor.distance} m")
    time.sleep(0.5)