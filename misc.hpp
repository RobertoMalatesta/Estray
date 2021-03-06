/**
 * @file misc.hpp
 */

/*
 * The following license applies to the code in this file:
 *
 * **************************************************************************
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * **************************************************************************
 *
 * Author: Dr. Rüdiger Berlich of Gemfony scientific UG (haftungsbeschraenkt)
 * See http://www.gemfony.eu for further information.
 *
 * This code is based on the Beast Websocket library by Vinnie Falco, as published
 * together with Boost 1.66 and above. For further information on Beast, see
 * https://github.com/boostorg/beast for the latest release, or download
 * Boost 1.66 or newer from http://www.boost.org .
 */

#ifndef EVALUATOR_MISC_HPP
#define EVALUATOR_MISC_HPP

// Standard headers go here
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <chrono>

// Boost headers go here
#include <boost/cast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/rfc6455.hpp>
#include <boost/exception/all.hpp>

// Our own heders go here
// Nothing

/******************************************************************************************/

void set_transfer_mode(boost::beast::websocket::stream<boost::asio::ip::tcp::socket>&);
const std::chrono::seconds DEFAULTPINGINTERVAL = std::chrono::seconds(5); // NOLINT

/** @brief A basetype used for all enums in the Evaluator */
using ENUMBASETYPE = unsigned int;

/** @brief Ids of the allowed commands for the Evaluator protocol */
enum class payload_command: ENUMBASETYPE {
  GETDATA = 0
  , NODATA = 1
  , COMPUTE = 2
  , RESULT = 3
  , ERROR = 4
  , NONE = 5
};

std::ostream &operator<<(std::ostream &, const payload_command &);
std::istream &operator>>(std::istream &, payload_command &);

/** @brief Indicates in what state of the ping submission we are */
enum class ping_state : ENUMBASETYPE {
	 CONNECTION_IS_ALIVE = 0
	 , SENDING_PING = 1
	 , CONNECTION_IS_STALE = 2
};

std::ostream &operator<<(std::ostream &, const ping_state &);
std::istream &operator>>(std::istream &, ping_state &);

/** @brief Indicates which payload type should be used */
enum class payload_type : ENUMBASETYPE {
	 container = 0
	 , sleep = 1
	 , command = 2
};

std::ostream &operator<<(std::ostream &, const payload_type &);
std::istream &operator>>(std::istream &, payload_type &);

/** @brief Creation of a fixed-width command-string to be transmitted between client and server */
std::string text_command_string(const std::string &cmd, std::size_t command_length);

/******************************************************************************************/

#endif /* EVALUATOR_MISC_HPP */
