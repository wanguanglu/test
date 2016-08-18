/**
* @file   test_shared_ptr.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2016/08/18 09:44:30
* @brief 
*  
**/

#include <boost/shared_ptr.hpp>
#include <logging.h>

class Test {
public:
    Test() {
        log_info("construct.");
    }

    ~Test() {
        log_info("destruct.");
    }
};


int main(int argc, char* argv[]) {

    boost::shared_ptr<Test> ptr = boost::shared_ptr<Test>(new Test());
    boost::shared_ptr<Test> ptr2 = ptr;
    log_info("ptr2.use_count():%d.", ptr.use_count());

    return 0;
}
