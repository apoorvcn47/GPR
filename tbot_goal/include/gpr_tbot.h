#ifndef GPR_TBOT_H
#define GPR_TBOT_H

#include "geometry_msgs/PointStamped.h"
#include "move_base_msgs/MoveBaseAction.h"
#include "actionlib/client/simple_action_client.h"
#include "ros/ros.h"
#include "visualization_msgs/MarkerArray.h"
#include <vector>

class gpr_tbot
{
public:
    int no_fs, no_ss, tp;

    double x1, x2, y1, y2, fs, ss, last_fs, last_ss; //, X[500], Y[500];
    //double* X = new double [tp];
    //double* Y = new double [tp];
    std::vector<double> X, Y;

    int q = 0;
    double pt_X[2], pt_Y[2];

    void tp_calculation();
    void grid_calculation();
    void grid_trace(move_base_msgs::MoveBaseGoal, actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>*);
    void callback(const geometry_msgs::PointStamped::ConstPtr& );

};
#endif
