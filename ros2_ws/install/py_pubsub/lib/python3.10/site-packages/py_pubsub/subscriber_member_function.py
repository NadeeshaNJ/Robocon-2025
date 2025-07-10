import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class MinimalSubscriber(Node):
    def __init__(self, name: str, topic: str):
        super().__init__(name)
        self.subscription = self.create_subscription(
            String, topic, self.listener_callback, 10)
        self.subscription  # prevent unused variable warning
        self.name = name

    def listener_callback(self, msg):
        self.get_logger().info(f"I heard {self.name}: {msg.data}")

def spin_node(node):
    rclpy.spin(node)
    node.destroy_node()

def main(args=None):
    rclpy.init(args=args)

    # Create the subscribers
    minimal_subscriber = MinimalSubscriber('MovingWheels_LEFT', 'topic')
    second_subscriber = MinimalSubscriber('MovingWheels_RIGHT', 'topic')

    rclpy.spin(minimal_subscriber)
    rclpy.spin(second_subscriber)

    rclpy.shutdown()

if __name__ == '__main__':
    main()
