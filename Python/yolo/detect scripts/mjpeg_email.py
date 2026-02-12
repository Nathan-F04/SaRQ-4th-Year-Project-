import io
import logging
import socketserver
from http import server
from threading import Condition
import time
import numpy as np
import cv2
from ultralytics import YOLO
from picamera2 import Picamera2
from picamera2.encoders import JpegEncoder
from picamera2.outputs import FileOutput
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email import encoders

#defining send email before it's called
def send_email():
    # create message object instance
    msg = MIMEMultipart()

    # set the sender and recipient email addresses, and the password
    sender = ''
    recipient = ''
    password =""

    # set the subject and body of the email
    msg['Subject'] = 'Test Email with Attachment'
    msg['From'] = sender
    msg['To'] = recipient

    body = 'This is a test email with attachment.'

    # attach the body of the email to the message object
    msg.attach(MIMEText(body, 'plain'))

    count = 10
    while(count>0):
        # open the file in binary
        filename = f"example{count}.jpg"
        attachment = open(f"/home/ferry/SaRQ-4th-Year-Project-/Python/yolo/detect scripts/img/example{count}.jpg", "rb")

        # create a MIMEBase object and set its attributes
        file = MIMEBase('application', 'octet-stream')
        file.set_payload((attachment).read())
        encoders.encode_base64(file)
        file.add_header('Content-Disposition', "attachment; filename= %s" % filename)

        # attach the MIMEBase object to the message object
        msg.attach(file)
        count-=1

    # create a SMTP session
    server = smtplib.SMTP('smtp.gmail.com', 587)

    # start TLS for security
    server.starttls()

    # authenticate with the email account
    server.login(sender, password)

    # send the email
    server.sendmail(sender, recipient, msg.as_string())
    print("Email sent")

    # terminate the SMTP session
    server.quit()

#Constants
MODEL_PATH = "yolo11n_ncnn_model" 
OBJECTS_TO_DETECT = [0]    # List of class IDs to monitor (e.g., 0 = person)

PAGE = """\
<html>
<head>
<title>Picamera2 YOLO MJPEG Stream</title>
</head>
<body>
<h1>YOLO Object Detection Stream</h1>
<img src="stream.mjpg" width="640" height="480" />
</body>
</html>
"""

#Camera streaming output
class StreamingOutput(io.BufferedIOBase):
    def __init__(self):
        self.frame = None
        self.condition = Condition()

    def write(self, buf):
        with self.condition:
            self.frame = buf
            self.condition.notify_all()

#HTTP request handler
class StreamingHandler(server.BaseHTTPRequestHandler):
    flag = 0
    def do_GET(self):
        if self.path == '/':
            self.send_response(301)
            self.send_header('Location', '/index.html')
            self.end_headers()

        elif self.path == '/index.html':
            content = PAGE.encode('utf-8')
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Content-Length', len(content))
            self.end_headers()
            self.wfile.write(content)

        elif self.path == '/stream.mjpg':
            self.send_response(200)
            self.send_header('Age', 0)
            self.send_header('Cache-Control', 'no-cache, private')
            self.send_header('Pragma', 'no-cache')
            self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
            self.end_headers()

            try:
                while True:
                    # Wait for a new frame from the camera
                    with output.condition:
                        output.condition.wait()
                        frame = output.frame  # raw JPEG bytes

                    # Decode JPEG to OpenCV image
                    img_array = np.frombuffer(frame, dtype=np.uint8)
                    image = cv2.imdecode(img_array, cv2.IMREAD_COLOR)

                    # Run YOLO detection
                    results = model(image, imgsz=320, conf=0.8)
                    # Flag if any objects of interest are detected
                    detected_objects = results[0].boxes.cls.tolist()
                    object_found = False
                    for obj_id in OBJECTS_TO_DETECT:
                        if obj_id in detected_objects:
                            object_found = True
                            print(f"Detected object with ID {obj_id}!")

                    # Draw bounding boxes on frame
                    annotated_frame = results[0].plot()

                    # Overlay FPS info
                    inference_time = results[0].speed['inference'] 
                    fps = 1000 / inference_time if inference_time > 0 else 0
                    text = f'FPS: {fps:.1f}'
                    font = cv2.FONT_HERSHEY_SIMPLEX
                    text_size = cv2.getTextSize(text, font, 1, 2)[0]
                    text_x = annotated_frame.shape[1] - text_size[0] - 10
                    text_y = text_size[1] + 10
                    cv2.putText(annotated_frame, text, (text_x, text_y), font, 1, (255, 255, 255), 2, cv2.LINE_AA)

                    # Control the Pin based on detection
                    if object_found:
                        print("Pin turned on!") #remove in final draft -testing purposes only
                        if StreamingHandler.flag>= 0:
                            StreamingHandler.flag+=1
                            filename = f"/home/ferry/SaRQ-4th-Year-Project-/Python/yolo/detect scripts/img/example{StreamingHandler.flag}.jpg"
                            cv2.imwrite(filename, annotated_frame)

                        #Logic to send email, will implement means to restart ability to email but this stops it for now
                        if StreamingHandler.flag == 10:
                            send_email()
                            StreamingHandler.flag = -1

                    # Encode frame back to JPEG for streaming
                    ret, jpeg = cv2.imencode('.jpg', annotated_frame)
                    if not ret:
                        continue
                    annotated_bytes = jpeg.tobytes()

                    # Send MJPEG frame to client
                    self.wfile.write(b'--FRAME\r\n')
                    self.send_header('Content-Type', 'image/jpeg')
                    self.send_header('Content-Length', len(annotated_bytes))
                    self.end_headers()
                    self.wfile.write(annotated_bytes)
                    self.wfile.write(b'\r\n')

            except Exception as e:
                logging.warning('Removed streaming client %s: %s', self.client_address, str(e))

        else:
            self.send_error(404)
            self.end_headers()


class StreamingServer(socketserver.ThreadingMixIn, server.HTTPServer):
    allow_reuse_address = True
    daemon_threads = True


#Initialise model
model = YOLO(MODEL_PATH)

#Initialise camera
picam2 = Picamera2()
picam2.configure(picam2.create_video_configuration(main={"size": (1280, 1280)}))
output = StreamingOutput()
picam2.start_recording(JpegEncoder(), FileOutput(output))

#Start server
try:
    address = ('', 7123)
    server = StreamingServer(address, StreamingHandler)
    print("Server started")
    server.serve_forever()
finally:
    picam2.stop_recording()
