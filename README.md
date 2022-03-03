This repo is used for functioning robot covid's automated navigation.
Steps to use this repo:
1. Install ROS basic
2. Install move_base, robot_localization, robot_state_publisher and amcl
sudo apt install ros-*your distro*-move-base ros-*your distro*-amcl ros-*your distro*-robot-state-publisher
change '*your-distro*' to your preferred distro. However, this package only tested in ROS noetic distro
3. Create new workspace
4. create a folder in your src directory on your workspace
5. clone this repo by
git clone samiframadhan/rovit_navsys
6. Compile the program by initiating catkin_make in your workspace directory
7. It is ready to be used!

I have not yet including how to route the circuits, maybe soon i'll create how to build the robot from scratch.
