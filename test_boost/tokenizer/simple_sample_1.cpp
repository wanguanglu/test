/**
* @file   simple_sample_1.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2016/05/04 00:25:52
* @brief 
*  
**/

#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>

#include <logging.h>

int main() {
    wangl::log::log_init("log", "test");


    std::string s = "This is,  a test";
    boost::tokenizer<> tok(s);

    for(boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg){
        log_info("%s", (*beg).c_str());
    }
}
