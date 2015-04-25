#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseStamped.h"

//Motion purely based on velocity

using namespace ros;

int steps = 10;


int main(int argc, char **argv) {

    init(argc, argv, "mover");

    NodeHandle n;
    //Publisher cmd_vel = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 1);
    Publisher cmd_vel = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1);


    //geometry_msgs::Twist move_cmd;
    geometry_msgs::PoseStamped move_cmd;




    Rate loop_rate(1);


    int count = 1;

    //move_cmd.angular.z = 0.00;
    //move_cmd.linear.x = 2.00;


    while (ros::ok())
    {


      ROS_INFO("%i", count);

      move_cmd.pose.position.x = 0.00;
      move_cmd.pose.position.y = 0.00;


    /*

      if(count % (steps + 3) <= steps){
          move_cmd.angular.z = 0.00;
          move_cmd.linear.x = 1.00;
          move_cmd.linear.y = 0.00;
          move_cmd.linear.z = 0.00;
          move_cmd.angular.x = 0.00;
          move_cmd.angular.y = 0.00;
      }
      else if((count % (steps + 3)== (steps+1)) && ((count/steps)%2 == 0)){
          move_cmd.angular.z = 5.57;
          move_cmd.linear.x = 0.00;
          move_cmd.linear.y = 0.00;
          move_cmd.linear.z = 0.00;
          move_cmd.angular.x = 0.00;
          move_cmd.angular.y = 0.00;

      }
      else if((count % (steps + 3)== (steps+1)) && ((count/steps)%2 != 0)){
          move_cmd.angular.z = -5.57;
          move_cmd.linear.x = 0.00;
          move_cmd.linear.y = 0.00;
          move_cmd.linear.z = 0.00;
          move_cmd.angular.x = 0.00;
          move_cmd.angular.y = 0.00;

      }
      else if(count % (steps + 3) <= (steps + 2)){
          move_cmd.angular.z = 0.00;
          move_cmd.linear.x = 1.00;
          move_cmd.linear.y = 0.00;
          move_cmd.linear.z = 0.00;
          move_cmd.angular.x = 0.00;
          move_cmd.angular.y = 0.00;
      }
      else if((count % (steps + 3)== 0) && ((count/steps)%2 != 0)){
          move_cmd.angular.z = 5.57;
          move_cmd.linear.x = 0.00;
          move_cmd.linear.y = 0.00;
          move_cmd.linear.z = 0.00;
          move_cmd.angular.x = 0.00;
          move_cmd.angular.y = 0.00;

      }
      else if((count % (steps + 3)== 0) && ((count/steps)%2 == 0)){
          move_cmd.angular.z = -5.57;
          move_cmd.linear.x = 0.00;
          move_cmd.linear.y = 0.00;
          move_cmd.linear.z = 0.00;
          move_cmd.angular.x = 0.00;
          move_cmd.angular.y = 0.00;

      }  */


      cmd_vel.publish(move_cmd);

      ros::spinOnce();

      loop_rate.sleep();
      ++count;
    }

    //cmd_vel.publish(move_cmd);

    //ros::spin();


    return 0;
}
