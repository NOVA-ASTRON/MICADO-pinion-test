#include "logfile.h"

#include <iostream>
#include <chrono>
#include <sstream>
#include <ios>

// clock stuff
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
using Clock = std::chrono::high_resolution_clock;
static std::string timePointToString(const Clock::time_point &tp, const std::string &format, bool withMs = true, bool utc = true)
{
    const Clock::time_point::duration tt = tp.time_since_epoch();
    const time_t durS = std::chrono::duration_cast<std::chrono::seconds>(tt).count();
    std::ostringstream ss;
    if (const std::tm *tm = (utc ? std::gmtime(&durS) : std::localtime(&durS))) {
        ss << std::put_time(tm, format.c_str());
        if (withMs) {
            const long long durMs = std::chrono::duration_cast<std::chrono::milliseconds>(tt).count();
            ss << std::setw(3) << std::setfill('0') << int(durMs - durS * 1000);
        }
    }
    // gmtime/localtime() returned null ?
    else {
        ss << "<FORMAT ERROR>";
    }
    return ss.str();
}

LogFile_t::LogFile_t()
{
    log=nullptr;
}

LogFile_t::LogFile_t(std::string filename)
{
    log = new std::ofstream;
    // append log.
    log->open(filename,std::ios_base::out|std::ios_base::app);
}

void LogFile_t::relocate(std::string filename)
{
    if (log){
        log->close();
        log->open(filename,std::ios_base::out|std::ios_base::app);
    }
}

LogFile_t::~LogFile_t()
{
    if(log){
        log->flush();
        log->close();
    }
}

std::string LogFile_t::Write(std::string s)
{
    const auto tp = Clock::now();
    std::stringstream line;
    line << timePointToString(tp, "%Z %Y-%m-%d %H:%M:%S.") << "," << s << std::endl;
    (*log) << line.str();
    return line.str();
}

void LogFile_t::sync()
{
    if(log){
        log->flush();
    }
}

void LogFile_t::close()
{
    if(log){
        log->flush();
        log->close();
        log=nullptr;
    }
}
