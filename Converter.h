//
//  Converter.h
//  movieconverter3ds
//
//  Created by 千葉優輝 on 2013/09/30.
//  Copyright (c) 2013年 ChibaQun. All rights reserved.
//
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "boost/python.hpp"

class Converter{
private:
    bool debugging;
    int debug_mode;
    std::string input_file;
    std::string output_file;
    int output_width;
    int output_height;
    std::vector<cv::Point2f> output_pos;
    bool initialized;
    int frame;
    cv::VideoCapture capture;
    cv::VideoWriter writer;
    cv::Mat frame_image;
    cv::Mat frame_src;
    cv::Vec3b color_data;
    cv::Point2f pos_data,ul_pos,ur_pos,ll_pos,lr_pos;
    cv::Vec3b ul_data,ur_data,ll_data,lr_data;
    cv::Point2f in_ul,in_ur,in_ll,in_lr;
    std::vector<int> revise_frames;
    std::vector< std::vector< cv::Point2f > > revise_points;
    cv::Mat rect_image;
    void writeImage();
public:
    void doDebug(int debug_mode);
    void loadFile(const std::string &filename);
    void addReviseFrame(int frame,double ul_x,double ul_y,double ur_x,double ur_y,double ll_x,double ll_y,double lr_x,double lr_y);
    void setOutputFilePath(const std::string &filename);
    void setOutputRectSize(int width,int height);
    bool initialize();
    bool loadFrameImageAtFrame(int frame);
    bool loadColorDataAtPos(double x_rate,double y_rate);
    bool setColorAs(int pos);
    bool getRect();
    bool converte();
    bool previewAtFrame(int frame);
    void previewInputPoints(double ul_x,double ul_y,double ur_x,double ur_y,double ll_x,double ll_y,double lr_x,double lr_y);
};
