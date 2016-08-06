/**
* @file   char_sep_example_1.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2016/05/04 00:49:45
* @brief 
*  
**/

#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>

#include <logging.h>

int main() {
    wangl::log::log_init("log", "test");

    std::string str = ";;Hello|world||-foo--bar;yow;baz|";
    typedef boost::tokenizer<boost::char_separator<char> >  tokenizer;
    boost::char_separator<char> sep("-;|");
    
    tokenizer tokens(str, sep);
    for (tokenizer::iterator tok_iter = tokens.begin();
                tok_iter != tokens.end(); ++tok_iter) {
        log_info("<%s>", (*tok_iter).c_str());
    }

    return 0;
}
