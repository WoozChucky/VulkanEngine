//
// Created by nunol on 12/22/2021.
//

#ifndef THIS_FRAMEWORK_CORE_ENV_LOGGER_HPP_
#define THIS_FRAMEWORK_CORE_ENV_LOGGER_HPP_

#include "DataTypes.hpp"
#include <mutex>

class Logger {

 public:
	/**
	 * Log priority V-verbose.
	 */
	static constexpr U32 PRIORITY_V = 0;

	/**
	 * Log priority D-debug.
	 */
	static constexpr U32 PRIORITY_D = 1;

	/**
	 * Log priority I-Info.
	 */
	static constexpr U32 PRIORITY_I = 2;

	/**
	 * Log priority W-Warning.
	 */
	static constexpr U32 PRIORITY_W = 3;

	/**
	 * Log priority E-error.
	 */
	static constexpr U32 PRIORITY_E = 4;

 public:
	/**
	 * Virtual Destructor.
	 */
	virtual ~Logger() = default;

	/**
	 * Log message with priority, tag, message.
	 * @param priority - priority channel of the message.
	 * @param tag - tag of the log message.
	 * @param message - message.
	 */
	virtual void log(U32 priority, const std::string& tag, const std::string& message) = 0;

	/**
	 * Returns whether or not a priority should be logged/printed
	 * @param priority
	 * @return - true if given priority should be logged
	 */
	virtual bool isLogPriorityEnabled(U32 ) {
		return true;
	}

	/**
	 * Should return the maximum amount of bytes that should be allocated for a single log message
	 * @return - maximum buffer size
	 */
	virtual BufferSize getMaxFormattingBufferSize() {
		return 4096;
	}

};

/**
 * Default Logger implementation.
 */
class DefaultLogger : public Logger {
 public:
	/**
	 * Default Logger Config.
	 */
	struct Config {

		/**
		 * Constructor.
		 * @param tfmt - time format.
		 * @param printMicroTicks - show ticks in microseconds.
		 */
		Config(const char* tfmt, bool printMicroTicks, U32 initialLogMask)
			: timeFormat(tfmt)
			, printTicks(printMicroTicks)
			, logMask(initialLogMask)
		{}

		/**
		 * Time format of the log message.
		 * If nullptr then do not print time.
		 */
		const char* timeFormat;

		/**
		 * Print micro-ticks in the log message.
		 */
		bool printTicks;

		/**
		 * Log mask to enable/disable certain priorities
		 */
		U32 logMask;
	};
 private:
	Config m_config;
	std::mutex m_lock;
 public:

	/**
	 * Constructor.
	 * @param config - Logger config.
	 */
	DefaultLogger(const Config& config = Config(
		"%Y-%m-%d %H:%M:%S",
		false,
		(1 << PRIORITY_V) | (1 << PRIORITY_D) | (1 << PRIORITY_I) | (1 << PRIORITY_W) | (1 << PRIORITY_E)
	));

	/**
	 * Log message with priority, tag, message.
	 * @param priority - log-priority channel of the message.
	 * @param tag - tag of the log message.
	 * @param message - message.
	 */
	void log(U32 priority, const std::string& tag, const std::string& message) override;

	/**
	 * Enables logging of a priorities for this instance
	 * @param priority - the priority level to enable
	 */
	void enablePriority(U32 priority);

	/**
	 * Disables logging of a priority for this instance
	 * @param priority - the priority level to disable
	 */
	void disablePriority(U32 priority);

	/**
	 * Returns wether or not a priority should be logged/printed
	 * @param priority
	 * @return - true if given priority should be logged
	 */
	bool isLogPriorityEnabled(U32 priority) override;
};

#endif //THIS_FRAMEWORK_CORE_ENV_LOGGER_HPP_
