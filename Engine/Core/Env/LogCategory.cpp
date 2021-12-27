//
// Created by nunol on 12/22/2021.
//

#include "LogCategory.hpp"

void LogCategory::enablePriority(U32 priority) {
	if (priority > Logger::PRIORITY_E) {
		return;
	}
	enabledPriorities |= (1 << priority);
}

void LogCategory::disablePriority(U32 priority) {
	if (priority > Logger::PRIORITY_E) {
		return;
	}
	enabledPriorities &= ~(1 << priority);
}

bool LogCategory::isLogPriorityEnabled(U32 priority) {
	if (priority > Logger::PRIORITY_E) {
		return true;
	}
	return enabledPriorities & (1 << priority);
}
