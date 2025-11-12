import serial
import time

# Configure serial port
ser = serial.Serial(
    port='/dev/serial0',  # Default UART port on GPIO 14/15
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

try:
    while True:
        # Send message
        ser.write(b"1\r\n")
        print("Sent: 1")

        # Receive response (if STM32 replies)
        if ser.in_waiting > 0:
            received = ser.readline().decode('utf-8').strip()
            print(f"Received: {received}")

        time.sleep(1)

except KeyboardInterrupt:
    ser.close()
    print("Serial port closed.")