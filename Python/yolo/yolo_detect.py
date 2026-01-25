import cv2
from picamera2 import Picamera2
from ultralytics import YOLO
import time

# Initialize the camera
picam2 = Picamera2()
picam2.preview_configuration.main.size = (640, 640)
picam2.preview_configuration.main.format = "RGB888"
picam2.preview_configuration.align()
picam2.configure("preview")
picam2.start()

# Load the YOLO model
model = YOLO("yolo11n_ncnn_model")

# List of class IDs we want to detect
objects_to_detect = [0]  # You can modify this list

while True:
    filename = f"Python/yolo/img/frame_{int(time.time()*1000)}.jpg"
    
    # Capture a frame from the camera
    frame = picam2.capture_array()

    # Run object detection on the frame
    results = model(frame, imgsz = 320)

    # Get the classes of detected objects
    detected_objects = results[0].boxes.cls.tolist()
    
    # Check if any of our specified objects are detected
    object_found = False
    for obj_id in objects_to_detect:
        if obj_id in detected_objects:
            object_found = True
            print(f"Detected object with ID {obj_id}!")
    
    # Control the Pin based on detection
    if object_found:
        print("Pin turned on!")
        cv2.imwrite(filename, frame)
    else:
        print("Pi turned off!")
            
    # Output the visual detection data, we will draw this on our camera preview window
    annotated_frame = results[0].plot()
    # Get inference time
    inference_time = results[0].speed['inference']
    fps = 1000 / inference_time  # Convert to milliseconds
    text = f'FPS: {fps:.1f}'

    # Define font and position
    font = cv2.FONT_HERSHEY_SIMPLEX
    text_size = cv2.getTextSize(text, font, 1, 2)[0]
    text_x = annotated_frame.shape[1] - text_size[0] - 10  # 10 pixels from the right
    text_y = text_size[1] + 10  # 10 pixels from the top

    # Draw the text on the annotated frame
    cv2.putText(annotated_frame, text, (text_x, text_y), font, 1, (255, 255, 255), 2, cv2.LINE_AA)

    cv2.imshow("Object Detection", annotated_frame)

    # Break the loop if 'q' is pressed
    if cv2.waitKey(1) == ord("q"):
        break

# Clean up
cv2.destroyAllWindows()
