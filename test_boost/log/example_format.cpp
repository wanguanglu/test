
#include <fstream>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

//[ example_tutorial_formatters_stream
void init1() {
    logging::add_file_log (
        keywords::file_name = "sample_%N.log",
        // This makes the sink to write log records that look like this:
        // 1: <normal> A normal severity message
        // 2: <error> An error severity message
        keywords::format = (
            expr::stream
                << expr::attr< unsigned int >("LineID")
                << ": <" << logging::trivial::severity
                << "> " << expr::smessage
        )
    );
}
//]

void init2() {
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

    sink->locked_backend()->add_stream (
        boost::make_shared< std::ofstream >("sample.log"));

    sink->set_formatter (
        expr::stream
        // line id will be written in hex, 8-digits, zero-filled
        << std::hex << std::setw(8) << std::setfill('0') << expr::attr< unsigned int >("LineID")
        << ": <" << logging::trivial::severity
        << "> " << expr::smessage
    );

    logging::core::get()->add_sink(sink);
}

//[ example_tutorial_formatters_stream_date_time
void init3() {
    logging::add_file_log (
        keywords::file_name = "sample_%N.log",
        // This makes the sink to write log records that look like this:
        // YYYY-MM-DD HH:MI:SS: <normal> A normal severity message
        // YYYY-MM-DD HH:MI:SS: <error> An error severity message
        keywords::format = (
            expr::stream
                << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << ": <" << logging::trivial::severity
                << "> " << expr::smessage
        )
    );
}
//]




int main(int, char*[]) {
    init2();
    logging::add_common_attributes();

    using namespace logging::trivial;
    src::severity_logger< severity_level > lg;

    BOOST_LOG_SEV(lg, trace) << "A trace severity message";
    BOOST_LOG_SEV(lg, debug) << "A debug severity message";
    BOOST_LOG_SEV(lg, info) << "An informational severity message";
    BOOST_LOG_SEV(lg, warning) << "A warning severity message";
    BOOST_LOG_SEV(lg, error) << "An error severity message";
    BOOST_LOG_SEV(lg, fatal) << "A fatal severity message";

    return 0;
}
