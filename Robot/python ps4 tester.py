import pygame

# Initialize pygame joystick module
pygame.init()
pygame.joystick.init()

# Check if a joystick (controller) is connected
if pygame.joystick.get_count() == 0:
    print("No controllers detected!")
    exit()

# Connect to the first controller
joystick = pygame.joystick.Joystick(0)
joystick.init()

print(f"Connected to: {joystick.get_name()}")
print("Press L3 or R3 to test...")

running = True
while running:
    pygame.event.pump()  # Process events

    # Check button presses
    if joystick.get_button(11):  # L3 Button
        print("L3 Button Pressed!")
    if joystick.get_button(12):  # R3 Button
        print("R3 Button Pressed!")

    # Quit on pressing the PS button (Optional: Change to a different button)
