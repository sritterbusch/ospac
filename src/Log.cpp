/**
 * @file		Log.cpp
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

#include <ctime>
#include <iomanip>
#include <stdio.h>

#include "Log.h"

std::ostream *Log::stdOutput=&std::cout;
std::ostream *Log::errOutput=&std::cerr;
TLogLevel     Log::logLevel=logINFO;
std::clock_t  Log::clockStart=clock();
bool          Log::showFunction=false;
bool		  Log::showRuntime=false;

std::ostream & Log::Get(std::string file,int line,TLogLevel level)
{
	std::ostream *o;

	if(level<=logERROR)
		o=errOutput;
	else
		o=stdOutput;

	if(showRuntime)
		(*o) << std::setw(7) << std::setprecision(2) << std::fixed
			 << int(clock()/double(CLOCKS_PER_SEC)*100)/100. << "\t";
	if(showFunction)
	{
		char number[20];
		std::string spaces="                         ";
		sprintf(number,"%d",line);
		std::string target=file.substr(7)+":"+number;
		target+=spaces.substr(target.size());

		(*o) << std::setw(25) << target << "\t";
	}
	return *o;
}
