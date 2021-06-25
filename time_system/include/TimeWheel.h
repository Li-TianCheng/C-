//
// Created by ltc on 2021/5/14.
//

#ifndef TIMESYSTEM_TIMEWHEEL_H
#define TIMESYSTEM_TIMEWHEEL_H

#include <vector>
#include <unordered_set>
#include <unistd.h>
#include <sys/epoll.h>
#include "event_system/include/EventSystem.h"
#include "Time.h"

using std::vector;
using std::unordered_set;

class TimeWheel: public EventSystem {
public:
    TimeWheel();
    ~TimeWheel() override;
    void deleteTicker(const string& uuid);
    static void* timeWheelCycle(void* arg);
    TimeWheel(const TimeWheel&) = delete;
    TimeWheel(TimeWheel&&) = delete;
    TimeWheel& operator=(const TimeWheel&) = delete;
    TimeWheel& operator=(TimeWheel&&) = delete;
private:
    void init();
    void addTimeToWheel(EventKey e, const shared_ptr<Time>& t);
    void update();
    static void handleTimerEvent(const shared_ptr<void>& arg);
    static void handleTickerEvent(const shared_ptr<void>& arg);
    static void handleTimerTimeOut(const shared_ptr<void>& arg);
    static void handleTickerTimeOut(const shared_ptr<void>& arg);
    static void handleEndCycle(const shared_ptr<void>& arg);
private:
    unordered_set<string> toDelete;
    vector<queue<shared_ptr<Event>>> millisecond;
    vector<queue<shared_ptr<Event>>> second;
    vector<queue<shared_ptr<Event>>> minute;
    vector<queue<shared_ptr<Event>>> hour;
    volatile bool shutdown;
    int epollFd;
    int msIter;
    int sIter;
    int mIter;
    int hIter;
    Mutex mutex;
};

struct TimeWheelEventArg {
    shared_ptr<Time> t;
    shared_ptr<Time> nextTime;
    TimeWheelEventArg(const shared_ptr<Time>& t, const shared_ptr<Time>& nextTime): t(t), nextTime(nextTime) {};
};

#endif //TIMESYSTEM_TIMEWHEEL_H
