import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class MinimalPublisher(Node):
    def __init__(self, name: str, topic: str):
        super().__init__(name)
        self.publisher_ = self.create_publisher(String, topic, 10)
        timer_period = 0.5  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0
        self.name = name

    def timer_callback(self):
        msg = String()
        msg.data = f"Hello World from {self.name}: {self.i}"
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: "{msg.data}"')
        self.i += 1

def spin_node(node):
    rclpy.spin(node)
    node.destroy_node()

def main(args=None):
    rclpy.init(args=args)

    # Create two publisher nodes
    minimal_publisher = MinimalPublisher('Controller', 'topic')
    second_publisher = MinimalPublisher('ESP32', 'topic')

    rclpy.spin(minimal_publisher)
    rclpy.spin(second_publisher)

    rclpy.shutdown()

if __name__ == '__main__':
    main()
