import cv2
from picamera2 import Picamera2
from ultralytics import YOLO

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
    # Capture a frame from the camera
    frame = picam2.capture_array()

    # Run object detection on the frame
    results = model(frame, imgsz = 160)

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
    else:
        print("Pi turned off!")
            
    # Display the frame with detection results
    annotated_frame = results[0].plot()
    cv2.imshow("Object Detection", annotated_frame)

    # Break the loop if 'q' is pressed
    if cv2.waitKey(1) == ord("q"):
        break

# Clean up
cv2.destroyAllWindows()