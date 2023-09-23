#!/usr/bin/env python

import rospy
from nav_msgs.msg import OccupancyGrid
from nav_msgs.msg import Path
from geometry_msgs.msg import PoseStamped
from nav_msgs.srv import GetPlan
from scipy.spatial import Voronoi
import numpy as np

class VoronoiGlobalPlanner:
    def __init__(self):
        rospy.init_node('voronoi_global_planner')

        # Parameters
        self.map_topic = rospy.get_param("~map_topic", "/map")
        self.start_topic = rospy.get_param("~start_topic", "/start")
        self.goal_topic = rospy.get_param("~goal_topic", "/goal")
        self.voronoi_topic = rospy.get_param("~voronoi_topic", "/voronoi")
        self.path_topic = rospy.get_param("~path_topic", "/path")
        self.robot_radius = rospy.get_param("~robot_radius", 0.2)  # Adjust as per your robot

        # Subscribers
        self.map_sub = rospy.Subscriber(self.map_topic, OccupancyGrid, self.map_callback)
        self.start_sub = rospy.Subscriber(self.start_topic, PoseStamped, self.start_callback)
        self.goal_sub = rospy.Subscriber(self.goal_topic, PoseStamped, self.goal_callback)

        # Publishers
        self.voronoi_pub = rospy.Publisher(self.voronoi_topic, OccupancyGrid, queue_size=1)
        self.path_pub = rospy.Publisher(self.path_topic, Path, queue_size=1)

        self.map = None
        self.start = None
        self.goal = None

    def map_callback(self, data):
        self.map = data

    def start_callback(self, data):
        self.start = data

    def goal_callback(self, data):
        self.goal = data

    def run(self):
        while not rospy.is_shutdown():
            if self.map is not None and self.start is not None and self.goal is not None:
                voronoi_map = self.calculate_voronoi()
                self.publish_voronoi(voronoi_map)
                path = self.plan_path(voronoi_map)
                self.publish_path(path)
            rospy.sleep(1)

    def calculate_voronoi(self):
        # Convert occupancy grid data to numpy array
        map_data = np.array(self.map.data).reshape((self.map.info.height, self.map.info.width))

        # Create a mask for obstacles (assuming 0 is for free space)
        obstacle_mask = map_data != 0

        # Create points for Voronoi diagram
        points = []
        for y in range(self.map.info.height):
            for x in range(self.map.info.width):
                if not obstacle_mask[y, x]:
                    points.append([x * self.map.info.resolution, y * self.map.info.resolution])

        if len(points) < 3:
            rospy.logwarn("Not enough points for Voronoi calculation.")
            return None

        vor = Voronoi(points)

        # Create a new occupancy grid based on Voronoi diagram
        voronoi_map = OccupancyGrid()
        voronoi_map.header = self.map.header
        voronoi_map.info = self.map.info
        voronoi_map.data = [0] * len(self.map.data)

        for y in range(self.map.info.height):
            for x in range(self.map.info.width):
                if not obstacle_mask[y, x]:
                    voronoi_map.data[y * self.map.info.width + x] = 0  # Free space
                else:
                    voronoi_map.data[y * self.map.info.width + x] = 100  # Obstacle

        return voronoi_map

    def plan_path(self, voronoi_map):
        # You can use the ROS service GetPlan to find a path from start to goal using the Voronoi map
        rospy.wait_for_service('/move_base/GlobalPlanner/make_plan')
        try:
            get_plan = rospy.ServiceProxy('/move_base/GlobalPlanner/make_plan', GetPlan)
            start_pose = PoseStamped()
            start_pose.header.frame_id = self.map.header.frame_id
            start_pose.pose = self.start.pose

            goal_pose = PoseStamped()
            goal_pose.header.frame_id = self.map.header.frame_id
            goal_pose.pose = self.goal.pose

            resp = get_plan(start=start_pose, goal=goal_pose, tolerance=self.robot_radius)

            return resp.plan

        except rospy.ServiceException as e:
            rospy.logerr("Service call failed: %s" % e)
            return None

    def publish_voronoi(self, voronoi_map):
        if voronoi_map is not None:
            self.voronoi_pub.publish(voronoi_map)

    def publish_path(self, path):
        if path is not None:
            self.path_pub.publish(path)

if __name__ == '__main__':
    try:
        planner = VoronoiGlobalPlanner()
        planner.run()
    except rospy.ROSInterruptException:
        pass
