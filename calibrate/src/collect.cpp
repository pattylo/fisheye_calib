/*
    This file is part of LEDVO - the relative state estimation package for UAV-UGV

    LEDVO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LEDVO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LEDVO.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * \file collect.cpp
 * \date 01/03/2024
 * \author pattylo
 * \copyright (c) RCUAS of Hong Kong Polytechnic University
 * \brief classes for fisheye-based relative localization for UAV and UGV based on LED markers
 */

#include "essential.h"
#include <opencv2/calib3d.hpp>

static double last_request;
static std::string image_save_folder;
static int image_i = 0;

void fisheye_callback(const sensor_msgs::Image::ConstPtr& image)
{
    // main process here:
    cv_bridge::CvImageConstPtr fisheye_ptr;
    try
    {
        fisheye_ptr  = cv_bridge::toCvCopy(image, image->encoding);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
    }

    cv::Mat fisheye_frame = fisheye_ptr->image;

    // cv::imshow("fisheye", fisheye_frame);
    // cv::waitKey(10);

    if(ros::Time::now().toSec() - last_request > ros::Duration(2.50).toSec())
    {
        std::string filename = image_save_folder + "fisheye_" + std::to_string(image_i) + ".png";
        std::remove(filename.c_str());

        cv::imwrite(filename, fisheye_frame);
        image_i ++;
        last_request = ros::Time::now().toSec();
    }


}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "collect");
    ros::NodeHandle nh("~");

    nh.getParam("log_path", image_save_folder);

    last_request = ros::Time::now().toSec();

    ros::Subscriber fisheye_sub = nh.subscribe<sensor_msgs::Image>
        ("/camera/fisheye1/image_raw", 1, &fisheye_callback);

    ros::spin();

    return 0;
}