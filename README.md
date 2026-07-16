# SaRQ-4th-Year-Project-

The goal of the Search & Rescue Quadruped (SaRQ) was to design, program and assemble a
quadruped that could self-navigate in the aftermath of disaster scenarios such as after an earthquake
had hit a city. The SaRQ would be required to walk, detect humans and alert search and rescue
personnel. 

Components:

Raspberry Pi 4 Model B (RPI): The brain of the SaRQ, making decisions on movement and connecting
to other devices to read data whilst running an MJPEG or simple mail transfer protocol server.

STM32IOT1A : Connected to the RPI over UART (universal synchronous asynchronous protocol),
programming 12 MG90S servo motors over pulse-width modulation (PWM) for movement.

MG90S: The servo motors programmed by the STM32IOT1A via PWM.

HC-SR04: The ultrasonic sensor used to read distance to obstacles ahead. Connected to the RPI for
collision detection.

AD002: The servo motor connected to the HC-SR04 to rotate it.

Pi Camera V3 : The camera running the you only look once (YOLO) real-time object detection
algorithm to identify humans.

Methodologies:

Agile methodologies were used to manage this project with Jira for sprint planning and keeping an
up-to-date project backlog.

Software tools:
The RPI was programmed via Python in the visual studio code integrated development environment.
The STM32IOT1A was programmed via C in the STM32CUBEIDE.
Onshape was used to draw and design all 3D printed components such as the legs, chassis and a
holder for the HC-SR04.

Project overview:

The brain of the SaRQ is the Raspberry Pi 4 model B (RPI). This chooses which direction the SaRQ would
move as it is connected to a HC-SR04 ultrasonic sensor. This sensor takes periodic readings ahead and
the RPI tells the STM32IOT1A over UART how to program the servo motors. If the RPI sends a 1 it would
be to take a step forward, a 2 would be a step backwards etc. The RPI sends a command to move
forward unless the HC-SR04 reads an obstacle less than 15 centimetres ahead. In this case, the servo
motor attached to the HC-SR04 would rotate left and right, allowing the ultrasonic sensor to take
readings before the RPI would choose the direction to turn to avoid a collision. 

The Pi Camera V3
implements YOLOv11 for real-time person detection, using the algorithm to detect humans. A motion
JPEG or MJPEG server hosted on the RPI streams the Pi Camera’s live feed but to avoid search and
rescue personnel having to stay looking at the feed an alert system is used. This system is in the form of
a simple-mail transfer protocol (SMTP) server which sends an email to search and rescue personnel. If a
human is detected 10 times, an email is sent with all 10 images as attachments. This allows search and
rescue operatives to be proactive and help others, aiding persons the SaRQ detects once they are freely
available to do so. 

The animation for walking was created with an inverse kinematics script that uses
trigonometric calculations to map movement in a co-ordinate plane. This was used as a tool to create
animations. Due to the number of wires with the servo motors the SaRQ has and a potential divider
circuit being necessary for the HC-SR04, a stripboard was designed and used to add the circuit and aid
in wire management. The SaRQ was assembled using a series of screws that were tapped for added grip
with a mount at the front of the chassis for the Pi camera and AD002 with HC-SR04 to attach to.
