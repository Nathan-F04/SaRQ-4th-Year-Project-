from ultralytics import YOLO

# Load a YOLO PyTorch model
model = YOLO("yolov8n.pt")

# Export the model to NCNN format
model.export(format="ncnn", imgsz=160)  # creates 'yolox_ncnn_model'