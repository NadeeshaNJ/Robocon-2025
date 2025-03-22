import pygame
import serial
import time

# Initialize pygame
pygame.init()

# Check for connected joysticks
if pygame.joystick.get_count() == 0:
    print("No joystick detected. Please connect a PS4 controller.")
    exit()

# Initialize the first joystick
joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"Joystick detected: {joystick.get_name()}")

# Initialize serial connection to HC-05
try:
    arduino = serial.Serial('COM11', 9600)  # Replace 'COM11' with your HC-05's port
    time.sleep(2)  # Wait for the connection to establish
    print("HC-05 connected successfully.")
except serial.SerialException as e:
    print(f"Failed to connect to HC-05: {e}")
    exit()

def send_command(command):
    arduino.write(command.encode())
    print(f"Sent command: {command}")

try:
    while True:
        pygame.event.pump()  # Process event queue

        # Check if L3 is pressed (left stick button)
        if joystick.get_button(11):  # L3 button
            send_command('L')  # Send command to rotate anti-clockwise
            print("L3 pressed: Rotating anti-clockwise")

        # Check if R3 is pressed (right stick button)
        elif joystick.get_button(12):  # R3 button
            send_command('R')  # Send command to rotate clockwise
            print("R3 pressed: Rotating clockwise")

        # Add a small delay to avoid spamming commands
        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting program")

finally:
    arduino.close()
    pygame.quit()