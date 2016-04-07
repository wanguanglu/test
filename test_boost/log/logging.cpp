/**
* @file   logging.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2016/04/07 10:58:25
* @brief 
*  
**/

#include "logging.h"


namespace glorey {
namespace log {

int init_log(const char* log_dir, const char* log_file) {
    char path_name[MAX_PATH_LEN];

    snprintf(path_name, MAX_PATH_LEN, "%s/%s.%%Y%%m%%d%%H", log_dir, log_file);
    path_name[MAX_PATH_LEN-1] = '\0';

    logging::add_file_log (
        keywords::file_name = path_name,
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::open_mode = std::ios::app,
        keywords::format = (
            expr::stream
            <<logging::trivial::severity<< ": " 
            <<expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")<< ": "
            <<expr::format_named_scope("Scopes", keywords::format="[%F]",  keywords::iteration = expr::reverse)
            << expr::smessage
        )
    );

    logging::add_common_attributes();
    logging::core::get()->add_global_attribute("Scopes", attrs::named_scope());

    return 0;
}


}
}

int main() {
    glorey::log::init_log("log", "test");

    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(info)  << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error)   << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal)   << "A fatal severity message";

    return 0;
}
