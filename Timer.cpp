//
// Created by assasinfil on 10.10.2022.
//

#include "Timer.h"

void Timer::start() {
    this->startTime = duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}

void Timer::stop() {
    auto endTime = duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << this->name << " elapsed: " << endTime - startTime << " ms" << std::endl;
}

Timer::Timer(std::string name) {
    this->name = std::move(name);
    this->startTime = 0;
}
