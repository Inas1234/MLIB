#include <string>
#include <vector>   
#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <unordered_map>

enum class LOG_LEVEL{
    INFO,
    WARNING,
    ERROR
};

class Cmd{
public:
    Cmd() : interval_(1000) {}


    template <typename... Args>
    void mlib_append(Args... args) {
        (this->args.push_back(args), ...);
    }


    void mlib_execute(){
        try
        {
            if (args.size() == 0){
                std::cerr << "No command to execute" << std::endl;
                return;
            }
            std::string command = "";
            for (auto& arg : args){
                command += arg + " ";
            }
            int res = system(command.c_str());
            if (res != 0){
                throw std::runtime_error("Error: " + command);
            }

            mlib_log("Command executed successfully");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    void mlib_rebuild(){
        REBUILD = true;
    }

    void mlib_log(std::string message, LOG_LEVEL level = LOG_LEVEL::INFO){
        std::string level_str = "";
        switch (level)
        {
        case LOG_LEVEL::INFO:
            level_str = "INFO";
            break;
        case LOG_LEVEL::WARNING:
            level_str = "WARNING";
            break;
        case LOG_LEVEL::ERROR:
            level_str = "ERROR";
            break;
        default:
            break;
        }
        std::cout << "[" << level_str << "] " << message << std::endl;
    }    

    void addPath(const std::string& path) {
        paths_[path] = std::filesystem::last_write_time(path);
    }

    void mlib_watch(){
        std::cout << "Watching..." << std::endl;
        while (running_){
            std::this_thread::sleep_for(std::chrono::milliseconds(interval_));
            if (REBUILD){
                mlib_execute();
                REBUILD = false;
            }
            else{
                for (auto& [path, last_write_time] : paths_){
                    auto last_write_time_now = std::filesystem::last_write_time(path);
                    if (last_write_time_now != last_write_time){
                        last_write_time = last_write_time_now;
                        mlib_log("File changed: " + path);
                        mlib_execute();
                    }
                }
            }
        }
    }

private:
    std::vector<std::string> args;   
    bool REBUILD = false;
    std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
    bool running_ = true;
    unsigned int interval_;  // In milliseconds

    
};