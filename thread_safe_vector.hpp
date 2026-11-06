// thread_safe_vector.hpp: starter code -- please refer to in-class example for ThreadSafeQueue
#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeVector
{
public:
//push an element to the back of the vector
void push_back(const T& value)
{
std::lock_guard<std::mutex> lock(mutex_);
vector_.push_back(value);
cond_var_.notify_one();
}

//wait until an element is available, then pop from the back
void wait_and_pop_back(T& value)
{
std::unique_lock<std::mutex> lock(mutex_);
cond_var_.wait(lock, [this] { return !vector_.empty(); });
value = vector_.back();
vector_.pop_back();
}

//try to pop from the back without blocking
bool try_pop_back(T& value)
{
std::lock_guard<std::mutex> lock(mutex_);
if (vector_.empty())
{
return false;
}
value = vector_.back();
vector_.pop_back();
return true;
}

//check if the vector is empty
bool empty() const
{
std::lock_guard<std::mutex> lock(mutex_);
return vector_.empty();
}

//get the current size of the vector
size_t size() const
{
std::lock_guard<std::mutex> lock(mutex_);
return vector_.size();
}

private:
mutable std::mutex mutex_;
std::condition_variable cond_var_;
std::vector<T> vector_;
};
