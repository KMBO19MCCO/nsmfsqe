//
// Created by assasinfil on 10.10.2022.
//

#ifndef NSMFSQE_TIMER_H
#define NSMFSQE_TIMER_H

#include <string>
#include <ctime>
#include <chrono>
#include <iostream>
#include <utility>


class Timer {
    std::string name;
    long long startTime;

public:

    explicit Timer(std::string name);

    void start();

    void stop();
};

#endif //NSMFSQE_TIMER_H
