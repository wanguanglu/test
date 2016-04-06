/**
* @file   example_attribute.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2016/04/06 15:33:31
* @brief 
*  
**/

#include <cstddef>
#include <string>
#include <ostream>
#include <fstream>
#include <iomanip>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/core/null_deleter.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

enum severity_level {
    debug,
    trace,
    info,
    warning,
    error,
    fatal
};

BOOST_LOG_ATTRIBUTE_KEYWORD(line_id,  "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope,    "Scope", attrs::named_scope::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(timeline, "Timeline", attrs::timer::value_type)

void logging_function() {

    // The logger implicitly adds a source-specific attribute 'Severity'
    // of type 'severity_level' on construction

    src::severity_logger< severity_level > slg;

    logging::record rec = slg.open_record(keywords::severity = info);
    if (rec) {
        logging::record_ostream strm(rec);
        strm << "A regular message";
        strm.flush();
        slg.push_record(boost::move(rec));
    }
    
    BOOST_LOG_SEV(slg, info)    << "A regular message";
    BOOST_LOG_SEV(slg, warning) << "Something bad is going on but I can handle it";
    BOOST_LOG_SEV(slg, fatal)   << "Everything crumbles, shoot me now!";

}

void add_common_attributes() {

    boost::shared_ptr<logging::core> core = logging::core::get();
    core->add_global_attribute("LineID", attrs::counter<unsigned int>(1));
    core->add_global_attribute("TimeStamp", attrs::local_clock());
    core->add_global_attribute("Scope", attrs::named_scope());

}

void named_scope_logging() {

    BOOST_LOG_NAMED_SCOPE("named_scope_logging");
    src::severity_logger< severity_level > slg;
    BOOST_LOG_SEV(slg, info) << "Hello from the function named_scope_logging!";

}

void tagged_logging() {

    src::severity_logger< severity_level > slg;
    slg.add_attribute("Tag", attrs::constant< std::string >("My tag value"));
    BOOST_LOG_SEV(slg, info) << "Here goes the tagged record";

}


void timed_logging() {

    BOOST_LOG_SCOPED_THREAD_ATTR("Timeline", attrs::timer());
    src::severity_logger< severity_level > slg;
    BOOST_LOG_SEV(slg, info) << "Starting to time nested functions";
    logging_function();
    BOOST_LOG_SEV(slg, info) << "Stopping to time nested functions";

}

std::ostream&  operator<< (std::ostream& strm, severity_level level) {

    static const char* strings[] = {
        "debug",
        "trace",
        "info" ,
        "warning",
        "error",
        "fatal"
    };

    if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings)) {
        strm << strings[level];
    } else {
        strm << static_cast< int >(level);
    }

    return strm;

}

void init() {

    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;

    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();
    boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());

    // Add a stream to write log to
    sink->locked_backend()->add_stream(stream);

    sink->set_formatter (
        expr::stream
            << std::hex << std::setw(8) << std::setfill('0') << line_id << std::dec << std::setfill(' ')
            << ": <" << severity << ">\t"
            << "(" << scope << ") "
            << expr::if_(expr::has_attr(tag_attr))
               [
                    expr::stream << "[" << tag_attr << "] "
               ]
            << expr::if_(expr::has_attr(timeline))
               [
                    expr::stream << "[" << timeline << "] "
               ]
            << expr::smessage
    );

    logging::core::get()->add_sink(sink);

    // Add attributes
    logging::add_common_attributes();
    logging::core::get()->add_global_attribute("Scope", attrs::named_scope());
}


int main() {
    init();
    logging_function();
    add_common_attributes();
    named_scope_logging();
    tagged_logging();
    timed_logging();

    return 0;
}
