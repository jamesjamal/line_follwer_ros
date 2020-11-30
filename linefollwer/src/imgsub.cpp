#include <ros/ros.h>
#include <std_msgs/Int64.h>
#include <std_srvs/SetBool.h>
#include <sensor_msgs/Image.h>
#include<nav_msgs/Odometry.h>
#include<geometry_msgs/Twist.h>

class Imagesub {

    private:
    //int counter;

    ros::Subscriber image_subscriber;
    ros::Publisher pub;
    
   // ros::ServiceServer reset_service;



    public:
    Imagesub(ros::NodeHandle *nh) {


          
        image_subscriber = nh->subscribe("/camera/rgb/image_raw", 1, &Imagesub::callback_number, this);

pub = nh->advertise<geometry_msgs::Twist>("/cmd_vel", 1); 
    }

    void callback_number(const sensor_msgs::Image& img) {
            int scan_start = (img.data.size() * 2/3);
            int scan_end = img.data.size() * 3/3;
            int num_yellow_pixels =0;
            int x_position_sum = 0;
            geometry_msgs::Twist msg;
           // pub.publish(msg);
            for (int i=scan_start; i<scan_end; i+=3){

                int red_channel =  img.data[i];
                int green_channel = img.data[i+1];
                int blue_channel = img.data[i+2];
                if (red_channel == 177 && green_channel == 171 && blue_channel == 0){

      int x_position = (i % (img.width * 3)) / 3;
      x_position_sum += x_position;
      num_yellow_pixels += 1;
                     
}


  if (num_yellow_pixels == 0)
  {
    ROS_INFO("no line");
         msg.angular.z=-0.1;
         
     pub.publish(msg);
    

  }
  else
  {
    int mean_x_position = x_position_sum / num_yellow_pixels;
    if (mean_x_position < img.width / 3)
    {
      ROS_INFO("left");
     msg.linear.x=0.05;
     msg.angular.z=0.1;
    
     pub.publish(msg);
    }
    else if (mean_x_position > img.width * 2 / 3)
    {
      ROS_INFO("right");
     // drive_robot(0.5, 0.0);
     msg.linear.x=0.05;
     msg.angular.z=-0.1;
     pub.publish(msg);
    }
    else
    {
      ROS_INFO("centre");
     msg.linear.x=0.1;
     //msg.linear.y=0.4;
     pub.publish(msg);
    }
}

                
             }

    }


};

int main (int argc, char **argv)
{
    ros::init(argc, argv, "number_counter");
    ros::NodeHandle nh;
    Imagesub is = Imagesub(&nh);
    ros::spin();
}
