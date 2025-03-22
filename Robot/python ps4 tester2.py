
import pygame

# Initialize pygame
pygame.init()
pygame.joystick.init()

# Check for controllers
joystick_count = pygame.joystick.get_count()
if joystick_count == 0:
    print("No controllers found")
    exit()

# Initialize the first controller
joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"Controller detected: {joystick.get_name()}")

# Main loop to read data
try:
    while True:
        pygame.event.pump()
        
        # Get axis values (analog sticks, triggers)
        axes = [joystick.get_axis(i) for i in range(joystick.get_numaxes())]
        print(f"Axes: {axes}")
        
        # Get button values
        buttons = [joystick.get_button(i) for i in range(joystick.get_numbuttons())]
        print(f"Buttons: {buttons}")
        
        # Small delay
        pygame.time.wait(100)
except KeyboardInterrupt:
    print("Stopped by user")
    pygame.quit()

