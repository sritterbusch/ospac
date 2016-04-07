/**
 * @file		Log.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		7.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Efficient logging stream
 *
 * Inspired by http://stackoverflow.com/questions/524524/creating-an-ostream
 *         and http://www.drdobbs.com/article/print?articleId=201804215&siteSectionName=cpp
 *
 */

#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <string>
#include <ctime>

/**
 * @def LOG(level)
 * @brief A macro for efficient creation of logging stream
 *
 * Use this macro for logging information that is only computed when
 * logging is activiated at given level: It expands to an if-statement
 * that computes the right hand side only if the logging level is reached.
 *
 * Example:
 * 	LOG(logDEBUG) << "test information " << expensiveFunction << std::endl;
 * expands to
 *  if(logDEBUG > LOG::getLoglevel())
 *  	; // do nothing
 *  else
 *  	LOG::GET(__FILE__,__LINE__,logDEBUG) << "test information " ...
 */

#define LOG(level) \
if (level > Log::getLoglevel()) ; \
else Log::Get(__FILE__,__LINE__,level)

/**
 * Logging Levels
 */
enum TLogLevel {logFATAL, logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

/**
 * @brief Logging class to specify output format and level. Use LOG(level) for logging
 */
class Log 
{
private:
	static std::ostream  *stdOutput;
	static std::ostream  *errOutput;
	static TLogLevel      logLevel;
	static std::clock_t	  clockStart;
	static bool           showFunction;
	static bool			  showRuntime;
	
public:
	/**
	 * Return suitable stream for logging level and write configured prefix
	 * @param file Source file
	 * @param line Source line
	 * @param level Logging Level
	 * @return output stream for logging
	 */
	static std::ostream & Get(std::string file, int line,TLogLevel level = logINFO);

	/**
	 * Set both error and standard logging output stream
	 * @param o output stream to use
	 */
	static void   	 	  setOutput(std::ostream & o)  { stdOutput=&o; errOutput=&o; }

	/**
	 * Set standard output stream for logging
	 * @param o output stream to use
	 */
	static void   	 	  setStandardOutput(std::ostream & o)  { stdOutput=&o; }

	/**
	 * Set error output stream for logging
	 * @param o output stream to use
	 */
	static void   	 	  setErrorOutput(std::ostream & o)  { errOutput=&o; }

	/**
	 * Set logging level to use
	 * @param level logging level
	 */
	static void   		  setLoglevel(TLogLevel level) { logLevel=level; }

	/**
	 * Should source file be displayed while logging
	 * @param show source file
	 */
	static void			  setShowFunction(bool show) { showFunction=show; }

	/**
	 * Should the running time be displayed while logging
	 * @param show logging time
	 */
	static void           setShowRuntime(bool show) { showRuntime=show; }

	/**
	 * request current logging level
	 * @return current logging level
	 */
	static TLogLevel   	  getLoglevel() { return logLevel; }
};

#endif /* LOG_H_ */
