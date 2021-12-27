//
// Created by nunol on 12/22/2021.
//

#include "Logger.hpp"

#include <iostream>
#include <ctime>
#include <iomanip>

struct tm* localtime_r(time_t *_clock, struct tm *_result) {
	_localtime64_s(_result, _clock);
	return _result;
}

DefaultLogger::DefaultLogger(const Config& config)
	: m_config(config)
{}

void DefaultLogger::log(U32 priority, const std::string& tag, const std::string& message) {

	bool indent = false;
	auto time = std::chrono::system_clock::now().time_since_epoch();

	std::lock_guard<std::mutex> lock(m_lock);

	switch (priority) {
		case PRIORITY_V:
			std::cout << "\033[0;0m V \033[0m|";
			break;

		case PRIORITY_D:
			std::cout << "\033[34;0m D \033[0m|";
			break;

		case PRIORITY_I:
			std::cout << "\033[32;0m I \033[0m|";
			break;

		case PRIORITY_W:
			std::cout << "\033[45;0m W \033[0m|";
			break;

		case PRIORITY_E:
			std::cout << "\033[41;0m E \033[0m|";
			break;

		default:
			std::cout << " " << priority << " |";
	}

	if (m_config.timeFormat) {
		time_t seconds = std::chrono::duration_cast<std::chrono::seconds>(time).count();
		struct tm now{};
		localtime_r(&seconds, &now);
		std::cout << std::put_time(&now, m_config.timeFormat);
		indent = true;
	}

	if (m_config.printTicks) {
		auto ticks = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
		if(indent) {
			std::cout << " ";
		}
		std::cout << ticks;
		indent = true;
	}

	if (indent) {
		std::cout << "|";
	}

	if (message.empty()) {
		std::cout << " " << tag << std::endl;
	} else {
		std::cout << " " << tag << " : " << message << std::endl;
	}

}

void DefaultLogger::enablePriority(U32 priority) {
	if (priority > PRIORITY_E) {
		return;
	}
	m_config.logMask |= (1 << priority);
}

void DefaultLogger::disablePriority(U32 priority) {
	if (priority > PRIORITY_E) {
		return;
	}
	m_config.logMask &= ~(1 << priority);
}

bool DefaultLogger::isLogPriorityEnabled(U32 priority) {
	if (priority > PRIORITY_E) {
		return true;
	}
	return m_config.logMask & (1 << priority);
}
