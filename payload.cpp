/**
 * @file payload.cpp
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

#include "payload.hpp"

/******************************************************************************************/

BOOST_CLASS_EXPORT_IMPLEMENT(command_container) // NOLINT
BOOST_CLASS_EXPORT_IMPLEMENT(stored_number) // NOLINT
BOOST_CLASS_EXPORT_IMPLEMENT(container_payload) // NOLINT
BOOST_CLASS_EXPORT_IMPLEMENT(sleep_payload) // NOLINT

/******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************************/

void payload_base::process() {
	this->process_();
}

/******************************************************************************************/

bool payload_base::is_processed() {
	return this->is_processed_();
}

/******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************************/

std::string to_string(const payload_base* payload_ptr) {
	if(!payload_ptr) {
		throw std::runtime_error("to_string: payload_ptr is empty");
	}

	std::ostringstream oss(std::ios::out
#if defined(BINARYARCHIVE)
  | std::ios::binary);
#else
	);
#endif


	{
#if defined(BINARYARCHIVE)
		boost::archive::binary_oarchive oa(oss);
#elif defined(XMLARCHIVE)
		boost::archive::xml_oarchive oa(oss);
#elif defined(TEXTARCHIVE)
		boost::archive::text_oarchive oa(oss);
#else
		boost::archive::xml_oarchive oa(oss);
#endif
		oa << boost::serialization::make_nvp("payload_base", payload_ptr);
	} // archive and stream closed at end of scope

	return oss.str();
}

/******************************************************************************************/

payload_base *from_string(const std::string& descr) {
	payload_base * payload_ptr;

	std::istringstream iss(descr, std::ios::in
#if defined(BINARYARCHIVE)
	| std::ios::binary); // de-serialize
#else
	);
#endif

	{
#if defined(BINARYARCHIVE)
		boost::archive::binary_iarchive ia(iss);
#elif defined(XMLARCHIVE)
		boost::archive::xml_iarchive ia(iss);
#elif defined(TEXTARCHIVE)
		boost::archive::text_iarchive ia(iss);
#else
		boost::archive::xml_iarchive ia(iss);
#endif
		ia >> boost::serialization::make_nvp("payload_base", payload_ptr);
	} // archive and stream closed at end of scope

	return payload_ptr;
}

/******************************************************************************************/

std::string to_xml(const payload_base* payload_ptr) {
	if(!payload_ptr) {
		throw std::runtime_error("to_xml: payload_ptr is empty");
	}

	std::ostringstream oss; // serialize
	{
		boost::archive::xml_oarchive oa(oss);
		oa << boost::serialization::make_nvp("payload_base", payload_ptr);
	} // archive and stream closed at end of scope

	return oss.str();
}

/******************************************************************************************/

std::string to_binary(const payload_base* payload_ptr) {
	if(!payload_ptr) {
		throw std::runtime_error("to_string: payload_ptr is empty");
	}

	std::ostringstream oss; // serialize
	{
		boost::archive::binary_oarchive oa(oss);
		oa << boost::serialization::make_nvp("payload_base", payload_ptr);
	} // archive and stream closed at end of scope

	return oss.str();
}

/******************************************************************************************/

payload_base *from_binary(const std::string& descr) {
	payload_base * payload_ptr;

	std::istringstream iss(descr); // de-serialize
	{
		boost::archive::binary_iarchive ia(iss);
		ia >> boost::serialization::make_nvp("payload_base", payload_ptr);
	} // archive and stream closed at end of scope

	return payload_ptr;
}

/******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************************/

command_container::command_container(payload_command command)
	: m_command(command)
{ /* nothing */ }

/******************************************************************************************/

command_container::command_container(
	payload_command command
	, payload_base *payload_ptr
)
	: m_command(command)
	, m_payload_ptr(payload_ptr)
{ /* nothing */ }

/******************************************************************************************/

command_container::command_container(command_container&& cp) noexcept {
	m_command = cp.m_command; cp.m_command = payload_command::NONE;
	if(m_payload_ptr) delete m_payload_ptr;
	m_payload_ptr = cp.m_payload_ptr; cp.m_payload_ptr = nullptr;
}

/******************************************************************************************/

command_container::~command_container() {
	if(m_payload_ptr) delete m_payload_ptr;
}

/******************************************************************************************/

command_container& command_container::operator=(command_container&& cp) noexcept {
	m_command = cp.m_command; cp.m_command = payload_command::NONE;
	if(m_payload_ptr) delete m_payload_ptr;
	m_payload_ptr = cp.m_payload_ptr; cp.m_payload_ptr = nullptr;

	return *this;
}

/******************************************************************************************/

const command_container& command_container::reset(
	payload_command command
	, payload_base *payload_ptr
) {
	m_command = command;

	if(m_payload_ptr) {
		delete m_payload_ptr;
	}
	m_payload_ptr = payload_ptr;

	return *this;
}

/******************************************************************************************/

void command_container::set_command(payload_command command) {
	m_command = command;
}

/******************************************************************************************/

payload_command command_container::get_command() const noexcept {
	return m_command;
}

/******************************************************************************************/

void command_container::process() {
	if(m_payload_ptr) {
		m_payload_ptr->process();
	} else {
		throw std::runtime_error("command_container::process(): No processing possible as m_payload_ptr is empty.");
	}
}

/******************************************************************************************/

bool command_container::is_processed() {
	if(m_payload_ptr) {
		return m_payload_ptr->is_processed();
	} else {
		return false;
	}
}

/******************************************************************************************/

std::string command_container::to_string() const {
	// Reset the internal stream
#ifdef BINARYARCHIVE
	std::stringstream(std::ios::out | std::ios::binary).swap(m_stringstream);
#else
	std::stringstream(std::ios::out).swap(m_stringstream);
#endif

	{
#if defined(BINARYARCHIVE)
		boost::archive::binary_oarchive oa(m_stringstream);
#elif defined(XMLARCHIVE)
		boost::archive::xml_oarchive oa(m_stringstream);
#elif defined(TEXTARCHIVE)
		boost::archive::text_oarchive oa(m_stringstream);
#else
		boost::archive::xml_oarchive oa(m_stringstream);
#endif
		oa << boost::serialization::make_nvp("command_container", *this);
	} // archive and stream closed at end of scope

	return m_stringstream.str();
}

/******************************************************************************************/

std::string command_container::to_xml() const {
	// Reset the internal stream
	std::stringstream(std::ios::out).swap(m_stringstream);

	{
		boost::archive::xml_oarchive oa(m_stringstream);
		oa << boost::serialization::make_nvp("command_container", *this);
	} // archive and stream closed at end of scope

	return m_stringstream.str();
}

/******************************************************************************************/

void command_container::from_string(const std::string& descr) {
	command_container local_command_container{payload_command::NONE};

	// Reset the internal stream
#ifdef BINARYARCHIVE
	std::stringstream(descr, std::ios::in | std::ios::binary).swap(m_stringstream);
#else
	std::stringstream(descr, std::ios::in).swap(m_stringstream);
#endif

	{
#if defined(BINARYARCHIVE)
		boost::archive::binary_iarchive ia(m_stringstream);
#elif defined(XMLARCHIVE)
		boost::archive::xml_iarchive ia(m_stringstream);
#elif defined(TEXTARCHIVE)
		boost::archive::text_iarchive ia(m_stringstream);
#else
		boost::archive::xml_iarchive ia(m_stringstream);
#endif
		ia >> boost::serialization::make_nvp("command_container", local_command_container);
	} // archive and stream closed at end of scope

	// Move the data from local_command_container
	*this = std::move(local_command_container);
}

/******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************************/

stored_number::stored_number(double secret) : m_secret(secret)
{ /* nothing */ }

/******************************************************************************************/

std::shared_ptr<stored_number> stored_number::clone() {
    return std::shared_ptr<stored_number>(new stored_number(*this));
}

/******************************************************************************************/

double stored_number::value() const {
    return m_secret;
}

/******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************************/

container_payload::container_payload(const container_payload& cp) : payload_base(cp)
{
	m_data.clear();
	for(const auto& d: cp.m_data) {
		m_data.push_back(d->clone());
	}
}

/******************************************************************************************/

container_payload& container_payload::operator=(const container_payload& cp) {
	m_data.clear();
	for(const auto& d: cp.m_data) {
		m_data.push_back(d->clone());
	}

	return *this;
}

/******************************************************************************************/

void container_payload::container_payload::sort(){
    std::sort(
			m_data.begin()
			, m_data.end()
			, [](std::shared_ptr<stored_number> x, std::shared_ptr<stored_number> y) -> bool { return x->value() < y->value(); }
    );
}

/******************************************************************************************/

std::size_t container_payload::size() const {
    return m_data.size();
}

/******************************************************************************************/

std::shared_ptr<stored_number> container_payload::member(std::size_t pos) const {
    return m_data.at(pos);
}

/******************************************************************************************/

void container_payload::process_() {
	this->sort();
}

/******************************************************************************************/

bool container_payload::is_processed_() {
	return std::is_sorted(
		m_data.begin()
		, m_data.end()
		, [](const std::shared_ptr<stored_number>& x_ptr, const std::shared_ptr<stored_number>& y_ptr) -> bool {
			return (x_ptr->value() < y_ptr->value());
		}
	);
}

/******************************************************************************************/

void container_payload::add(std::shared_ptr<stored_number> p) {
	m_data.push_back(p);
}

/******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************************/

sleep_payload::sleep_payload(double sleep_time) : m_sleep_time(sleep_time)
{ /* default */ }

/******************************************************************************************/

void sleep_payload::process_() {
	std::this_thread::sleep_for(std::chrono::duration<double>(m_sleep_time));
}

/******************************************************************************************/

bool sleep_payload::is_processed_() {
	return true;
}

/******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************************/
