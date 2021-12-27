//
// Created by ltc on 2021/6/21.
//

#include "log/include/Log.h"

Log::Log(const string &path) {
    file.open(path, std::ios::out | std::ios::trunc);
}

void Log::log(string &&log) {
    mutex.lock();
    logQueue.push(std::move(log));
    mutex.unlock();
}

void Log::checkOut() {
    if (logQueue.empty()) {
        return;
    }
    vector<string> tmp;
    mutex.lock();
    while (!logQueue.empty()) {
        tmp.push_back(move(logQueue.front()));
        logQueue.pop();
    }
    mutex.unlock();
    for (auto& s : tmp) {
        file << s << std::endl;
    }
}
