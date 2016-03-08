/**
 * @file		Wave.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		5.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Wave file management via libsndfile
 */


#ifndef WAVE_H_
#define WAVE_H_

#include <string>
#include <vector>

#include "Channel.h"

/**
 * @brief Wave-file loading and saving via libsndfile
 */
class Wave
{
public:
	/**
	 * Load a wave file from the file system using libsndfile.
	 * @param  name 	file system name of file
	 * @return Channels containing the wave channels
	 */
	static Channels   load(const std::string &);

	/**
	 * Load a wave file from the file system using libsndfile, avoiding
	 * copy operations.
	 * @param name		file system name of file
	 * @param target	Channel object to save the data in
	 * @return Channels references containing the wave channels
	 */
	static Channels & load(const std::string &,Channels & target);

	/**
	 * Save a multi-channel wave file to the file system using libsndfile.
	 * The sample data is assumed to be in the range of [-32767,32767] and
	 * entries beyond are limited to the range.
	 * @param name		file system name of file
	 * @param channels	channels to be saved.
	 * @return	0 in case of success, 1 in case of error.
	 */
	static int      save(const std::string &,Channels &);

	/**
	 * Save a single-channel wave file to the file system using libsndfile.
	 * The sample data is assumed to be in the range of [-32767,32767] and
	 * entries beyond are limited to the range.
	 * @param name		file system name of file
	 * @param channel	channels to be saved.
	 * @return	0 in case of success, 1 in case of error.
	 */
	static int      save(const std::string &,Channel &);
};

#endif /* WAVE_H_ */
