#include <editor/managers/LogManager.h>

#include <chrono>
#include <iostream>

#include <format>

using namespace editor::managers;
using namespace std;

std::string LogManager::prefix() {
	std::string colours[6] = {
		colourToEscape(COLOURS::RED),
		colourToEscape(COLOURS::YELLOW),
		colourToEscape(COLOURS::GREEN),
		colourToEscape(COLOURS::BRIGHT_BLUE),
		colourToEscape(COLOURS::BLUE),
		colourToEscape(COLOURS::MAGENTA)
	};

	return format("[{}E{}D{}I{}T{}O{}R{}]    ", colours[messageIndex % 6], colours[(messageIndex + 1) % 6], colours[(messageIndex + 2) % 6], colours[(messageIndex + 3) % 6], colours[(messageIndex + 4) % 6], colours[(messageIndex + 5) % 6], colourToEscape(COLOURS::RESET));
}
unsigned char LogManager::messageIndex = 0;

void LogManager::debug(string message) {
	if (LogManager::enabledLevels::debug) {
		time_t time = std::time(0);
		tm localbuf = { 0 };
		localtime_s(&localbuf, &time);
		cout << prefix() << colourToEscape(COLOURS::BRIGHT_BLACK) << "[DEBUG]    " << put_time(&localbuf, "%Y/%m/%d %T") << " \t> " << message.c_str() << colourToEscape(COLOURS::RESET) << endl;
		messageIndex++;
		if (messageIndex >= 6) messageIndex = 0;
	}
}

void LogManager::info(string message) {
	if (LogManager::enabledLevels::info) {
		time_t time = std::time(0);
		tm localbuf = { 0 };
		localtime_s(&localbuf, &time);
		cout << prefix() << colourToEscape(COLOURS::BRIGHT_BLUE) << "[INFO]     " << put_time(&localbuf, "%Y/%m/%d %T") << " \t> " << message.c_str() << colourToEscape(COLOURS::RESET) << endl;
		messageIndex++;
		if (messageIndex >= 6) messageIndex = 0;
	}
}

void LogManager::warn(string message) {
	if (LogManager::enabledLevels::warn) {
		time_t time = std::time(0);
		tm localbuf = { 0 };
		localtime_s(&localbuf, &time);
		cout << prefix() << colourToEscape(COLOURS::YELLOW) << "[WARN]     " << put_time(&localbuf, "%Y/%m/%d %T") << " \t> " << message.c_str() << colourToEscape(COLOURS::RESET) << endl;
		messageIndex++;
		if (messageIndex >= 6) messageIndex = 0;
	}
}

void LogManager::error(string message) {
	if (LogManager::enabledLevels::error) {
		time_t time = std::time(0);
		tm localbuf = { 0 };
		localtime_s(&localbuf, &time);
		cout << prefix() << colourToEscape(COLOURS::RED) << "[ERROR]    " << put_time(&localbuf, "%Y/%m/%d %T") << " \t> " << message.c_str() << colourToEscape(COLOURS::RESET) << endl;
		messageIndex++;
		if (messageIndex >= 6) messageIndex = 0;
	}
}

void LogManager::critical(string message) {
	if (LogManager::enabledLevels::error) {
		time_t time = std::time(0);
		tm localbuf = { 0 };
		localtime_s(&localbuf, &time);
		cout << prefix() << colourToEscape(COLOURS::BRIGHT_RED) << "[CRITICAL] " << put_time(&localbuf, "%Y/%m/%d %T") << " \t> " << message.c_str() << colourToEscape(COLOURS::RESET) << endl;
		messageIndex++;
		if (messageIndex >= 6) messageIndex = 0;
	}
}

const char* LogManager::colourToEscape(COLOURS colour) {
	switch (colour) {
	case RESET:
		return "\033[0m";
	case BLACK:
		return "\x1B[30m";
	case RED:
		return "\x1B[31m";
	case GREEN:
		return "\x1B[32m";
	case YELLOW:
		return "\x1B[33m";
	case BLUE:
		return "\x1B[34m";
	case MAGENTA:
		return "\x1B[35m";
	case CYAN:
		return "\x1B[36m";
	case WHITE:
		return "\x1B[37m";
	case BRIGHT_BLACK:
		return "\x1B[90m";
	case BRIGHT_RED:
		return "\x1B[91m";
	case BRIGHT_GREEN:
		return "\x1B[92m";
	case BRIGHT_YELLOW:
		return "\x1B[93m";
	case BRIGHT_BLUE:
		return "\x1B[94m";
	case BRIGHT_MAGENTA:
		return "\x1B[95m";
	case BRIGHT_CYAN:
		return "\x1B[96m";
	case BRIGHT_WHITE:
		return "\x1B[97m";
	default:
		return "\033[0m";
	}
}