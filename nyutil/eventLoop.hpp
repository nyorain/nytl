#pragma once

#include <nyutil/callback.hpp>
#include <nyutil/time.hpp>

#include <uv.h>
#include <vector>
#include <memory>

namespace nyutil
{


//eventLoop////////////////////////////////////////////////////////////////////7
class eventLoop
{
protected:
    uv_loop_t* loop_ {nullptr}; //no unique ptr, uv_default_loop may be used here
    bool owned_ {0};

public:
    inline eventLoop();
    inline ~eventLoop();

    inline void run(); //runs until stop is called
    inline void runOnce(); //blocks for next event
    inline void runNowait(); //does not block

    inline void stop();

    inline uv_loop_t& uvLoop() { return *loop_; };
    inline const uv_loop_t& uvLoop() const { return *loop_; };
};

eventLoop::eventLoop()
{
    loop_ = new uv_loop_t();
    owned_ = 1;

    uv_loop_init(loop_);
    loop_->data = this;
}

eventLoop::~eventLoop()
{
    stop();
    uv_loop_close(loop_);

    if(owned_)
        delete loop_;
}

void eventLoop::run()
{
    uv_run(loop_, UV_RUN_DEFAULT);
}

void eventLoop::runOnce()
{
    uv_run(loop_, UV_RUN_ONCE);
}

void eventLoop::runNowait()
{
    uv_run(loop_, UV_RUN_NOWAIT);
}

void eventLoop::stop()
{
    if(uv_loop_alive(loop_))
        uv_stop(loop_);
}



////////////////////////////////////////////////////////////////////////////////////
//eventSource/////////////////////////////////////////////////////////////////
//base
class eventSource
{
public:
    virtual ~eventSource() = default;

    virtual uv_handle_t& uvHandle() = 0;
    virtual const uv_handle_t& uvHandle() const = 0;

    virtual void enable(bool enabled) = 0;
    inline virtual bool enabled() const;

    inline eventLoop& getLoop() const { return *static_cast<eventLoop* const>(uvHandle().loop->data); }
};

bool eventSource::enabled() const
{
    return uv_is_active(&uvHandle());
}


///////////////////////////////////////////////////////////////////////
//idle///////////////////////////////////////
class idleEventSource : public eventSource
{
protected:
    static void cbfunc(uv_idle_t* idle){ static_cast<idleEventSource*>(idle->data)->onNotify(); }
    uv_idle_t handle_;

public:
    inline idleEventSource(eventLoop& loop, bool enable = 1);
    inline ~idleEventSource();

    inline virtual void enable(bool enbl) override;

    inline uv_idle_t& uvIdle() { return handle_; }
    inline const uv_idle_t& uvIdle() const { return handle_; }

    inline virtual uv_handle_t& uvHandle() override { return (uv_handle_t&)handle_; }
    inline virtual const uv_handle_t& uvHandle() const override { return (uv_handle_t&)handle_; }

    //callback
    callback<void()> onNotify {};
};

idleEventSource::idleEventSource(eventLoop& loop, bool enbl)
{
    uv_idle_init(&loop.uvLoop(), &handle_);
    handle_.data = this;

    if(enbl) enable(1);
}

idleEventSource::~idleEventSource()
{
    enable(0);
    //uv_close(uvHandle()); //?
}

void idleEventSource::enable(bool enbl)
{
    if(enbl && !enabled()) uv_idle_start(&handle_, &idleEventSource::cbfunc);
    else if(!enbl && enabled()) uv_idle_stop(&handle_);
}


/////////////////////////////////////////////////////////////////////////////////
//streamBase//////////////////////////////////////////
class streamEventSource : public eventSource
{
protected:
    inline static void cbAlloc(uv_handle_t* handle, size_t size, uv_buf_t* buffer);
    inline static void cbRead(uv_stream_t* stream, ssize_t read, const uv_buf_t* buf);

public:
    streamEventSource() = default;
    ~streamEventSource() = default;

    virtual uv_stream_t& uvStream() = 0;
    virtual const uv_stream_t& uvStream() const = 0;

    inline virtual uv_handle_t& uvHandle() override { return (uv_handle_t&)uvStream(); }
    inline virtual const uv_handle_t& uvHandle() const override { return (uv_handle_t&)uvStream(); }

    //callback
    callback<void(const std::string&)> onRead;
    callback<void()> onEOF;
};

void streamEventSource::cbAlloc(uv_handle_t* handle, size_t size, uv_buf_t* buffer)
{
    *buffer = uv_buf_init(new char[size], size);
}

void streamEventSource::cbRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    streamEventSource* src = static_cast<streamEventSource*>(stream->data);

    if(nread < 0)
    {
        if(nread == UV_EOF)
            src->onEOF();
    }
    else if(nread > 0)
    {
        src->onRead(buf->base);
    }

    if(buf->base) delete[] buf->base;
}


//////////////////////////////////////////////////////////////////
//pipe///////////////////////////////////////////////
class pipeEventSource : public streamEventSource
{
protected:
    uv_pipe_t handle_;

public:
    inline pipeEventSource(eventLoop& loop, uv_file file, bool enbld = 1);
    inline ~pipeEventSource();

    inline virtual void enable(bool enbl = 1) override;

    inline uv_pipe_t& uvPipe() { return handle_; }
    inline const uv_pipe_t& uvPipe() const { return handle_; }

    inline virtual uv_stream_t& uvStream() override { return (uv_stream_t&)handle_; };
    inline virtual const uv_stream_t& uvStream() const override { return (uv_stream_t&)handle_; };
};

pipeEventSource::pipeEventSource(eventLoop& loop, uv_file file, bool enbld)
{
    uv_pipe_init(&loop.uvLoop(), &handle_, 0);
    handle_.data = this;

    uv_pipe_open(&handle_, file);

    if(enbld) enable(1);
}

pipeEventSource::~pipeEventSource()
{
    enable(0);
    uv_close(&uvHandle(), nullptr);
}

void pipeEventSource::enable(bool enbl)
{
    if(enbl && !enabled()) uv_read_start(&uvStream(), &streamEventSource::cbAlloc, &streamEventSource::cbRead);
    else if(!enbl && enabled()) uv_read_stop(&uvStream());
}


////////////////////////////////////////////////////////////////////
//timer////////////////////////////////////////
class timerEventSource : public eventSource
{
protected:
    static void cbfunc(uv_timer_t* timer){ static_cast<timerEventSource*>(timer->data)->onNotify(); }

    uv_timer_t handle_;
    timeDuration time_;
    bool repeat_;

public:
    inline timerEventSource(eventLoop& loop, timeDuration time, bool repeat, bool enbl = 1);
    inline ~timerEventSource();

    //setInterval();
    //getInterval();

    inline uv_timer_t& uvTimer() { return handle_; }
    inline const uv_timer_t& uvTimer() const { return handle_; }

    inline virtual uv_handle_t& uvHandle() override { return (uv_handle_t&)handle_; }
    inline virtual const uv_handle_t& uvHandle() const override { return (uv_handle_t&)handle_; }

    inline virtual void enable(bool enbl) override;

    //callback
    callback<void()> onNotify;
};

timerEventSource::timerEventSource(eventLoop& loop, timeDuration time, bool repeat, bool enbl) : time_(time), repeat_(repeat)
{
    uv_timer_init(&loop.uvLoop(), &handle_);
    handle_.data = this;

    if(enbl) enable(1);
}

timerEventSource::~timerEventSource()
{
    enable(0);
}

void timerEventSource::enable(bool enbl)
{
    if(enbl && !enabled()) uv_timer_start(&handle_, &timerEventSource::cbfunc, time_.asNanoseconds(), repeat_ ? time_.asNanoseconds() : 0);
    else if(!enbl && enabled()) uv_timer_stop(&handle_);
}



//poll//////////////////////////////////////
class pollEventSource : public eventSource
{
protected:
    static void cbfunc(uv_poll_t* poll, int status, int event){ static_cast<pollEventSource*>(poll->data)->onNotify(status, event); }

    uv_poll_t handle_;
    int mask_;

public:
    inline pollEventSource(eventLoop& loop, int fd, int mask = UV_READABLE, bool enbl = 1);
    inline ~pollEventSource();

    inline uv_poll_t& uvPoll() { return handle_; }
    inline const uv_poll_t& uvPoll() const { return handle_; }

    inline virtual void enable(bool enbl) override;

    inline virtual uv_handle_t& uvHandle() override { return (uv_handle_t&) handle_; }
    inline virtual const uv_handle_t& uvHandle() const override { return (uv_handle_t&) handle_; }

    //callback
    callback<void(int, int)> onNotify;
};

pollEventSource::pollEventSource(eventLoop& loop, int fd, int mask, bool enbl) : mask_(mask)
{
    uv_poll_init(&loop.uvLoop(), &handle_, fd);
    handle_.data = this;

    if(enbl) enable(1);
}

pollEventSource::~pollEventSource()
{
    enable(0);
}

void pollEventSource::enable(bool enbl)
{
    if(enbl && !enabled()) uv_poll_start(&handle_, mask_, &pollEventSource::cbfunc);
    else if(!enbl && enabled()) uv_poll_stop(&handle_);
}

}
