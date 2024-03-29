#ifndef LOGGER_H__
#define LOGGER_H__

#include <boost/log/trivial.hpp>

namespace mtp {

#define LOG_ERROR BOOST_LOG_TRIVIAL(error)
#define LOG_INFO BOOST_LOG_TRIVIAL(info)
#define LOG_DEBUG BOOST_LOG_TRIVIAL(debug)
#define LOG_TRACE BOOST_LOG_TRIVIAL(trace)
#define LOG_WARNING BOOST_LOG_TRIVIAL(warning)
#define LOG_FATAL BOOST_LOG_TRIVIAL(fatal)

}

#endif