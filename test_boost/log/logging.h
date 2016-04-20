/**
* @file   logging.h
* @author wanguanglu(wanguanglu@qq.com)
* @date   2016/04/07 10:30:53
* @brief 
*  
**/

#include <fstream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/named_scope.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/utility/formatting_ostream.hpp>

#include <boost/log/attributes/current_process_name.hpp>

#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>

#include <boost/log/support/date_time.hpp>


#include <cstdlib>
#include <string>
#include <utility>
#include <stdexcept>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/phoenix.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/value_ref.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>


namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;


namespace glorey {
namespace log {

const int MAX_PATH_LEN = 1024;

enum severity_level {
    debug,
    trace,
    notice,
    warning,
    error,
    fatal
};


template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
                std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl) {
    static const char* const str[] = {
        "DEBUG",
        "TRACE",
        "NOTICE",
        "WARNING",
        "ERROR",
        "FATAL"
    };
    
    if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str))) {
        strm << str[lvl];
    } else {
        strm << static_cast< int >(lvl);
    }
    return strm;
}


/**
 * @brief init log
 * @param log_file log file to output
 * @return errno
 *         0  OK
 *         -1 ERROR
 */
int init_log(const char* log_dir, const char* log_file);

#define DEBUG_LOG    BOOST_LOG_SEV(glorey::log::g_log::get(), glorey::log::debug)<<"["<<__FILE__<<":"<<__LINE__<<"] "
#define TRACE_LOG    BOOST_LOG_SEV(glorey::log::g_log::get(), glorey::log::trace)<<"["<<__FILE__<<":"<<__LINE__<<"] "
#define NOTICE_LOG   BOOST_LOG_SEV(glorey::log::g_log::get(), glorey::log::notice)<<"["<<__FILE__<<":"<<__LINE__<<"] "
#define WARNING_LOG  BOOST_LOG_SEV(glorey::log::g_log::get(), glorey::log::warning)<<"["<<__FILE__<<":"<<__LINE__<<"] "
#define ERROR_LOG    BOOST_LOG_SEV(glorey::log::g_log::get(), glorey::log::error)<<"["<<__FILE__<<":"<<__LINE__<<"] "
#define FATAL_LOG    BOOST_LOG_SEV(glorey::log::g_log::get(), glorey::log::fatal)<<"["<<__FILE__<<":"<<__LINE__<<"] "

}
}
