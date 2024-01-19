#include <string>
#include <vector>   
#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <unordered_map>
#include <fstream>

enum class LOG_LEVEL{
    INFO,
    WARNING,
    ERROR
};

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

std::string mlib_read_file(const std::string& filepath){
    std::ifstream file(filepath);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (content.size() == 0){
        throw std::runtime_error("Error: " + filepath);
    }

    return content;
}

template <typename T>
bool mlib_write_to_file(const std::string& filepath, T *data, size_t size){
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()){
        return false;
    }
    file.write((char*)data, size);
    file.close();
    return true;
}


std::string mlib_get_file_extension(const std::string& filepath){
    std::string extension = "";
    for (int i = filepath.size() - 1; i >= 0; i--){
        if (filepath[i] == '.'){
            break;
        }
        extension += filepath[i];
    }
    std::reverse(extension.begin(), extension.end());
    return extension;
}


std::string mlib_get_args(int argc, char** argv){
    std::string args = "";
    for (int i = 1; i < argc; i++){
        args += argv[i];
        if (i != argc - 1){
            args += " ";
        }
    }
    return args;
}

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


    void addPath() {
        for (auto& arg : args){
            std::string extension = mlib_get_file_extension(arg);
            if (extension == "cpp" || extension == "c"){
                paths_[arg] = std::filesystem::last_write_time(arg);
            }
        }
        
    }

    void mlib_watch(){
        addPath();
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
    unsigned int interval_;  
};


