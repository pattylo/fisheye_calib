/*
    This file is part of LEDVO - learning dynamic factor for visual odometry

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

/*
 * \file calibrate.cpp
 * \date 01/03/2024
 * \author spmallick / learnopencv
 * \copyright (c) 
 * \brief intrinsic parameter calibration
*/

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

#include "essential.h"

// Defining the dimensions of checkerboard
int CHECKERBOARD[2]{6,9}; 
static std::string image_folder;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "calibrate");
    ros::NodeHandle nh("~");

    nh.getParam("image_path", image_folder);
    
    // Creating vector to store vectors of 3D points for each checkerboard image
    std::vector<std::vector<cv::Point3f> > objpoints;

    // Creating vector to store vectors of 2D points for each checkerboard image
    std::vector<std::vector<cv::Point2f> > imgpoints;

    // Defining the world coordinates for 3D points
    std::vector<cv::Point3f> objp;
    for(int i{0}; i<CHECKERBOARD[1]; i++)
    {
        for(int j{0}; j<CHECKERBOARD[0]; j++)
        objp.push_back(cv::Point3f(j,i,0));
    }


    // Extracting path of individual image stored in a given directory
    std::vector<cv::String> images;
    // Path of the folder containing checkerboard images

    std::cout<<image_folder<<std::endl;

    cv::glob(image_folder, images);

    cv::Mat frame, gray;
    // vector to store the pixel coordinates of detected checker board corners 
    std::vector<cv::Point2f> corner_pts;
    bool success;

    // Looping over all the images in the directory
    for(int i{0}; i<images.size(); i++)
    {
        frame = cv::imread(images[i]);
        cv::cvtColor(frame,gray,cv::COLOR_BGR2GRAY);

        // Finding checker board corners
        // If desired number of corners are found in the image then success = true  
        success = cv::findChessboardCorners(gray,cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]), corner_pts, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);

        /*
        * If desired number of corner are detected,
        * we refine the pixel coordinates and display 
        * them on the images of checker board
        */
        if(success)
        {
            cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);

            // refining pixel coordinates for given 2d points.
            cv::cornerSubPix(gray,corner_pts,cv::Size(11,11), cv::Size(-1,-1),criteria);

            // Displaying the detected corner points on the checker board
            cv::drawChessboardCorners(frame, cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]), corner_pts,success);

            objpoints.push_back(objp);
            imgpoints.push_back(corner_pts);
        }

        // cv::imshow("Image",frame);
        // cv::waitKey(0);
    }

    cv::destroyAllWindows();

    cv::Mat cameraMatrix,distCoeffs,R,T;

    /*
    * Performing camera calibration by 
    * passing the value of known 3D points (objpoints)
    * and corresponding pixel coordinates of the 
    * detected corners (imgpoints)
    */
    cv::calibrateCamera(objpoints, imgpoints,cv::Size(gray.rows,gray.cols),cameraMatrix,distCoeffs,R,T);

    std::cout << "cameraMatrix :\n" << cameraMatrix << std::endl;
    std::cout << "distCoeffs : \n" << distCoeffs << std::endl;
    // std::cout << "Rotation vector : " << R << std::endl;
    // std::cout << "Translation vector : " << T << std::endl;

    ros::spin();

    return 0;
}