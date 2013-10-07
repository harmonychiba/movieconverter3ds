//
//  main.cpp
//  movieconverter3ds
//
//  Created by 千葉優輝 on 2013/09/30.
//  Copyright (c) 2013年 ChibaQun. All rights reserved.
//

#include <iostream>
#include "Converter.h"

int main(int argc, const char * argv[])
{
    
    // insert code here...
    std::cout << "Hello, World!\n";
    Converter converter;
    converter.loadFile("/Users/Yuki/Desktop/raw.m4v");
    converter.setOutputFilePath("/Users/Yuki/Movies/sample.avi");
    converter.setOutputRectSize(640, 360);
    bool init = converter.initialize();
    if(!init){
        return 1;
    }
    converter.doDebug(3);
    converter.addReviseFrame(0, 0.5, 0.7, 0.25, 0.68, 0.5, 0.56, 0.26, 0.55);
    converter.converte();
    return 0;
}

