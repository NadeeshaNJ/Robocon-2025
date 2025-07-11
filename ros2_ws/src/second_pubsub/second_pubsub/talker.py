#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import tty
import sys
import termios
import select

class Controller(Node):
    def __init__(self):
        super().__init__('Controller')
        # Create publishers for both topics
        self.moving_publisher = self.create_publisher(String, 'robot/moving', 10)
        self.lift_publisher = self.create_publisher(String, 'robot/lift', 10)
        
        # Set up terminal for key press detection
        self.old_attr = termios.tcgetattr(sys.stdin)
        tty.setcbreak(sys.stdin.fileno())
        
        self.get_logger().info('Controller node started. Press WASD for movement, QE for lift, or X to exit')
        
    def read_key(self):
        if select.select([sys.stdin], [], [], 0)[0]:
            return sys.stdin.read(1)
        return None
    
    def process_keys(self):
        key = self.read_key()
        if key:
            key = key.lower()
            if key == 'x':
                return False  # Exit signal
            
            # Movement controls (WASD)
            if key in ['w', 'a', 's', 'd']:
                msg = String()
                if key == 'w':  # Forward
                    msg.data = '255 255 255 255'  # All motors forward
                elif key == 's':  # Backward
                    msg.data = '-255 -255 -255 -255'  # All motors backward
                elif key == 'a':  # Left
                    msg.data = '255 -255 -255 255'  # Left motors backward, right motors forward
                elif key == 'd':  # Right
                    msg.data = '-255 255 255 -255'  # Left motors forward, right motors backward
                
                self.moving_publisher.publish(msg)
                self.get_logger().info(f'Published to robot/moving: "{msg.data}"')
            
            # Lift controls (QE)
            elif key in ['q', 'e']:
                msg = String()
                if key == 'q':  # Lift up
                    msg.data = 'UP'
                elif key == 'e':  # Lift down
                    msg.data = 'DOWN'
                
                self.lift_publisher.publish(msg)
                self.get_logger().info(f'Published to robot/lift: "{msg.data}"')
        
        return True

    def cleanup(self):
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, self.old_attr)

def main(args=None):
    rclpy.init(args=args)
    controller = Controller()
    
    try:
        while rclpy.ok():
            rclpy.spin_once(controller, timeout_sec=0.1)
            if not controller.process_keys():
                break
    except KeyboardInterrupt:
        pass
    finally:
        controller.cleanup()
        controller.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
