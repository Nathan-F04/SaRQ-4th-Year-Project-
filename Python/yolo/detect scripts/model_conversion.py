from ultralytics import YOLO

# Load a YOLO PyTorch model
model = YOLO("yolo11n.pt")

# Export the model to NCNN format
model.export(format="ncnn", imgsz=320)  # creates 'yolox_ncnn_model'