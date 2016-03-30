/**
* @file   test_boost_log.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2016/03/29 17:37:44
* @brief 
*  
**/


#include <boost/log/trivial.hpp>

int main(int argc, char* argv[]) {

    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

    return 0;
}
