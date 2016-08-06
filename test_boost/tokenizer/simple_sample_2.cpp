/**
* @file   simple_sample_2.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2016/05/04 00:41:47
* @brief 
*  
**/

#include<iostream>
#include<boost/tokenizer.hpp>
#include<string>

#include <logging.h>

int main(){
    wangl::log::log_init("log", "test");
    
    std::string s = "Field 1,\"putting quotes around fields, allows commas\",Field 3";
    boost::tokenizer<boost::escaped_list_separator<char> > tok(s);

    log_info("start simple sample 2");
    for(boost::tokenizer<boost::escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg){
        log_info("%s.", (*beg).c_str());
    }
}
