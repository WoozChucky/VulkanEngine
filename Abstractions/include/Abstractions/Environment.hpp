//
// Created by nunol on 12/22/2021.
//

#ifndef THIS_FRAMEWORK_CORE_ENV_ENVIRONMENT_HPP_
#define THIS_FRAMEWORK_CORE_ENV_ENVIRONMENT_HPP_

#include "DataTypes.hpp"
#include "Logger.hpp"
#include "LogCategory.hpp"
#include <unordered_map>

/**
 * Class to manage application environment.<br>
 * Manage object counters, manage components, and do system health-checks.
 */
class Environment {
 private:

	static AtomicCounter m_objectsCount;
	static AtomicCounter m_objectsCreated;

	static thread_local S64 m_threadLocalObjectsCount;
	static thread_local S64 m_threadLocalObjectsCreated;

 private:
	static std::shared_ptr<Logger> m_logger;
	static void checkTypes();
 private:
	static std::unordered_map<std::string, std::unordered_map<std::string, void*>> m_components;
 public:

	/**
	 * Class representing system component.
	 * @tparam T - component type.
	 */
	template <typename T>
	class Component {
	 private:
		std::string m_type;
		std::string m_name;
		T m_object;
	 public:

		/**
		 * Constructor.
		 * @param name - component name.
		 * @param object - component object.
		 */
		Component(const std::string& name, const T& object)
			: m_type(typeid(T).name())
			, m_name(name)
			, m_object(object)
		{
			Environment::registerComponent(m_type, m_name, &m_object);
		}

		/**
		 * Constructor.
		 * @param object - component object.
		 */
		Component(const T& object)
			: Component("NoName", object)
		{}

		/**
		 * Non-virtual Destructor.
		 */
		~Component() {
			Environment::unregisterComponent(m_type, m_name);
		}

		/**
		 * Get object stored in the component.
		 * @return - object.
		 */
		T getObject() {
			return m_object;
		}

	};

 private:
	static void registerComponent(const std::string& typeName, const std::string& componentName, void* component);
	static void unregisterComponent(const std::string& typeName, const std::string& componentName);
	static void vlogFormatted(U32 priority, const std::string& tag, const char* message, va_list args);
 public:

	/**
	 * Initialize environment and do basic health-checks.
	 */
	static void init();

	/**
	 * Initialize environment and do basic health-checks.
	 * @param logger - system-wide logger.
	 */
	static void init(const std::shared_ptr<Logger>& logger);

	/**
	 * De-initialize environment and do basic health-checks.
	 * Check for memory leaks.
	 */
	static void destroy(int flag = 0);

	/**
	 * increment counter of objects.
	 */
	static void incObjects();

	/**
	 * decrement counter of objects.
	 */
	static void decObjects();

	/**
	 * Get count of objects currently allocated and stored in the memory.
	 * @return
	 */
	static S64 getObjectsCount();

	/**
	 * Get count of objects created for a whole system lifetime.
	 * @return - count of objects.
	 */
	static S64 getObjectsCreated();

	/**
	 * Same as `getObjectsCount()` but `thread_local`
	 * @return - count of objects. <br>
	 * *0 - if built with `-DOATPP_COMPAT_BUILD_NO_THREAD_LOCAL` flag*
	 */
	static S64 getThreadLocalObjectsCount();

	/**
	 * Same as `getObjectsCreated()` but `thread_local`
	 * @return - count of objects. <br>
	 * *0 - if built with `-DOATPP_COMPAT_BUILD_NO_THREAD_LOCAL` flag*
	 */
	static S64 getThreadLocalObjectsCreated();

	/**
	 * Set environment logger.
	 * @param logger - system-wide logger.
	 */
	static void setLogger(const std::shared_ptr<Logger>& logger);

	/**
	 * Gets the current environment logger
	 * @return - current logger
	 */
	static std::shared_ptr<Logger> getLogger();

	/**
	 * Call `Logger::log()`
	 * @param priority - log-priority channel of the message.
	 * @param tag - tag of the log message.
	 * @param message - message.
	 */
	static void log(U32 priority, const std::string& tag, const std::string& message);

	/**
	 * Format message and call `Logger::log()`<br>
	 * Message is formatted using `vsnprintf` method.
	 * @param priority - log-priority channel of the message.
	 * @param tag - tag of the log message.
	 * @param message - message.
	 * @param ... - format arguments.
	 */
	static void logFormatted(U32 priority, const std::string& tag, const char* message, ...);

	/**
	 * Format message and call `Logger::log()`<br>
	 * Message is formatted using `vsnprintf` method.
	 * @param priority - log-priority channel of the message.
	 * @param category - category of the log message.
	 * @param message - message.
	 * @param ... - format arguments.
	 */
	static void logFormatted(U32 priority, const LogCategory& category, const char* message, ...);

	/**
	 * Get component object by typeName.
	 * @param typeName - type name of the component.
	 * @return - pointer to a component object.
	 */
	static void* getComponent(const std::string& typeName);

	/**
	 * Get component object by typeName and componentName.
	 * @param typeName - type name of the component.
	 * @param componentName - component qualifier name.
	 * @return - pointer to a component object.
	 */
	static void* getComponent(const std::string& typeName, const std::string& componentName);

	/**
	 * Get ticks count in microseconds.
	 * @return - ticks count in microseconds.
	 */
	static S64 getMicroTickCount();
};

/**
 * Default oatpp assert method.
 * @param EXP - expression that must be `true`.
 */
#define GASSERT(EXP) \
if(!(EXP)) { \
  LOGE("\033[1mASSERT\033[0m[\033[1;31mFAILED\033[0m]", #EXP); \
  exit(EXIT_FAILURE); \
}

/**
 * Convenience macro to declare a logging category directly in a class header.
 * @param NAME - variable-name of the category which is later used to reference the category.
 */
#define OATPP_DECLARE_LOG_CATEGORY(NAME) \
  static LogCategory NAME;

/**
 * Convenience macro to implement a logging category directly in a class header.
 * @param NAME - variable-name of the category which is later used to reference the category.
 * @param TAG - tag printed with each message printed usig this category.
 * @param ENABLED - enable or disable a category (bool).
 */
#define OATPP_LOG_CATEGORY(NAME, TAG, ENABLED) \
  LogCategory NAME = LogCategory(TAG, ENABLED);

/**
 * Log message with &l:Logger::PRIORITY_V; <br>
 * *To disable this log compile oatpp with `#define OATPP_DISABLE_LOGV`*
 * @param TAG - message tag.
 * @param ...(1) - message.
 * @param ... - optional format parameter.
 */
#define LOGV(TAG, ...) \
  Environment::logFormatted(Logger::PRIORITY_V, TAG, __VA_ARGS__);

/**
 * Log message with &l:Logger::PRIORITY_D; <br>
 * *To disable this log compile oatpp with `#define OATPP_DISABLE_LOGD`*
 * @param TAG - message tag.
 * @param ...(1) - message.
 * @param ... - optional format parameter.
 */
#define LOGD(TAG, ...) \
  Environment::logFormatted(Logger::PRIORITY_D, TAG, __VA_ARGS__);

/**
 * Log message with &l:Logger::PRIORITY_I; <br>
 * *To disable this log compile oatpp with `#define OATPP_DISABLE_LOGI`*
 * @param TAG - message tag.
 * @param ...(1) - message.
 * @param ... - optional format parameter.
 */
#define LOGI(TAG, ...) \
  Environment::logFormatted(Logger::PRIORITY_I, TAG, __VA_ARGS__);

/**
 * Log message with &l:Logger::PRIORITY_W; <br>
 * *To disable this log compile oatpp with `#define OATPP_DISABLE_LOGW`*
 * @param TAG - message tag.
 * @param ...(1) - message.
 * @param ... - optional format parameter.
 */
#define LOGW(TAG, ...) \
  Environment::logFormatted(Logger::PRIORITY_W, TAG, __VA_ARGS__);

/**
 * Log message with &l:Logger::PRIORITY_E; <br>
 * *To disable this log compile oatpp with `#define OATPP_DISABLE_LOGE`*
 * @param TAG - message tag.
 * @param ...(1) - message.
 * @param ... - optional format parameter.
 */
#define LOGE(TAG, ...) \
  Environment::logFormatted(Logger::PRIORITY_E, TAG, __VA_ARGS__);

#endif //THIS_FRAMEWORK_CORE_ENV_ENVIRONMENT_HPP_
