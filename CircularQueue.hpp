#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
<template T>
class CirculaQueue
{
    private:
        std::queue<T> q;
        mutable std::mutex m;
        std::condition_variable c;
    public:

}
