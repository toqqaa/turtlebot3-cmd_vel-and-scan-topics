#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"





geometry_msgs::Twist move_msg;
ros::Publisher move_pub ;
ros::Subscriber laser_sub ;
bool detect_obstacle_front = false;

void move_forward()
{
    move_msg.linear.x = 0.3;
    move_msg.angular.z = 0.0;
}

void move_right()
{
    move_msg.linear.x = 0.3;
    move_msg.angular.z = -0.5;
}

void move_left()
{
    move_msg.linear.x = 0.3;
    move_msg.angular.z = 0.5;
}


void laserCallback(const sensor_msgs::LaserScan::ConstPtr& laser_msg)
{
    double forward_distance = laser_msg->ranges[laser_msg->ranges.size() / 2];
    double left_distance = laser_msg->ranges.front();
    double right_distance = laser_msg->ranges.back();

    if (!detect_obstacle_front)
    {
        if (forward_distance < 1.0)
        {
            detect_obstacle_front = true;
           
            move_left();
        }
        else
        {
           
            move_forward();
        }
    }
    else
    {
        if (right_distance < 1.0)
        {
            detect_obstacle_front = false;
            
            move_right();
        }
        else
        {
           
            move_left();
           
        }
    }
}

int main(int argc, char **argv)
{
  
   ros::init(argc, argv, "topics_project_node");
   ros::NodeHandle n;
   laser_sub = n.subscribe("/scan", 10, laserCallback);
    move_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
  
ros::Rate loop_rate(10);




while (ros::ok())


  {

     
    ros::spinOnce();

  move_pub.publish(move_msg); 

    loop_rate.sleep();
   
  }

  return 0;
}
