/**
* @file   test_boost_log.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2016/03/29 17:37:44
* @brief 
*  
**/


#include <fstream>
#include <boost/core/null_deleter.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;



//[ example_tutorial_file_advanced_no_callouts
void init() {
    boost::log::add_file_log("sample.log");

    boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::info);
}
//]

//[ example_tutorial_file_advanced
void init2() {
    boost::log::add_file_log(
        boost::log::keywords::file_name = "sample_%N.log",
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        boost::log::keywords::format = "[%TimeStamp%]: %Message%");

    boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::info);
}
//]

//[ example_tutorial_file_manual
void init3() {
    // Construct the sink
    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > text_sink;
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

    // Add a stream to write log to
    sink->locked_backend()->add_stream(boost::make_shared< std::ofstream >("sample.log"));

    // Register the sink in the logging core
    boost::log::core::get()->add_sink(sink);
}
//]

//[ example_tutorial_standard_stream
void init4() {
    // Construct the sink
    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > text_sink;
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

    boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());

    // Add a stream to write log to
    sink->locked_backend()->add_stream(stream);

    // Register the sink in the logging core
    boost::log::core::get()->add_sink(sink);
}
//]

void test() {
    init4();

    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(info)  << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error)   << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal)   << "A fatal severity message";

}

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, boost::log::sources::logger_mt);    
void test2() {
    boost::log::sources::logger_mt& lg = my_logger::get();

    boost::log::record rec = lg.open_record();
    if (rec) {
        boost::log::record_ostream strm(rec);
        strm << "Hello, World!";
        strm.flush();
        lg.push_record(boost::move(rec));
    }

    BOOST_LOG(lg) << "Hello, World!";
}

int main(int argc, char* argv[]) {


    return 0;
}
