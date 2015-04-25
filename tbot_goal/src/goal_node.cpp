#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PointStamped.h>
#include <math.h>
#include <iostream>
#include "gpr_tbot.h"
#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>
#include <string>
#include <time.h>


// Write constructors and destructors for better practice

/* Entire idea is to publish two points on rviz by clicking on the map. Then fetch those points by call back function 'callback'.
 * Once these two points are obtained, tp_calculation is used to calculate total no of path points in the area covered in the
 * rectangle to which points x1,y1 and x2,y2 are diagonally opposite points and sides are parallel to principal axes. In
 * grid_calculation actual grid points are calculated and stored in vector X and Y. fs is step taken in y direction and ss is
 * step taken in x direction. units are in meters. Last_fs and last_ss are the last steps in Y and X directions respectively.
 *
 *
 *
 * */




using namespace std;
using namespace ros;



typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;



int main(int argc, char** argv){
  ros::init(argc, argv, "goal_node");



  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);
  MoveBaseClient *ac_ptr = &ac;
  NodeHandle n;
  gpr_tbot gt;
  gpr_tbot *gt_address;
  gt_address = &gt;


  move_base_msgs::MoveBaseGoal goal;
  move_base_msgs::MoveBaseGoal *goal_ptr = &goal;

  Subscriber sub = n.subscribe("/clicked_point", 1000, &gpr_tbot::callback, gt_address);
  Publisher grid_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 2000);

  ROS_INFO("Select two points");

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  //visualization_msgs::MarkerArray grid_marker_array;
  visualization_msgs::Marker grid_marker;






  gt.fs = 0.5;  //Can be edited to get a finer grid
  gt.ss = 0.5;

  Rate loop_rate(10);


  //we'll send a goal to the robot to move 1 meter forward
  //goal.target_pose.header.frame_id = "base_link";
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();



  while (ros::ok())
  {

      if(gt.q == 2)  // This part is run only after callback function has gotten called twice,
                     // each call incrementing value of q by 1
      {

          gt.x1 = gt.pt_X[0];   // callback function stores two points in pt_X and pt_Y.
          gt.x2 = gt.pt_X[1];
          gt.y1 = gt.pt_Y[0];
          gt.y2 = gt.pt_Y[1];

          ROS_INFO("Points assigned to x1 x2 y1 y2 is %f, %f, %f, %f ", gt.x1, gt.x2, gt.y1, gt.y2);

          gt.tp_calculation();
          ROS_INFO("Done with tp calculation");

          gt.grid_calculation();
          ROS_INFO("Done with grid calculation");
          //char a = 'A';

          //grid_marker_array.markers.resize(500);
          for(int k = 0; k<gt.tp; k++)
          {
              grid_marker.header.frame_id = "map";
              grid_marker.header.stamp = ros::Time();

              //stringstream ss;
              //ss << k;
              //string str = ss.str();
              //string *str_add;
              //str_add = &str;
              //ROS_INFO("%i",k);





              grid_marker.ns = "goal_node"; //+ a;
              grid_marker.id = k;
              grid_marker.type = visualization_msgs::Marker::CUBE;
              grid_marker.action = visualization_msgs::Marker::ADD;
              grid_marker.scale.x = 0.15;
              grid_marker.scale.y = 0.15;
              grid_marker.scale.z = 0.10;
              grid_marker.color.r = 0.0;
              grid_marker.color.g = 1.0;
              grid_marker.color.b = 0.0;
              grid_marker.color.a = 1.0;
              grid_marker.lifetime = ros::Duration();
              grid_marker.pose.orientation.w = 1.0;
              grid_marker.pose.position.x = gt.X[k];//gt.X.at(k);
              grid_marker.pose.position.y = gt.Y[k];//gt.Y.at(k);


              //grid_marker_array.markers.push_back(grid_marker);

              grid_pub.publish(grid_marker);
              usleep(2000);


              //ROS_INFO("Marker %i", k);
              //a++;

          }
          //grid_pub.publish(grid_marker_array);



          gt.grid_trace(goal, ac_ptr);

          gt.q ++;              // So that this part only gets called once


       }




      ros::spinOnce();

      loop_rate.sleep();

  }



  return 0;
}



