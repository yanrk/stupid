/********************************************************
 * Description : single timer
 * Data        : 2013-08-09 22:21:48
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_SINGLE_TIMER_H
#define STUPID_BASE_SINGLE_TIMER_H


#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/thread/thread.h"

NAMESPACE_STUPID_BASE_BEGIN

class SingleTimer;

class STUPID_API ISingleTimerSink
{
public:
    virtual ~ISingleTimerSink();

public:
    virtual void on_timer(bool first_time, size_t index) = 0;
};

class STUPID_API SingleTimer : private Uncopy
{
public:
    SingleTimer();
    ~SingleTimer();

public:
    bool init(ISingleTimerSink * sink, size_t period);
    void exit();

public:
    static void run(SingleTimer * timer);

private:
    bool running();

private:
    ISingleTimerSink   * m_sink;
    size_t               m_period;
    size_t               m_index;
    bool                 m_first_time;
    volatile bool        m_running;
    Thread               m_thread;
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_SINGLE_TIMER_H
