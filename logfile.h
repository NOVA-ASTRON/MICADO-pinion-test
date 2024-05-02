#ifndef LOGFILE_H
#define LOGFILE_H
#include <iostream>
#include <fstream>

typedef std::ofstream Stream;
typedef std::ofstream FileStream;
typedef std::ofstream StreamWriter;

class LogFile_t
{
public:
    LogFile_t();
            LogFile_t(std::string filename);

            void relocate(std::string filename);

            ~LogFile_t();

            std::string Write(std::string s);
            void sync();
            void close();
private:
    std::ofstream *log;
};

typedef LogFile_t * LogFile;

#endif // LOGFILE_H
