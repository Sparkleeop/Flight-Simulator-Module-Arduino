import serial
import pyvjoy
import time

SERIAL_PORT = "COM7"   # change if needed
BAUD = 115200

ser = serial.Serial(SERIAL_PORT, BAUD, timeout=1)
time.sleep(2)

joy = pyvjoy.VJoyDevice(1)

def map_axis(val):
    return int(val * 32767 / 1023)

while True:
    try:
        line = ser.readline().decode().strip()
        if not line:
            continue

        parts = line.split(",")
        if len(parts) != 8:
            continue   # IMPORTANT

        roll, pitch, throttle, rudder, brake, gear, flap_dn, flap_up = map(int, parts)

        # Axes
        joy.set_axis(pyvjoy.HID_USAGE_X,  map_axis(roll))
        joy.set_axis(pyvjoy.HID_USAGE_Y,  map_axis(pitch))
        joy.set_axis(pyvjoy.HID_USAGE_Z,  map_axis(throttle))
        joy.set_axis(pyvjoy.HID_USAGE_RZ, map_axis(rudder))

        # Buttons (pulse)
        if brake:
            joy.set_button(1, 1)
            time.sleep(0.05)
            joy.set_button(1, 0)

        if gear:
            joy.set_button(2, 1)
            time.sleep(0.05)
            joy.set_button(2, 0)

        if flap_dn:
            joy.set_button(3, 1)
            time.sleep(0.05)
            joy.set_button(3, 0)

        if flap_up:
            joy.set_button(4, 1)
            time.sleep(0.05)
            joy.set_button(4, 0)

    except Exception as e:
        print("Error:", e)
