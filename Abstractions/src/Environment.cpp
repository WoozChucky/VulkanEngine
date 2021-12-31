//
// Created by nunol on 12/22/2021.
//

#include <Abstractions/Environment.hpp>
#include <cstdarg>
#include <csignal>


std::shared_ptr<Logger> Environment::m_logger;
std::unordered_map<std::string, std::unordered_map<std::string, void*>> Environment::m_components;

AtomicCounter Environment::m_objectsCount(0);
AtomicCounter Environment::m_objectsCreated(0);

thread_local S64 Environment::m_threadLocalObjectsCount = 0;
thread_local S64 Environment::m_threadLocalObjectsCreated = 0;


void Environment::init() {
	init(std::make_shared<DefaultLogger>());

	// Init Signal handling
	signal(SIGINT, destroy);
	signal(SIGILL, destroy);
	signal(SIGSEGV, destroy);
	signal(SIGTERM, destroy);
	signal(SIGBREAK, destroy);
	signal(SIGABRT, destroy);
}

void Environment::init(const std::shared_ptr<Logger>& logger) {

	m_logger = logger;

	checkTypes();

	m_objectsCount = 0;
	m_objectsCreated = 0;

	m_threadLocalObjectsCount = 0;
	m_threadLocalObjectsCreated = 0;

	if(!m_components.empty()) {
		throw std::runtime_error("[Environment::init()]: Error. Invalid state. Components were created before call to Environment::init()");
	}

}

void Environment::destroy(int flag){
	LOGD(__FUNCTION__, "Caught signal %d.", flag);
	LOGD(__FUNCTION__, "Shutting down system", flag);
	if(!m_components.empty()) {
		throw std::runtime_error("[Environment::destroy()]: Error. Invalid state. Leaking components");
	}
	m_logger.reset();
	if (flag == EXIT_FAILURE) {
		exit(EXIT_FAILURE);
	}
}

void Environment::checkTypes(){

	static_assert(sizeof(U8) == 1, "");
	static_assert(sizeof(S16) == 2, "");
	static_assert(sizeof(U16) == 2, "");
	static_assert(sizeof(S32) == 4, "");
	static_assert(sizeof(S64) == 8, "");
	static_assert(sizeof(U32) == 4, "");
	static_assert(sizeof(U64) == 8, "");
	static_assert(sizeof(R64) == 8, "");

	S32 vInt32 = ~S32(1);
	S64 vInt64 = ~S64(1);
	U32 vUInt32 = ~U32(1);
	U64 vUInt64 = ~U64(1);

	GASSERT(vInt32 < 0);
	GASSERT(vInt64 < 0);
	GASSERT(vUInt32 > 0);
	GASSERT(vUInt64 > 0);

}

void Environment::incObjects(){
	m_objectsCount ++;
	m_objectsCreated ++;

	m_threadLocalObjectsCount ++;
	m_threadLocalObjectsCreated ++;
}

void Environment::decObjects(){
	m_objectsCount --;

	m_threadLocalObjectsCount --;
}

S64 Environment::getObjectsCount(){
	return m_objectsCount;
}

S64 Environment::getObjectsCreated(){
	return m_objectsCreated;
}

S64 Environment::getThreadLocalObjectsCount(){
	return m_threadLocalObjectsCount;
}

S64 Environment::getThreadLocalObjectsCreated(){
	return m_threadLocalObjectsCreated;
}

void Environment::setLogger(const std::shared_ptr<Logger>& logger){
	m_logger = logger;
}

std::shared_ptr<Logger> Environment::getLogger() {
	return m_logger;
}

void Environment::log(U32 priority, const std::string& tag, const std::string& message) {
	if(m_logger != nullptr) {
		m_logger->log(priority, tag, message);
	}
}


void Environment::logFormatted(U32 priority, const LogCategory& category, const char* message, ...) {
	if (category.categoryEnabled && (category.enabledPriorities & (1 << priority))) {
		va_list args;
		va_start(args, message);
		vlogFormatted(priority, category.tag, message, args);
		va_end(args);
	}
}

void Environment::logFormatted(U32 priority, const std::string& tag, const char* message, ...) {
	va_list args;
	va_start(args, message);
	vlogFormatted(priority, tag, message, args);
	va_end(args);
}

void Environment::vlogFormatted(U32 priority, const std::string& tag, const char* message, va_list args) {
	// do we have a logger and the priority is enabled?
	if (m_logger == nullptr || !m_logger->isLogPriorityEnabled(priority)) {
		return;
	}
	// if we dont need to format anything, just print the message
	if(message == nullptr) {
		log(priority, tag, std::string());
		return;
	}
	// check how big our buffer has to be
	va_list argscpy;
	va_copy(argscpy, args);
	BufferSize allocsize = vsnprintf(nullptr, 0, message, argscpy) + 1;
	// alloc the buffer (or the max size)
	if (allocsize > m_logger->getMaxFormattingBufferSize()) {
		allocsize = m_logger->getMaxFormattingBufferSize();
	}
	auto buffer = std::unique_ptr<char[]>(new char[allocsize]);
	memset(buffer.get(), 0, allocsize);
	// actually format
	vsnprintf(buffer.get(), allocsize, message, args);
	// call (user) provided log function
	log(priority, tag, buffer.get());
}

void Environment::registerComponent(const std::string& typeName, const std::string& componentName, void* component) {
	auto& bucket = m_components[typeName];
	auto it = bucket.find(componentName);
	if(it != bucket.end()){
		throw std::runtime_error("[Environment::registerComponent()]: Error. Component with given name already exists: name='" + componentName + "'");
	}
	bucket[componentName] = component;
}

void Environment::unregisterComponent(const std::string& typeName, const std::string& componentName) {
	auto bucketIt = m_components.find(typeName);
	if(bucketIt == m_components.end() || bucketIt->second.empty()) {
		throw std::runtime_error("[Environment::unregisterComponent()]: Error. Component of given type doesn't exist: type='" + typeName + "'");
	}
	auto& bucket = bucketIt->second;
	auto componentIt = bucket.find(componentName);
	if(componentIt == bucket.end()) {
		throw std::runtime_error("[Environment::unregisterComponent()]: Error. Component with given name doesn't exist: name='" + componentName + "'");
	}
	bucket.erase(componentIt);
	if(bucket.empty()) {
		m_components.erase(bucketIt);
	}
}

void* Environment::getComponent(const std::string& typeName) {
	auto bucketIt = m_components.find(typeName);
	if(bucketIt == m_components.end() || bucketIt->second.empty()) {
		throw std::runtime_error("[Environment::getComponent()]: Error. Component of given type doesn't exist: type='" + typeName + "'");
	}
	auto bucket = bucketIt->second;
	if(bucket.size() > 1){
		throw std::runtime_error("[Environment::getComponent()]: Error. Ambiguous component reference. Multiple components exist for a given type: type='" + typeName + "'");
	}
	return bucket.begin()->second;
}

void* Environment::getComponent(const std::string& typeName, const std::string& componentName) {
	auto bucketIt = m_components.find(typeName);
	if(bucketIt == m_components.end() || bucketIt->second.empty()) {
		throw std::runtime_error("[Environment::getComponent()]: Error. Component of given type doesn't exist: type='" + typeName + "'");
	}
	auto bucket = bucketIt->second;
	auto componentIt = bucket.find(componentName);
	if(componentIt == bucket.end()) {
		throw std::runtime_error("[Environment::getComponent()]: Error. Component with given name doesn't exist: name='" + componentName + "'");
	}
	return componentIt->second;
}

S64 Environment::getMicroTickCount(){
	auto ms = std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::system_clock::now().time_since_epoch());
	return ms.count();
}
