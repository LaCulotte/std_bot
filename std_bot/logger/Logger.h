#ifndef LOGGER_H
#define LOGGER_H

#include "std_include.h"
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include "thread_include.h"

#define LOG_ERROR "Error - "
#define LOG_IMPORTANT_WARNING "Important warning - "
#define LOG_WARNING "Warning - "
#define LOG_INFO "Info - "
#define LOG_DEBUG "Debug - "

using namespace std;
class Logger{
public:

    static Logger* getInstance();
    static void endInstance();

    static void write(string str, string type = LOG_DEBUG);

    static void init();

    static string fullLogPath;
private:
    Logger();
    ~Logger();

    static Logger* logger;
    static string logPath;

    static ofstream log;

    static mutex locker;
};

#endif