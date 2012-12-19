/*
 * File:   TimeoutSerial.cpp
 * Author: Terraneo Federico
 * Distributed under the Boost Software License, Version 1.0.
 * Created on September 12, 2009, 3:47 PM
 *
 * v1.05: Fixed a bug regarding reading after a timeout (again).
 *
 * v1.04: Fixed bug with timeout set to zero
 *
 * v1.03: Fix for Mac OS X, now fully working on Mac.
 *
 * v1.02: Code cleanup, speed improvements, bug fixes.
 *
 * v1.01: Fixed a bug that caused errors while reading after a timeout.
 *
 * v1.00: First release.
 */

#include "TimeoutSerial.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

TimeoutSerial::TimeoutSerial(): io(), port(io), timer(io),
    timeout(posix_time::seconds(0)) {}

TimeoutSerial::TimeoutSerial(const std::string &devname, unsigned int baud_rate,
                             asio::serial_port_base::parity opt_parity,
                             asio::serial_port_base::character_size opt_csize,
                             asio::serial_port_base::flow_control opt_flow,
                             asio::serial_port_base::stop_bits opt_stop)
    : io(), port(io), timer(io), timeout(posix_time::seconds(0))
{
#ifdef CONNECTED
    open(devname, baud_rate, opt_parity, opt_csize, opt_flow, opt_stop);
#endif
}

void TimeoutSerial::open(const std::string &devname, unsigned int baud_rate,
                         asio::serial_port_base::parity opt_parity,
                         asio::serial_port_base::character_size opt_csize,
                         asio::serial_port_base::flow_control opt_flow,
                         asio::serial_port_base::stop_bits opt_stop)
{
#ifdef CONNECTED
    if (isOpen())
        close();
    port.open(devname);
    port.set_option(asio::serial_port_base::baud_rate(baud_rate));
    port.set_option(opt_parity);
    port.set_option(opt_csize);
    port.set_option(opt_flow);
    port.set_option(opt_stop);
#endif
}

bool TimeoutSerial::isOpen() const
{
#ifdef CONNECTED
    return port.is_open();
#else
    return true;
#endif
}

void TimeoutSerial::close()
{
#ifdef CONNECTED
    if (isOpen() == false) return;
    port.close();
#endif
}

void TimeoutSerial::setTimeout(const posix_time::time_duration &t)
{
#ifdef CONNECTED
    timeout = t;
#endif
}

void TimeoutSerial::write(const char *data, size_t size)
{
#ifdef CONNECTED
    asio::write(port, asio::buffer(data, size));
#endif
}

void TimeoutSerial::write(const std::vector<char> &data)
{
#ifdef CONNECTED
    asio::write(port, asio::buffer(&data[0], data.size()));
#endif
}

void TimeoutSerial::writeString(const std::string &s)
{
#ifdef CONNECTED
    asio::write(port, asio::buffer(s.c_str(), s.size()));
#endif
}

void TimeoutSerial::read(char *data, size_t size)
{
#ifdef CONNECTED
    if (readData.size() > 0) //If there is some data from a previous read
    {
        istream is(&readData);
        size_t toRead = min(readData.size(), size); //How many bytes to read?
        is.read(data, toRead);
        data += toRead;
        size -= toRead;
        if (size == 0) return; //If read data was enough, just return
    }

    setupParameters = ReadSetupParameters(data, size);
    performReadSetup(setupParameters);

    //For this code to work, there should always be a timeout, so the
    //request for no timeout is translated into a very long timeout
    if (timeout != posix_time::seconds(0)) timer.expires_from_now(timeout);
    else timer.expires_from_now(posix_time::hours(100000));

    timer.async_wait(boost::bind(&TimeoutSerial::timeoutExpired, this,
                                 asio::placeholders::error));

    result = resultInProgress;
    bytesTransferred = 0;
    for (;;)
    {
        io.run_one();
        switch (result)
        {
        case resultSuccess:
            timer.cancel();
            return;
        case resultTimeoutExpired:
            port.cancel();
            throw(timeout_exception("Timeout expired"));
        case resultError:
            timer.cancel();
            port.cancel();
            throw(boost::system::system_error(boost::system::error_code(),
                                              "Error while reading"));
            //if resultInProgress remain in the loop
        }
    }
#endif
}

std::vector<char> TimeoutSerial::read(size_t size)
{
#ifdef CONNECTED
    vector<char> result(size, '\0'); //Allocate a vector with the desired size
    read(&result[0], size); //Fill it with values
    return result;
#else
    return vector<char>();
#endif
}

std::string TimeoutSerial::readString(size_t size)
{
#ifdef CONNECTED
    string result(size, '\0'); //Allocate a string with the desired size
    read(&result[0], size); //Fill it with values
    return result;
#else
    return "";
#endif
}

std::string TimeoutSerial::readStringUntil(const std::string &delim)
{
#ifdef CONNECTED
    // Note: if readData contains some previously read data, the call to
    // async_read_until (which is done in performReadSetup) correctly handles
    // it. If the data is enough it will also immediately call readCompleted()
    setupParameters = ReadSetupParameters(delim);
    performReadSetup(setupParameters);

    //For this code to work, there should always be a timeout, so the
    //request for no timeout is translated into a very long timeout
    if (timeout != posix_time::seconds(0)) timer.expires_from_now(timeout);
    else timer.expires_from_now(posix_time::hours(100000));

    timer.async_wait(boost::bind(&TimeoutSerial::timeoutExpired, this,
                                 asio::placeholders::error));

    result = resultInProgress;
    bytesTransferred = 0;
    for (;;)
    {
        io.run_one();
        switch (result)
        {
        case resultSuccess:
        {
            timer.cancel();
            bytesTransferred -= delim.size(); //Don't count delim
            istream is(&readData);
            string result(bytesTransferred, '\0'); //Alloc string
            is.read(&result[0], bytesTransferred); //Fill values
            is.ignore(delim.size());//Remove delimiter from stream
            return result;
        }
        case resultTimeoutExpired:
            port.cancel();
            throw(timeout_exception("Timeout expired"));
        case resultError:
            timer.cancel();
            port.cancel();
            throw(boost::system::system_error(boost::system::error_code(),
                                              "Error while reading"));
            //if resultInProgress remain in the loop
        }
    }
#else
    return "";
#endif
}

TimeoutSerial::~TimeoutSerial() {}

void TimeoutSerial::performReadSetup(const ReadSetupParameters &param)
{
#ifdef CONNECTED
    if (param.fixedSize)
    {
        asio::async_read(port, asio::buffer(param.data, param.size), boost::bind(
                             &TimeoutSerial::readCompleted, this, asio::placeholders::error,
                             asio::placeholders::bytes_transferred));
    }
    else
    {
        asio::async_read_until(port, readData, param.delim, boost::bind(
                                   &TimeoutSerial::readCompleted, this, asio::placeholders::error,
                                   asio::placeholders::bytes_transferred));
    }
#endif
}

void TimeoutSerial::timeoutExpired(const boost::system::error_code &error)
{
#ifdef CONNECTED
    if (!error && result == resultInProgress) result = resultTimeoutExpired;
#endif
}

void TimeoutSerial::readCompleted(const boost::system::error_code &error,
                                  const size_t bytesTransferred)
{
#ifdef CONNECTED
    if (!error)
    {
        result = resultSuccess;
        this->bytesTransferred = bytesTransferred;
        return;
    }

    //In case a asynchronous operation is cancelled due to a timeout,
    //each OS seems to have its way to react.
#ifdef _WIN32
    if (error.value() == 995) return; //Windows spits out error 995
#elif defined(__APPLE__)
    if (error.value() == 45)
    {
        //Bug on OS X, it might be necessary to repeat the setup
        //http://osdir.com/ml/lib.boost.asio.user/2008-08/msg00004.html
        performReadSetup(setupParameters);
        return;
    }
#else //Linux
    if (error.value() == 125) return; //Linux outputs error 125
#endif

    result = resultError;
#endif
}
