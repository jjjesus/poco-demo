// AppLogger.cpp

#include "Poco/LoggingFactory.h"
#include "Poco/Logger.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"

#include "AppLogger.h"

using namespace Poco;
using namespace std;

CAppLogger::CAppLogger(void) {
	// We tell the vector how much elements we it'll have - its more efficient.
	mLoggers.resize(ELoggersCount);

	// Build the loggers
	mLoggers[ELoggerConsole] = &Logger::create("Log.Console",
			LoggingFactory::defaultFactory().createChannel("ConsoleChannel"),
			Message::PRIO_NOTICE);
	mLoggers[ELoggerFile] = &Logger::create("Log.File",
			LoggingFactory::defaultFactory().createChannel("FileChannel"),
			Message::PRIO_TRACE);
	//note that the priorty level set different. the file logger will have more masseges then the console logger

	// Set file channel path property (file & directory).
	mLoggers[ELoggerFile]->getChannel()->setProperty("path", "app.log");

	// Open all loggers.
	vector<Logger*>::iterator iterator;
	for (iterator = mLoggers.begin(); iterator != mLoggers.end(); iterator++) {
		if (*iterator != NULL) {
			(*iterator)->getChannel()->open();
		}
	}
}

CAppLogger::~CAppLogger(void) {
	// Close all loggers
	vector<Logger*>::iterator iterator;
	for (iterator = mLoggers.begin(); iterator != mLoggers.end(); iterator++) {
		if (*iterator != NULL) {
			(*iterator)->getChannel()->close();
		}
	}
}

void CAppLogger::Log(const std::string& inLogString,
		Poco::Message::Priority inPriority/* = Poco::Message::PRIO_INFORMATION*/) {
	Message msg;
	msg.setPriority(inPriority);
	msg.setText(inLogString);

	vector<Logger*>::iterator iterator;
	for (iterator = mLoggers.begin(); iterator != mLoggers.end(); iterator++) {
		if (*iterator != NULL) {
			(*iterator)->log(msg);
		}
	}
}

void CAppLogger::Log(const std::ostringstream& inLogString,
		Poco::Message::Priority inPriority/* = Poco::Message::PRIO_INFORMATION*/) {
	Log(inLogString.str(), inPriority);
}

