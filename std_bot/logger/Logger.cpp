#include "Logger.h"
#include <experimental/filesystem>
#include <time_include.h>

Logger * Logger::logger = NULL;
string Logger::logPath = "";
string Logger::fullLogPath = "";
ofstream Logger::log;
mutex Logger::locker;

using namespace std;

Logger::Logger(){
}

Logger::~Logger(){
    endInstance();
}

void Logger::endInstance(){
    log.close();

    experimental::filesystem::copy(fullLogPath, logPath + "/logs/last-log.txt", experimental::filesystem::copy_options::overwrite_existing);
}

Logger* Logger::getInstance(){
 
    static Logger *log; 
    return log;
}

void Logger::init(){
    logPath = experimental::filesystem::current_path();
    time_t currentTime = time(NULL);
    tm *localCurrentTime = localtime(&currentTime);

    // log.open(logPath + "/logs/log"  \
    //         + "_" + to_string(localCurrentTime->tm_hour) \
    //         + ":" + to_string(localCurrentTime->tm_min) \
    //         + ":" + to_string(localCurrentTime->tm_sec) \
    //         + "_" + to_string(localCurrentTime->tm_mday) \
    //         + "-" + to_string(localCurrentTime->tm_mon) \
    //         + "-" + to_string(localCurrentTime->tm_year%100) \
    //         + ".txt");

    fullLogPath = logPath + "/logs/log" + "_" + to_string(localCurrentTime->tm_hour) + ":" + to_string(localCurrentTime->tm_min) + ":" + to_string(localCurrentTime->tm_sec) + "_" + to_string(localCurrentTime->tm_mday) + "-" + to_string(localCurrentTime->tm_mon) + "-" + to_string(localCurrentTime->tm_year%100) + ".txt";
    log.open(fullLogPath);
}

void Logger::write(string str, string type){
    unique_lock<mutex> u_lock(locker, defer_lock);

    while(!u_lock.try_lock());

    time_t currentTime = time(NULL);
    tm *localCurrentTime = localtime(&currentTime);


    cout << "[" << (localCurrentTime->tm_hour<10?"0":"") << to_string(localCurrentTime->tm_hour) << ":" << (localCurrentTime->tm_min<10?"0":"") << to_string(localCurrentTime->tm_min) << ":" << (localCurrentTime->tm_sec<10?"0":"") << to_string(localCurrentTime->tm_sec) << "] " << type << str << endl; 
    log << "[" << (localCurrentTime->tm_hour<10?"0":"") << to_string(localCurrentTime->tm_hour) << ":" << (localCurrentTime->tm_min<10?"0":"") << to_string(localCurrentTime->tm_min) << ":" << (localCurrentTime->tm_sec<10?"0":"") << to_string(localCurrentTime->tm_sec) << "] " << type << str << endl; 
}