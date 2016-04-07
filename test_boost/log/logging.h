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

#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>

#include <boost/log/support/date_time.hpp>


namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;


namespace glorey {
namespace log {

const int MAX_PATH_LEN = 1024;


/**
 * @brief init log
 * @param log_file log file to output
 * @return errno
 *         0  OK
 *         -1 ERROR
 */
int init_log(const char* log_dir, const char* log_file);


}
}
