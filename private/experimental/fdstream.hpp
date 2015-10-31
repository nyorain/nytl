#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <streambuf>
#include <cstddef>
#include <unistd.h>

namespace nytl
{

//http://stackoverflow.com/questions/13541313/handle-socket-descriptors-like-file-descriptor-fstream-c-linux
class fdbuf : public std::streambuf
{
private:
    enum { bufsize = 1024 };
    char outbuf_[bufsize];
    char inbuf_[bufsize + 16 - sizeof(int)];
    int  fd_;

public:
    typedef std::streambuf::traits_type traits_type;

    fdbuf(int fd);
    ~fdbuf();
    void open(int fd);
    void close();

protected:
    int overflow(int c);
    int underflow();
    int sync();
};

fdbuf::fdbuf(int fd)
  : fd_(-1) {
    this->open(fd);
}

fdbuf::~fdbuf() {
    this->close();
}

void fdbuf::open(int fd) {
    this->close();
    this->fd_ = fd;
    this->setg(this->inbuf_, this->inbuf_, this->inbuf_);
    this->setp(this->outbuf_, this->outbuf_ + bufsize - 1);
}

void fdbuf::close() {
    if (!(this->fd_ < 0)) {
        this->sync();
        ::close(this->fd_);
    }
}

int fdbuf::overflow(int c) {
    if (!traits_type::eq_int_type(c, traits_type::eof())) {
        *this->pptr() = traits_type::to_char_type(c);
        this->pbump(1);
    }
    return this->sync() == -1
        ? traits_type::eof()
        : traits_type::not_eof(c);
}

int fdbuf::sync() {
    if (this->pbase() != this->pptr()) {
        std::streamsize size(this->pptr() - this->pbase());
        std::streamsize done(::write(this->fd_, this->outbuf_, size));
        if (0 < done) {
            std::copy(this->pbase() + done, this->pptr(), this->pbase());
            this->setp(this->pbase(), this->epptr());
            this->pbump(size - done);
        }
    }
    return this->pptr() != this->epptr()? 0: -1;
}

int fdbuf::underflow()
{
    if (this->gptr() == this->egptr()) {
        std::streamsize pback(std::min(this->gptr() - this->eback(),
                                       std::ptrdiff_t(16 - sizeof(int))));
        std::copy(this->egptr() - pback, this->egptr(), this->eback());
        int done(::read(this->fd_, this->eback() + pback, bufsize));
        this->setg(this->eback(),
                   this->eback() + pback,
                   this->eback() + pback + std::max(0, done));
    }
    return this->gptr() == this->egptr()
        ? traits_type::eof()
        : traits_type::to_int_type(*this->gptr());
}


////
class fdostream : public std::ostream
{
protected:
    fdbuf buffer_;

public:
    fdostream(int fd) : std::ostream(), buffer_(fd)
    {
        this->init(&buffer_);
    }
};

////
class fdistream : public std::istream
{
protected:
    fdbuf buffer_;

public:
    fdistream(int fd) : std::istream(), buffer_(fd)
    {
        this->init(&buffer_);
    }
};

////
class fdiostream : public std::iostream
{
protected:
    fdbuf buffer_;

public:
    fdiostream(int fd) : std::iostream(), buffer_(fd)
    {
        this->init(&buffer_);
    }
};


}
