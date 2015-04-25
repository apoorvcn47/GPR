#include "gpr_tbot.h"
#include "ros/ros.h"
#include "actionlib/client/simple_action_client.h"


void gpr_tbot::tp_calculation()
{

    no_fs = abs(y2 - y1)/fs;
    last_fs = (y2-y1) - floor((y2-y1) / fs) * fs;       //fmod(abs(y2 - y1),fs);

    no_ss = abs(x2 - x1)/ss;
    last_ss = (x2-x1) - floor((x2-x1) / ss) * ss;        //fmod(abs(x2 - x1),ss);

    if(y2 < y1) {
        fs = -1*fs;
        last_fs = -1*last_fs;

    }
    if(x2 < x1) {
        ss = -1*ss;
        last_ss = -1*last_ss;

    }



    tp = (no_fs + 2)*(no_ss + 2);
    ROS_INFO("fs = %f ss = %f last_fs = %f last_ss = %f  no_fs = %i no_ss = %i", fs, ss, last_fs, last_ss, no_fs, no_ss);

}



void gpr_tbot::grid_calculation()
{


    for(int i = 0; i<tp; i++){
        if(i == 0){
            X.push_back(x1); //X[0] = x1;
            Y.push_back(y1); //Y[0] = y1;
        }
        else if(i == tp-1){
            if((no_ss % 2)!=0){
                X.push_back(x2); //X[tp-1]=x2;
                Y.push_back(y2); //Y[tp-1]=y2;
            }
            else if((no_ss % 2)==0){
                X.push_back(x2); //X[tp-1]=x2;
                Y.push_back(y1); //Y[tp-1]=y1;
            }
        }
        else if(((i%(no_fs+2))>0) && ((i/(no_fs+2))%2 ==0) && ((i%(no_fs+2))<(no_fs+1))){
            Y.push_back(Y.at(i-1) +fs); //Y[i] = Y[i-1] + fs;
            X.push_back(X.at(i-1)); //X[i] = X[i-1];
        }
        else if(((i%(no_fs+2))>0) && ((i/(no_fs+2))%2 !=0) && ((i%(no_fs+2))<(no_fs+1))){
            Y.push_back(Y.at(i-1) -fs); //Y[i] = Y[i-1] - fs;
            X.push_back(X.at(i-1)); //X[i] = X[i-1];
        }
        else if(((i%(no_fs+2))==0) && ((i/(no_fs+2)) ==(no_ss+1))){
            Y.push_back(Y.at(i-1)); //Y[i] = Y[i-1];
            X.push_back(X.at(i-1) +last_ss); //X[i] = X[i-1] + last_ss;
        }
        else if(((i%(no_fs+2))==(no_fs+1)) && ((i/(no_fs+2))%2 ==0)){
            Y.push_back(Y.at(i-1) +last_fs); //Y[i] = Y[i-1] + last_fs;
            X.push_back(X.at(i-1)); //X[i] = X[i-1];
        }
        else if(((i%(no_fs+2))==0) && ((i/(no_fs+2)) !=(no_ss+1))){
            Y.push_back(Y.at(i-1)); //Y[i] = Y[i-1];
            X.push_back(X.at(i-1) +ss); //X[i] = X[i-1] + ss;
        }

        else if(((i%(no_fs+2))==(no_fs+1)) && ((i/(no_fs+2))%2 !=0)){
            Y.push_back(Y.at(i-1) -last_fs); //Y[i] = Y[i-1] - last_fs;
            X.push_back(X.at(i-1)); //X[i] = X[i-1];
        }





    }
}


void gpr_tbot::grid_trace( move_base_msgs::MoveBaseGoal goal, actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> *ac)
{
    for(int j = 0; j < tp; j++) {
        if(j==0){
            goal.target_pose.pose.position.x = X[j];
            goal.target_pose.pose.position.y = Y[j];
            goal.target_pose.pose.orientation.z = 0.7071;
            goal.target_pose.pose.orientation.w = 0.7071;


        }
        else{
            double num, den, alpha;
            num = Y[j] - Y[j-1];
            den = X[j] - X[j-1];
            alpha = atan2(num, den);

            goal.target_pose.pose.position.x = X[j];
            goal.target_pose.pose.position.y = Y[j];
            goal.target_pose.pose.orientation.z = sin(alpha/2);
            goal.target_pose.pose.orientation.w = cos(alpha/2);

        }

    ROS_INFO("Target point is %f, %f", X[j], Y[j]);


    ac->sendGoal(goal);
    ROS_INFO("goal sent");
    ac->waitForResult();
    ROS_INFO("got result");



    if(ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("Point No %i reached", j+1);
    else
      ROS_INFO("The base failed to reach Point No %i", j+1);

    }

}

void gpr_tbot::callback(const geometry_msgs::PointStamped::ConstPtr& msg){
    pt_X[q] = msg->point.x;
    pt_Y[q] = msg->point.y;
    ROS_INFO("Point no %i is %f, %f", q+1, pt_X[q], pt_Y[q]);
    q++;
}


