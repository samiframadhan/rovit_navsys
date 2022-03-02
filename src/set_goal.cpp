/*
 * Author: Automatic Addison
 * Date: May 30, 2021
 * ROS Version: ROS 1 - Melodic
 * Website: https://automaticaddison.com
 * This ROS node sends the robot goals to move to a particular location on 
 * a map. I have configured this program to the map of my own apartment.
 *
 * 1 = Point 1
 * 2 = Point 1 Rotated
 * 3 = Point 2
 * 4 = Point 2 Rotated
 * 5 = Point 3
 * 6 = Point 3 Rotated
 * 7 = Home
 * 8 = Home rotated
 */
 
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <iostream>
 
using namespace std;
 
// Action specification for move_base
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
 
int main(int argc, char** argv){
   
  // Connect to ROS
  ros::init(argc, argv, "simple_navigation_goals");
 
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);
 
  // Wait for the action server to come up so that we can begin processing goals.
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
 
  int user_choice = 6;
  char choice_to_continue = 'Y';
  bool run = true;
  char sequence_mode = 'Y';
  bool run_sequence = true;

     
  while(run) {
 
    // Ask the user where he wants the robot to go?
    cout << "\nWhere do you want the robot to go?" << endl;
    cout << "\n1 = Point1" << endl;
    cout << "2 = Point1Rotate" << endl;
    cout << "3 = Point2" << endl;
    cout << "4 = Point2Rotate" << endl;
    cout << "5 = Point3" << endl;
    cout << "6 = Point3Rotate" << endl;
    cout << "7 = Home" << endl;
    cout << "8 = HomeRotate" << endl;
    cout << "\nEnter a number: ";
    cin >> user_choice;
 
    // Create a new goal to send to move_base 
    move_base_msgs::MoveBaseGoal goal;
    move_base_msgs::MoveBaseResult result_cb;
    
 
    // Send a goal to the robot
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
         
    bool valid_selection = true;
 
    // Use map_server to load the map of the environment on the /map topic. 
    // Launch RViz and click the Publish Point button in RViz to 
    // display the coordinates to the /clicked_point topic.
    switch (user_choice) {
      case 1:
        cout << "\nGoal Location: Point 1\n" << endl;
        goal.target_pose.pose.position.x = -6.423;
        goal.target_pose.pose.position.y = -0.70519;
        goal.target_pose.pose.orientation.w = 0.72402;
        goal.target_pose.pose.orientation.z = 0.6897;         
      break;
      case 2:
        cout << "\nGoal Location: RotatedP1\n" << endl;
        goal.target_pose.pose.position.x = -1.112;
        goal.target_pose.pose.position.y = 2.5274;
        goal.target_pose.pose.orientation.w = 0.74751;
        goal.target_pose.pose.orientation.z = -0.66626;
        break;
      case 3:
        cout << "\nGoal Location: Point 2\n" << endl;
        goal.target_pose.pose.position.x = 7.587;
        goal.target_pose.pose.position.y = -3.681;
        goal.target_pose.pose.orientation.w = 0.899;
        goal.target_pose.pose.orientation.z = 0.438;
        break;
      case 4:
        cout << "\nGoal Location: RotatedP2\n" << endl;
        goal.target_pose.pose.position.x = 7.590;
        goal.target_pose.pose.position.y = -3.623;
        goal.target_pose.pose.orientation.w = 0.549;
        goal.target_pose.pose.orientation.z = -0.836;
        break;
      case 5:
        cout << "\nGoal Location: Point 3\n" << endl;
        goal.target_pose.pose.position.x = 0.453;
        goal.target_pose.pose.position.y = -3.708;
        goal.target_pose.pose.orientation.w = 0.436;
        goal.target_pose.pose.orientation.z = -0.9;
        break;
      case 6:
        cout << "\nGoal Location: RotatedP3\n" << endl;
        goal.target_pose.pose.position.x = 0.441;
        goal.target_pose.pose.position.y = -3.730;
        goal.target_pose.pose.orientation.w = 0.756;
        goal.target_pose.pose.orientation.z = 0.655;
        break;
        case 7:
        cout << "\nGoal Location: Home\n" << endl;
        goal.target_pose.pose.position.x = -0.329279;
        goal.target_pose.pose.position.y = 0.0305;
        goal.target_pose.pose.orientation.w = -0.9;
        break;
        case 8:
        cout << "\nGoal Location: RotatedHome\n" << endl;
        goal.target_pose.pose.position.x = -0.216;
        goal.target_pose.pose.position.y = 0.0321;
        goal.target_pose.pose.orientation.z = -0.346;
        goal.target_pose.pose.orientation.w = 0.938;
        break;
      default:
        cout << "\nInvalid selection. Please try again.\n" << endl;
        valid_selection = false;
    }       
         
    // Go back to beginning if the selection is invalid.
    if(!valid_selection) {
      continue;
    }
 
    ROS_INFO("Sending goal");
    ac.sendGoal(goal);
 
    // Wait until the robot reaches the goal
    ac.waitForResult();
 
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("The robot has arrived at the goal location");
    else
      ROS_INFO("The robot failed to reach the goal location for some reason");
         
    // Ask the user if he wants to continue giving goals
    do {
      cout << "\nWould you like to go to another destination? (Y/N)" << endl;
      cin >> choice_to_continue;
      choice_to_continue = tolower(choice_to_continue); // Put your letter to its lower case
    } while (choice_to_continue != 'n' && choice_to_continue != 'y'); 
 
    if(choice_to_continue =='n') {
        run = false;
    }  
  }
   
  return 0;
}
