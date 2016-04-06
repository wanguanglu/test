/**
* @file   example_logger_obj.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2016/04/06 15:23:12
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


BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::logger_mt);    
int main() {
    src::logger_mt& lg = my_logger::get();

    logging::record rec = lg.open_record();
    if (rec) {
        logging::record_ostream strm(rec);
        strm << "Hello, World!";
        strm.flush();
        lg.push_record(boost::move(rec));
    }

    BOOST_LOG(lg) << "Hello, World!";
}

