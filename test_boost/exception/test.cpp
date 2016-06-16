/**
* @file   test.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2016/06/16 23:36:12
* @brief 
*  
**/

#include <boost/exception/all.hpp>
#include <iostream>


class MyException : virtual public std::exception,virtual public boost::exception {
};

//定义错误信息类型，
typedef boost::error_info<struct tag_err_no, int> err_no;
typedef boost::error_info<struct tag_err_str, std::string> err_str;

void test_exception() {
    try {
        throw MyException() << err_no(10) << err_str("error");
    } catch(std::exception& e) {
        std::cout << *boost::get_error_info<err_no>(e) << "\t" << *boost::get_error_info<err_str>(e) << std::endl;
    }
}

int main(int argc, char* argv[]) {

    test_exception();
    return 0;
}
