#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
<template T>
class CircularQueue
{
    private:
        std::queue<T> q;
        mutable std::mutex m;
        std::condition_variable c;
    public:
        CircularQueue(void): q(), m(), c()
        {

        }

        ~CircularQueue(void)
        {

        }

        void enqueue(T t)
        {
            std::lock_guard<std::mutex> lock(m);
            //unlock method is automatically excuted when destructor called
            
            q.push(t);
            c.notify_one();
        }

        void dequeue(void)
        {
            std::unique_lock<std::mutex> lock(m);
            //unique lock doesn't try lock when mutex is already locked.
            // but call unlock method when destructor called

            while(q.empty())
            {
                c.wait(lock);
            }

            T val = q.front();
            q.pop();
            return val;
        }

        int size(void)
        {
            return q.size();
        }
}
