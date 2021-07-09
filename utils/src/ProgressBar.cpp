//
// Created by ltc on 2021/5/26.
//

#include "ProgressBar.h"

ProgressBar::ProgressBar(const string &title, int num) :curr(0,0,0,0,nullptr), updateTime(nullptr),
    title(title), num(num), count(0) {
    init();
}

void ProgressBar::start() {
    updateTime = ObjPool::allocate<Time>(0,0,0,500,this);
    auto arg = ObjPool::allocate<ProgressBar*>(this);
    TaskSystem::addTask(cycleTask, arg);
}

void ProgressBar::done() {
    if (count >= num) {
        draw();
    } else {
        count++;
    }
}

void ProgressBar::init() {
    registerEvent(EventEndCycle, nullptr);
    registerEvent(EventTickerTimeOut, handleTimeOut);
}

void ProgressBar::handleTimeOut(shared_ptr<void> arg) {
    ((ProgressBar*)(static_pointer_cast<Time>(arg))->ePtr)->draw();
}

void ProgressBar::stop() {
    cout << endl;
    auto e = ObjPool::allocate<Event>(EventEndCycle, nullptr);
    receiveEvent(e);
}

void ProgressBar::cycleInit() {
    TimeSystem::receiveEvent(EventTicker, updateTime);
    for (int i = 0; i < 60-title.size(); i++){
        cout << " ";
    }
    cout << title << endl;
    draw();
}

void ProgressBar::cycleClear() {
    TimeSystem::deleteTicker(updateTime);
}

void ProgressBar::draw() {
    curr += *updateTime;
    cout << "\r";
    cout << "[" << count << "/" << num;
    cout << "|" << fixed << setprecision(2) << double(count)/num*100 << "%]";
    int percent = count * 100 / num;
    for (int i = 0; i < percent; i++) {
        cout << "■";
    }
    for (int i = percent; i < 100; i++) {
        cout << " ";
    }
    int total = curr.ms + 1000*curr.s + 1000*60*curr.m + 1000*24*60*curr.h;
    double speed = double(count) / total;
    int remaining = (int)((num-count) / speed);
    int remainingH = remaining / 3600000;
    remaining %= 3600000;
    int remainingM = remaining / 60000;
    remaining %= 60000;
    int remainingS = remaining / 1000;
    cout << "[" << fixed << setprecision(2) << speed*1000 << "/s";
    cout << "|" << setw(2) << setfill('0') << curr.h << ":";
    cout << setw(2) << setfill('0') << curr.m << ":";
    cout << setw(2) << setfill('0') << curr.s;
    cout << "|" << setw(2) << setfill('0') << remainingH << ":";
    cout << setw(2) << setfill('0') << remainingM << ":";
    cout << setw(2) << setfill('0') << remainingS << "]";
    cout << flush;
    if (count == num) {
        stop();
    }
}
