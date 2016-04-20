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

//Global logger declaration
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(g_log, src::severity_logger<severity_level>)

BOOST_LOG_ATTRIBUTE_KEYWORD(severity,  "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(program,   "Program",  std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(thread_id, "ThreadID", attrs::current_thread_id::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(process_name, "ProcessName", std::string)

int init_log(const char* log_dir, const char* log_name) {

    char path_name[MAX_PATH_LEN];

    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
    
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();
    
    snprintf(path_name, MAX_PATH_LEN, "%s/%s.log", log_dir, log_name);
    path_name[MAX_PATH_LEN-1] = '\0';

    sink->locked_backend()->add_stream(
            boost::make_shared< std::ofstream >(path_name));

    sink->set_formatter (
        expr::stream
//            << expr::attr< severity_level >("Severity")
            << severity << ": "
            << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")<< ":  "
            << program   << " * "
            << process_name <<" "
            << thread_id << " "
            << expr::smessage
    );


    logging::core::get()->add_sink(sink);

    logging::core::get()->add_global_attribute("Program",  attrs::constant<std::string>(log_name));
    logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
    logging::core::get()->add_global_attribute("ProcessName", attrs::current_process_name());


    logging::add_common_attributes();

    return 0;
}


}
}

int main() {
    glorey::log::init_log("log", "test");


    BOOST_LOG_FUNCTION();

    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(info)  << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error)   << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal)   << "A fatal severity message";


    DEBUG_LOG<<"test a debug log";
    WARNING_LOG<<"test a warning log";

    src::severity_logger<glorey::log::severity_level>& lg = glorey::log::g_log::get();
    BOOST_LOG_SEV(lg, glorey::log::debug) << "This is a debug severity record";
    BOOST_LOG_SEV(lg, glorey::log::trace) << "This is a trace severity record";

    return 0;
}
