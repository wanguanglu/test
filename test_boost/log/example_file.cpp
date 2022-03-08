/**
 * @file   file_tutorial.cpp
 * @author wanguanglu(wanguanglu@qq.com)
 * @date   2016/04/06 15:06:55
 * @brief
 *
 **/

#include <boost/core/null_deleter.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <fstream>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

int main(int argc, char *argv[]) {

  logging::add_file_log("sample.log");

  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::info);

  BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
  BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
  BOOST_LOG_TRIVIAL(info) << "An informational severity message";
  BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
  BOOST_LOG_TRIVIAL(error) << "An error severity message";
  BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

  return 0;
}
