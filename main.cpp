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
    converter.loadFile("/Users/Yuki/Movies/sample1.mov");
    converter.setOutputFilePath("/Users/Yuki/Movies/sample.avi");
    converter.setOutputRectSize(640, 360);
    bool init = converter.initialize();
    if(!init){
        return 1;
    }
    converter.doDebug(-2);
    converter.addReviseFrame(0, 0.22, 0.43, 0.58, 0.45, 0.2, 0.72, 0.55, 0.70);
    converter.converte();
    return 0;
}

