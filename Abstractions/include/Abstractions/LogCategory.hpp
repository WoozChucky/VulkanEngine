//
// Created by nunol on 12/22/2021.
//

#ifndef THIS_FRAMEWORK_CORE_ENV_LOGCATEGORY_HPP_
#define THIS_FRAMEWORK_CORE_ENV_LOGCATEGORY_HPP_

#include "DataTypes.hpp"
#include "Logger.hpp"

/**
 * Describes a logging category (i.e. a logging "namespace")
 */
class LogCategory {
 public:
	/**
	 * Constructs a logging category.
	 * @param pTag - Tag of this logging category
	 * @param pCategoryEnabled - Enable or disable the category completely
	 * @param pEnabledPriorities - Bitmap of initially active logging categories.
	 */
	LogCategory(std::string pTag, bool pCategoryEnabled, U32 pEnabledPriorities = ((1<<Logger::PRIORITY_V) | (1<<Logger::PRIORITY_D) | (1<<Logger::PRIORITY_I) | (1<<Logger::PRIORITY_W) | (1<<Logger::PRIORITY_E)))
		: tag(std::move(pTag))
		, categoryEnabled(pCategoryEnabled)
		, enabledPriorities(pEnabledPriorities)
	{};

	/**
	 * The tag for this category
	 */
	const std::string tag;

	/**
	 * Generally enable or disable this category
	 */
	bool categoryEnabled;

	/**
	 * Priorities to print that are logged in this category
	 */
	U32 enabledPriorities;

	/**
	 * Enables logging of a priorities for this category
	 * @param priority - the priority level to enable
	 */
	void enablePriority(U32 priority);

	/**
	 * Disabled logging of a priorities for this category
	 * @param priority - the priority level to disable
	 */
	void disablePriority(U32 priority);

	/**
	 * Returns wether or not a priority of this category should be logged/printed
	 * @param priority
	 * @return - true if given priority should be logged
	 */
	bool isLogPriorityEnabled(U32 priority);
};

#endif //THIS_FRAMEWORK_CORE_ENV_LOGCATEGORY_HPP_
