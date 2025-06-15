#ifndef BINARY_LOGGER_H
#define BINARY_LOGGER_H

#include "Str.h"
#include "AppException.h"
#include <fstream>
#include <ctime>
#include <iostream>

class BinaryFile {
private:
    Str filename;
    bool isOpen;
    std::ofstream out;

public:
    BinaryFile(const Str& fname) : filename(fname), isOpen(false) {}

    bool open() {
        out.open(filename.return_array(), std::ios::binary | std::ios::app);
        isOpen = out.is_open();
        return isOpen;
    }

    bool write(const char* data) {
        if (!isOpen) return false;
        time_t now = time(0);
        out.write((char*)&now, sizeof(time_t)); 
        int len = strlen(data);
        out.write((char*)&len, sizeof(int));    
        out.write(data, len);                    
        return true;
    }

    void close() {
        if (isOpen) {
            out.close();
            isOpen = false;
        }
    }

    ~BinaryFile() {
        close();
    }
};

class BinaryLogger {
private:
    static BinaryLogger* instance;
    Str filePath;
    BinaryFile* logFile;

    BinaryLogger() {
        filePath = "log.bin";
        logFile = new BinaryFile(filePath);
        logFile->open();
    }

public:
    static BinaryLogger* getInstance() {
        if (!instance) {
            instance = new BinaryLogger();
        }
        return instance;
    }

    void logLogin(int userId) {
        Str log = Str("LOGIN: User ID = ").concat(userId);
        logFile->write(log.return_array());
    }

    void logLogout(int userId) {
        Str log = Str("LOGOUT: User ID = ").concat(userId);
        logFile->write(log.return_array());
    }

    void logError(AppException* ex) {
        Str log = Str("ERROR: ").concat(ex->getMessage());
        logFile->write(log.return_array());
    }

    void logInfo(const Str& message) {
        Str log = Str("INFO: ").concat(message);
        logFile->write(log.return_array());
    }

    ~BinaryLogger() {
        logFile->close();
        delete logFile;
    }
};
BinaryLogger* BinaryLogger::instance = nullptr;

#endif
