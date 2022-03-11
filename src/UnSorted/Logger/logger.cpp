#include "logger.h"

#include <ctime>
#include <iomanip>
#include <iostream>

#include "../Config/config.h"

namespace fs = std::filesystem;

Logger::Logger() {
}
Logger::~Logger() {
    for (auto& [name, out] : _ofstreams) {
        IInfo(name, "Logger", "Log closed.");
        out.close();
    }
}

void Logger::IPrintTime(std::ostream& out) {
    time_t now = time(0);
    tm* current_time = localtime(&now);
    out << '[' << current_time->tm_year + 1900 << '.';
    IPrintValue(out, current_time->tm_mon + 1);
    out << '.';
    IPrintValue(out, current_time->tm_mday);
    out << " | ";
    IPrintValue(out, current_time->tm_hour);
    out << '.';
    IPrintValue(out, current_time->tm_min);
    out << '.';
    IPrintValue(out, current_time->tm_sec);
    out << "] ";
}
void Logger::IPrintValue(std::ostream& out, const int& value) {
    if (value > 9) {
        out << value;
        return;
    }
    out << '0' << value;
}

void Logger::IOpen(const fs::path& path_to_file, const std::string& log_name) {
    if (_openned_files.contains(path_to_file.string())) {
        return;
    }
    _openned_files.insert(path_to_file.string());
    _ofstreams[log_name].open(path_to_file);
}
void Logger::IClose(const std::string& log_name) {
    IInfo(log_name, "Logger", "Log closed.");
    _ofstreams[log_name].close();
}
void Logger::IWrite(const std::string& log_name, const std::string& author, const std::string& massage, const std::string& message_type) {
    IPrintTime(_ofstreams[log_name]);
    _ofstreams[log_name] << std::setw(MAX_MESSAGE_TYPE_LENGTH) << ("[" + message_type + "]") << " " << author << ": " << massage << std::endl;
}
void Logger::IError(const std::string& log_name, const std::string& author, const std::string& massage) {
    IWrite(log_name, author, massage, "ERROR");
}
void Logger::IInfo(const std::string& log_name, const std::string& author, const std::string& massage) {
    IWrite(log_name, author, massage, "INFO");
}
void Logger::IStage(const std::string& log_name, const std::string& author, const std::string& massage) {
    IWrite(log_name, author, massage, "STAGE");
}
void Logger::IWarning(const std::string& log_name, const std::string& author, const std::string& massage) {
    IWrite(log_name, author, massage, "WARNING");
}
void Logger::IFatal(const std::string& log_name, const std::string& author, const std::string& massage) {
    IWrite(log_name, author, massage, "FATAL");
    IClose(log_name);
}

Logger& Logger::Get() {
    static Logger logger;
    return logger;
}

void Logger::Open(const fs::path& path_to_file, const std::string& log_name) {
    Get().IOpen(path_to_file, log_name);
}
void Logger::Close(const std::string& log_name) {
    Get().IClose(log_name);
}
void Logger::Write(const std::string& log_name, const std::string& author, const std::string& massage, const std::string& message_type) {
    Get().IWrite(log_name, author, massage, message_type);
}
void Logger::Error(const std::string& log_name, const std::string& author, const std::string& massage) {
    Get().IError(log_name, author, massage);
}
void Logger::Info(const std::string& log_name, const std::string& author, const std::string& massage) {
    Get().IInfo(log_name, author, massage);
}
void Logger::Stage(const std::string& log_name, const std::string& author, const std::string& massage) {
    Get().IStage(log_name, author, massage);
}
void Logger::Warning(const std::string& log_name, const std::string& author, const std::string& massage) {
    Get().IWarning(log_name, author, massage);
}
void Logger::Fatal(const std::string& log_name, const std::string& author, const std::string& massage) {
    Get().IFatal(log_name, author, massage);
}