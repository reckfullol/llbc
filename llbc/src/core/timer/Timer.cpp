// The MIT License (MIT)

// Copyright (c) 2013 lailongwei<lailongwei@126.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to 
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
// the Software, and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "llbc/common/Export.h"

#include "llbc/core/time/Time.h"
#include "llbc/core/variant/Variant.h"

#include "llbc/core/timer/Timer.h"
#include "llbc/core/timer/TimerData.h"
#include "llbc/core/timer/TimerScheduler.h"

__LLBC_NS_BEGIN

LLBC_Timer::LLBC_Timer(const LLBC_Delegate<void(LLBC_Timer *)> &timeoutDeleg,
                       const LLBC_Delegate<void(LLBC_Timer *)> &cancelDeleg,
                       LLBC_TimerScheduler *scheduler)
: _scheduler(scheduler)
, _timerData(nullptr)

, _data(nullptr)
, _timeoutDeleg(timeoutDeleg)
, _cancelDeleg(cancelDeleg)
{
}

LLBC_Timer::~LLBC_Timer()
{
    if (_timerData)
    {
        Cancel();
        if (--_timerData->refCount == 0)
            delete _timerData;
    }

    if (_data)
        delete _data;
}

LLBC_TimeSpan LLBC_Timer::GetDueTime() const
{
    return _timerData ?
        LLBC_TimeSpan::FromMillis(_timerData->dueTime) : LLBC_TimeSpan::zero;
}

LLBC_TimeSpan LLBC_Timer::GetPeriod() const
{
    return _timerData ?
        LLBC_TimeSpan::FromMillis(_timerData->period) : LLBC_TimeSpan::zero;
}

LLBC_TimerId LLBC_Timer::GetTimerId() const
{
    return _timerData ? _timerData->timerId : LLBC_INVALID_TIMER_ID;
}

sint64 LLBC_Timer::GetTimeoutTimes() const
{
    return _timerData ? _timerData->repeatTimes : 0;
}

LLBC_Variant &LLBC_Timer::GetTimerData()
{
    return *(_data ? _data : (_data = new LLBC_Variant));
}

const LLBC_Variant &LLBC_Timer::GetTimerData() const
{
    return _data ? *_data : LLBC_Variant::nil;
}

LLBC_Time LLBC_Timer::GetTimeoutTime() const
{
    return _timerData ?
        LLBC_Time::FromMillis(_timerData->handle) :
            LLBC_Time::utcBegin;
}

int LLBC_Timer::Schedule(const LLBC_TimeSpan &firstPeriod, const LLBC_TimeSpan &period)
{
    // Note: Allow reschedule in <OnCancel> event meth.
    // if (_timerData && _timerData->cancelling)
    // {
    //     LLBC_SetLastError(LLBC_ERROR_ILLEGAL);
    //     return LLBC_FAILED;
    // }

    const int cancelRet = Cancel();
    if (UNLIKELY(cancelRet != LLBC_OK))
        return cancelRet;

    if (UNLIKELY(!_scheduler))
    {
        _scheduler = reinterpret_cast<LLBC_TimerScheduler *>(
            __LLBC_GetLibTls()->coreTls.timerScheduler);
        if (UNLIKELY(!_scheduler))
        {
            LLBC_SetLastError(LLBC_ERROR_INVALID);
            return LLBC_FAILED;
        }
    }

    const sint64 firstPeriodInMillis = MAX(0ll, firstPeriod.GetTotalMillis());
    sint64 periodMillis = MAX(0ll, period.GetTotalMillis());
    if (periodMillis == 0ll)
        periodMillis = firstPeriodInMillis;

    return _scheduler->Schedule(this, firstPeriodInMillis, periodMillis);
}

int LLBC_Timer::Cancel()
{
    if (!_timerData || !_timerData->validate)
        return LLBC_OK;

    if (UNLIKELY(!_scheduler))
    {
        LLBC_SetLastError(LLBC_ERROR_INVALID);
        return LLBC_FAILED;
    }

    return _scheduler->Cancel(this);
}

bool LLBC_Timer::IsScheduling() const
{
    return (_timerData && _timerData->validate);
}

bool LLBC_Timer::IsTimeouting() const
{
    return _timerData ? _timerData->timeouting : false;
}

bool LLBC_Timer::IsCancelling() const
{
    return _timerData ? _timerData->cancelling: false;
}

LLBC_String LLBC_Timer::ToString() const
{
    return LLBC_String().format(
        "timerId:%llu, dueTime:%llums, period:%llums, timeoutTimes:%lld, scheduling:%s",
        GetTimerId(),
        GetDueTime().GetTotalMillis(),
        GetPeriod().GetTotalMillis(),
        GetTimeoutTimes(),
        IsScheduling()? "true" : "false");
}

__LLBC_NS_END
