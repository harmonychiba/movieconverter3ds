//
//  Converter.cpp
//  movieconverter3ds
//
//  Created by 千葉優輝 on 2013/09/30.
//  Copyright (c) 2013年 ChibaQun. All rights reserved.
//

#include "Converter.h"

void Converter::doDebug(int a_debug_mode){
    cv::namedWindow("3DS Movie Converter Debug Window",CV_WINDOW_AUTOSIZE);
    debugging = true;
    debug_mode = a_debug_mode;
}
bool Converter::initialize(){
    if(input_file != "" && output_file != ""){
        capture.open(input_file);
        if(!capture.isOpened()){
            return false;
        }
        initialized = true;
        return true;
    }
    else{
        return false;
    }
};
void Converter::loadFile(std::string filename){
    input_file = filename;
};
void Converter::setOutputFilePath(std::string filename){
    output_file = filename;
};
void Converter::setOutputRectSize(int width, int height){
    output_width = width;
    output_height = height;
    cv::Point2f ul = cv::Point2f(0,0);
    cv::Point2f ur = cv::Point2f(width,0);
    cv::Point2f ll = cv::Point2f(0,height);
    cv::Point2f lr= cv::Point2f(width,height);
    
    output_pos.push_back(ul);
    output_pos.push_back(ur);
    output_pos.push_back(ll);
    output_pos.push_back(lr);
};
bool Converter::loadFrameImageAtFrame(int a_frame){
    if(initialized){
        frame = a_frame;
        int frame_count = (int)capture.get(CV_CAP_PROP_FRAME_COUNT);
        if(frame <= frame_count){
            capture.set(CV_CAP_PROP_POS_FRAMES, frame);
            capture >> frame_image;
            if(debugging && debug_mode == 0){
                cv::imshow("3DS Movie Converter Debug Window", frame_image);
                cv::waitKey(1);
            }
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
};

bool Converter::loadColorDataAtPos(double x_rate, double y_rate){
    if (&frame_image && x_rate <= 1 && y_rate <= 1) {
        int pos_x = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH)*x_rate;
        int pos_y = (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT)*y_rate;
        
        cv::Mat hsv_mat = cv::Mat::zeros(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT), CV_8UC3);
        cv::cvtColor(frame_image, hsv_mat, CV_RGB2HSV);
        cv::Vec3b pixel = hsv_mat.at<cv::Vec3b>(pos_y,pos_x);
        color_data = pixel;
        
        pos_data = cv::Point2f(pos_x,pos_y);
        
        return true;
    }
    else{
        return false;
    }
}

bool Converter::setColorAs(int pos){
    if(&color_data && pos >= 0 && pos <= 3){
        switch (pos){
            case 0:
                ul_data = color_data;
                ul_pos = pos_data;
                break;
            case 1:
                ur_data = color_data;
                ur_pos = pos_data;
                break;
            case 2:
                ll_data = color_data;
                ll_pos = pos_data;
                break;
            case 3:
                lr_data = color_data;
                lr_pos = pos_data;
                break;
            default:
                break;
        }
        return true;
    }
    else{
        return false;
    }
};

bool Converter::getRect(){
    if(&ul_data && &ur_data && &ll_data && &lr_data){
        if(debugging && debug_mode == 1){
            cv::Mat pointed_image;
            //frame_image.copyTo(pointed_image);
            cv::cvtColor(frame_image, pointed_image, CV_RGB2HSV);
            cv::Mat draw_image;
            cv::circle(pointed_image, ul_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ul_pos, 3, cv::Scalar((double)ul_data[0],(double)ul_data[1],(double)ul_data[2]),-1,CV_AA);
            cv::circle(pointed_image, ur_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ur_pos, 3, cv::Scalar(ur_data[0],ur_data[1],ur_data[2]),-1,CV_AA);
            cv::circle(pointed_image, ll_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ll_pos, 3, cv::Scalar(ll_data[0],ll_data[1],ll_data[2]),-1,CV_AA);
            cv::circle(pointed_image, lr_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, lr_pos, 3, cv::Scalar(lr_data[0],lr_data[1],lr_data[2]),-1,CV_AA);
            cv::cvtColor(pointed_image, draw_image, CV_HSV2RGB);
            cv::imshow("3DS Movie Converter Debug Window", draw_image);
            cv::waitKey(1);
        }
        cv::Mat hsv_img;
        cv::cvtColor(frame_image, hsv_img, CV_RGB2HSV);
        std::vector<cv::Mat> singlechannels;
        cv::split(hsv_img, singlechannels);
        cv::Mat ul_bin_img,ur_bin_img,ll_bin_img,lr_bin_img,hue1,hue2,saturation1,saturation2,value1,value2,hue,saturation,value,out1;
        
        cv::threshold(singlechannels[0], hue1, (double)ul_data[0]-5, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[0], hue2, (double)ul_data[0]+5, 255, CV_THRESH_BINARY_INV);
        cv::threshold(singlechannels[1], saturation1, (double)ul_data[1]-75, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[1], saturation2, (double)ul_data[1]+75, 255, CV_THRESH_BINARY_INV);
        cv::threshold(singlechannels[2], value1, (double)ul_data[2]-75, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[2], value2, (double)ul_data[2]+75, 255, CV_THRESH_BINARY_INV);
        cv::bitwise_and(hue1, hue2, hue);
        cv::bitwise_and(saturation1, saturation2, saturation);
        cv::bitwise_and(value1, value2, value);
        cv::bitwise_and(hue, saturation, out1);
        cv::bitwise_and(out1, value, ul_bin_img);
        
        cv::threshold(singlechannels[0], hue1, (double)ur_data[0]-5, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[0], hue2, (double)ur_data[0]+5, 255, CV_THRESH_BINARY_INV);
        cv::threshold(singlechannels[1], saturation1, (double)ur_data[1]-75, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[1], saturation2, (double)ur_data[1]+75, 255, CV_THRESH_BINARY_INV);
        cv::threshold(singlechannels[2], value1, (double)ur_data[2]-75, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[2], value2, (double)ur_data[2]+75, 255, CV_THRESH_BINARY_INV);
        cv::bitwise_and(hue1, hue2, hue);
        cv::bitwise_and(saturation1, saturation2, saturation);
        cv::bitwise_and(value1, value2, value);
        cv::bitwise_and(hue, saturation, out1);
        cv::bitwise_and(out1, value, ur_bin_img);
        
        cv::threshold(singlechannels[0], hue1, (double)ll_data[0]-5, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[0], hue2, (double)ll_data[0]+5, 255, CV_THRESH_BINARY_INV);
        cv::threshold(singlechannels[1], saturation1, (double)ll_data[1]-75, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[1], saturation2, (double)ll_data[1]+75, 255, CV_THRESH_BINARY_INV);
        cv::threshold(singlechannels[2], value1, (double)ll_data[2]-75, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[2], value2, (double)ll_data[2]+75, 255, CV_THRESH_BINARY_INV);
        cv::bitwise_and(hue1, hue2, hue);
        cv::bitwise_and(saturation1, saturation2, saturation);
        cv::bitwise_and(value1, value2, value);
        cv::bitwise_and(hue, saturation, out1);
        cv::bitwise_and(out1, value, ll_bin_img);
        
        cv::threshold(singlechannels[0], hue1, (double)lr_data[0]-5, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[0], hue2, (double)lr_data[0]+5, 255, CV_THRESH_BINARY_INV);
        cv::threshold(singlechannels[1], saturation1, (double)lr_data[1]-75, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[1], saturation2, (double)lr_data[1]+75, 255, CV_THRESH_BINARY_INV);
        cv::threshold(singlechannels[2], value1, (double)lr_data[2]-75, 255, CV_THRESH_BINARY);
        cv::threshold(singlechannels[2], value2, (double)lr_data[2]+75, 255, CV_THRESH_BINARY_INV);
        cv::bitwise_and(hue1, hue2, hue);
        cv::bitwise_and(saturation1, saturation2, saturation);
        cv::bitwise_and(value1, value2, value);
        cv::bitwise_and(hue, saturation, out1);
        cv::bitwise_and(out1, value, lr_bin_img);
        
        if(debugging && debug_mode == 2){
            cv::Mat pointed_image;
            ul_bin_img.copyTo(pointed_image);
            cv::circle(pointed_image, ul_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ul_pos, 3, cv::Scalar(ul_data[0],ul_data[1],ul_data[2]),-1,CV_AA);
            cv::imshow("3DS Movie Converter Debug Window", pointed_image);
            cv::waitKey(1);
        }
        
        if(debugging && debug_mode == 3){
            cv::Mat pointed_image;
            ur_bin_img.copyTo(pointed_image);
            cv::circle(pointed_image, ur_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ur_pos, 3, cv::Scalar(ur_data[0],ur_data[1],ur_data[2]),-1,CV_AA);
            cv::imshow("3DS Movie Converter Debug Window", pointed_image);
            cv::waitKey(1);
        }
        
        if(debugging && debug_mode == 4){
            cv::Mat pointed_image;
            ll_bin_img.copyTo(pointed_image);
            cv::circle(pointed_image, ll_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ll_pos, 3, cv::Scalar(ll_data[0],ll_data[1],ll_data[2]),-1,CV_AA);
            cv::imshow("3DS Movie Converter Debug Window", pointed_image);
            cv::waitKey(1);
        }
        
        if(debugging && debug_mode == 5){
            cv::Mat pointed_image;
            lr_bin_img.copyTo(pointed_image);
            cv::circle(pointed_image, lr_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, lr_pos, 3, cv::Scalar(lr_data[0],lr_data[1],lr_data[2]),-1,CV_AA);
            cv::imshow("3DS Movie Converter Debug Window", pointed_image);
            cv::waitKey(1);
        }
        cv::Mat element(3, 3,CV_8U,cv::Scalar(1));
        cv::Mat ul_mol_img,ur_mol_img,ll_mol_img,lr_mol_img;
        
        cv::morphologyEx(ul_bin_img, ul_mol_img, CV_MOP_OPEN, element);
        std::vector<std::vector<cv::Point> > ul_contours;
        cv::Mat ul_mol;
        ul_mol_img.copyTo(ul_mol);
        cv::findContours(ul_mol_img, ul_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
        int min_ind = -1;
        double min_value = DBL_MAX;
        double average_x,average_y;
        for(int i = 0;i<ul_contours.size();i++){
            double ave_x=0.0,ave_y=0.0;
            for(int j = 0;j<ul_contours[i].size();j++){
                ave_x += ul_contours[i][j].x;
                ave_y += ul_contours[i][j].y;
            }
            ave_x /= ul_contours[i].size();
            ave_y /= ul_contours[i].size();
            
            double pow_dis = (ave_x - ul_pos.x) * (ave_x - ul_pos.x) + (ave_y - ul_pos.y) * (ave_y - ul_pos.y);
            if(pow_dis < min_value){
                min_ind = i;
                min_value = pow_dis;
                average_x = ave_x;
                average_y = ave_y;
            }
        }
        ul_pos = cv::Point2f(ul_pos.x * 0.7 + average_x * 0.3,ul_pos.y *0.7 + average_y*0.3);
        
        cv::morphologyEx(ur_bin_img, ur_mol_img, CV_MOP_OPEN, element);
        std::vector<std::vector<cv::Point> > ur_contours;
        cv::Mat ur_mol;
        ur_mol_img.copyTo(ur_mol);
        cv::findContours(ur_mol_img, ur_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
        min_ind = -1;
        min_value = DBL_MAX;
        for(int i = 0;i<ur_contours.size();i++){
            double ave_x=0.0,ave_y=0.0;
            for(int j = 0;j<ur_contours[i].size();j++){
                ave_x += ur_contours[i][j].x;
                ave_y += ur_contours[i][j].y;
            }
            ave_x /= ur_contours[i].size();
            ave_y /= ur_contours[i].size();
            
            double pow_dis = (ave_x - ur_pos.x) * (ave_x - ur_pos.x) + (ave_y - ur_pos.y) * (ave_y - ur_pos.y);
            if(pow_dis < min_value){
                min_ind = i;
                min_value = pow_dis;
                average_x = ave_x;
                average_y = ave_y;
            }
        }
        ur_pos = cv::Point2f(ur_pos.x * 0.7 + average_x * 0.3,ur_pos.y *0.7 + average_y*0.3);
        
        cv::morphologyEx(ll_bin_img, ll_mol_img, CV_MOP_OPEN, element);
        std::vector<std::vector<cv::Point> > ll_contours;
        cv::Mat ll_mol;
        ll_mol_img.copyTo(ll_mol);
        cv::findContours(ll_mol_img, ll_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
        min_ind = -1;
        min_value = DBL_MAX;
        for(int i = 0;i<ll_contours.size();i++){
            double ave_x=0.0,ave_y=0.0;
            for(int j = 0;j<ll_contours[i].size();j++){
                ave_x += ll_contours[i][j].x;
                ave_y += ll_contours[i][j].y;
            }
            ave_x /= ll_contours[i].size();
            ave_y /= ll_contours[i].size();
            
            double pow_dis = (ave_x - ll_pos.x) * (ave_x - ll_pos.x) + (ave_y - ll_pos.y) * (ave_y - ll_pos.y);
            if(pow_dis < min_value){
                min_ind = i;
                min_value = pow_dis;
                average_x = ave_x;
                average_y = ave_y;
            }
        }
        ll_pos = cv::Point2f(ll_pos.x * 0.7 + average_x * 0.3,ll_pos.y *0.7 + average_y*0.3);
        
        cv::morphologyEx(lr_bin_img, lr_mol_img, CV_MOP_OPEN, element);
        std::vector<std::vector<cv::Point> > lr_contours;
        cv::Mat lr_mol;
        lr_mol_img.copyTo(lr_mol);
        cv::findContours(lr_mol_img, lr_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
        min_ind = -1;
        min_value = DBL_MAX;
        for(int i = 0;i<lr_contours.size();i++){
            double ave_x=0.0,ave_y=0.0;
            for(int j = 0;j<lr_contours[i].size();j++){
                ave_x += lr_contours[i][j].x;
                ave_y += lr_contours[i][j].y;
            }
            ave_x /= lr_contours[i].size();
            ave_y /= lr_contours[i].size();
            
            double pow_dis = (ave_x - lr_pos.x) * (ave_x - lr_pos.x) + (ave_y - lr_pos.y) * (ave_y - lr_pos.y);
            if(pow_dis < min_value){
                min_ind = i;
                min_value = pow_dis;
                average_x = ave_x;
                average_y = ave_y;
            }
        }
        lr_pos = cv::Point2f(lr_pos.x * 0.7 + average_x * 0.3,lr_pos.y *0.7 + average_y*0.3);
    
        if(debugging && debug_mode == 6){
            cv::Mat pointed_image;
            ul_mol_img.copyTo(pointed_image);
            cv::circle(pointed_image, ul_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ul_pos, 3, cv::Scalar(ul_data[0],ul_data[1],ul_data[2]),-1,CV_AA);
            cv::imshow("3DS Movie Converter Debug Window", pointed_image);
            cv::waitKey(1);
        }
        
        if(debugging && debug_mode == 7){
            cv::Mat pointed_image;
            ul_mol_img.copyTo(pointed_image);
            cv::circle(pointed_image, ur_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ur_pos, 3, cv::Scalar(ur_data[0],ur_data[1],ur_data[2]),-1,CV_AA);
            cv::imshow("3DS Movie Converter Debug Window", pointed_image);
            cv::waitKey(1);
        }
        
        if(debugging && debug_mode == 8){
            cv::Mat pointed_image;
            ll_mol_img.copyTo(pointed_image);
            cv::circle(pointed_image, ll_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ll_pos, 3, cv::Scalar(ll_data[0],ll_data[1],ll_data[2]),-1,CV_AA);
            cv::imshow("3DS Movie Converter Debug Window", pointed_image);
            cv::waitKey(1);
        }
        
        if(debugging && debug_mode == 9){
            cv::Mat pointed_image;
            lr_mol_img.copyTo(pointed_image);
            cv::circle(pointed_image, lr_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, lr_pos, 3, cv::Scalar(lr_data[0],lr_data[1],lr_data[2]),-1,CV_AA);
            cv::imshow("3DS Movie Converter Debug Window", pointed_image);
            cv::waitKey(1);
        }
        
        if(debugging && debug_mode == 10){
            cv::Mat pointed_image;
            //frame_image.copyTo(pointed_image);
            cv::cvtColor(frame_image, pointed_image, CV_RGB2HSV);
            cv::Mat draw_image;
            cv::circle(pointed_image, ul_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ul_pos, 3, cv::Scalar((double)ul_data[0],(double)ul_data[1],(double)ul_data[2]),-1,CV_AA);
            cv::circle(pointed_image, ur_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ur_pos, 3, cv::Scalar(ur_data[0],ur_data[1],ur_data[2]),-1,CV_AA);
            cv::circle(pointed_image, ll_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ll_pos, 3, cv::Scalar(ll_data[0],ll_data[1],ll_data[2]),-1,CV_AA);
            cv::circle(pointed_image, lr_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, lr_pos, 3, cv::Scalar(lr_data[0],lr_data[1],lr_data[2]),-1,CV_AA);
            cv::cvtColor(pointed_image, draw_image, CV_HSV2RGB);
            cv::imshow("3DS Movie Converter Debug Window", draw_image);
            cv::waitKey(1);
        }
        
        std::vector<cv::Point2f> input_pos;
        input_pos.push_back(ul_pos);
        input_pos.push_back(ur_pos);
        input_pos.push_back(ll_pos);
        input_pos.push_back(lr_pos);
        
        cv::Mat transform_matrix = cv::getPerspectiveTransform(input_pos, output_pos);
        cv::warpPerspective(frame_image, rect_image, transform_matrix, cv::Size(output_width, output_height));
        
        if(debugging && debug_mode == -1){
            cv::Mat pointed_image;
            //frame_image.copyTo(pointed_image);
            cv::cvtColor(rect_image, pointed_image, CV_RGB2HSV);
            cv::Mat draw_image;
            cv::circle(pointed_image, ul_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ul_pos, 3, cv::Scalar((double)ul_data[0],(double)ul_data[1],(double)ul_data[2]),-1,CV_AA);
            cv::circle(pointed_image, ur_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ur_pos, 3, cv::Scalar(ur_data[0],ur_data[1],ur_data[2]),-1,CV_AA);
            cv::circle(pointed_image, ll_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, ll_pos, 3, cv::Scalar(ll_data[0],ll_data[1],ll_data[2]),-1,CV_AA);
            cv::circle(pointed_image, lr_pos, 5, cv::Scalar(200,200,200), -1, CV_AA);
            cv::circle(pointed_image, lr_pos, 3, cv::Scalar(lr_data[0],lr_data[1],lr_data[2]),-1,CV_AA);
            cv::cvtColor(pointed_image, draw_image, CV_HSV2RGB);
            cv::imshow("3DS Movie Converter Debug Window", draw_image);
            cv::waitKey(1);
        }
        
    }
    return true;
};
void Converter::addReviseFrame(int frame, double ul_x, double ul_y, double ur_x, double ur_y, double ll_x, double ll_y, double lr_x, double lr_y){
    std::vector<cv::Point2f> points;
    points.push_back(cv::Point2f(ul_x,ul_y));
    points.push_back(cv::Point2f(ur_x,ur_y));
    points.push_back(cv::Point2f(ll_x,ll_y));
    points.push_back(cv::Point2f(lr_x,lr_y));
    revise_points.push_back(points);
    revise_frames.push_back(frame);
};

bool Converter::converte(){
    if(revise_frames.size() == 0 || revise_points.size() == 0){
        return false;
    }
    writer = cv::VideoWriter();
    writer.open(output_file, CV_FOURCC_DEFAULT, capture.get(CV_CAP_PROP_FPS), cv::Size(output_width,output_height));
    std::cout << "initialize video writer => fps:" << capture.get(CV_CAP_PROP_FPS) << ", size:("<<output_width << "," << output_height << ")"<<std::endl;
    if(writer.isOpened() != true){
        std::cout << "can't open file to write video" << std::endl;
    }
    frame = 0;
    while(true){
        bool ok = loadFrameImageAtFrame(frame);
        if(!ok){
            break;
        }
        for(int i = 0;i<revise_frames.size();i++){
            if(revise_frames[i] == frame){
                for(int j = 0;j<4;j++){
                    loadColorDataAtPos(revise_points[i][j].x, revise_points[i][j].y);
                    setColorAs(j);
                }
            }
        }
        getRect();
        writeImage();
        frame+=1;
    }
    writer.release();
    return true;
};

void Converter::writeImage(){
    if(writer.isOpened()){
        writer.write(rect_image);
        double rate = frame * 100.0 / capture.get(CV_CAP_PROP_FRAME_COUNT);
        std::cout << rate << "% writen" << std::endl;
    }
};
