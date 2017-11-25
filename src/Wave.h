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

// #define HAS_FFMPEG

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
	 * @param skip      skip seconds
	 * @param length	maximum length to load (after skip)
	 * @return Channels containing the wave channels
	 */
	static Channels   load(const std::string &,float skip=0,float length=1e+99);

	/**
	 * Load a wave file from the file system using libsndfile, avoiding
	 * copy operations.
	 * @param name		file system name of file
	 * @param target	Channel object to save the data in
	 * @param skip      skip seconds
	 * @param length	maximum length to load (after skip)
	 * @return Channels references containing the wave channels
	 */
	static Channels & load(const std::string &,Channels & target,float skip=0,float length=1e+99);

#ifdef HAS_FFMPEG
	/**
	 * Load a wave file from the file system using libavcodec.
	 * @param  name 	file system name of file
	 * @param skip      skip seconds
	 * @param length	maximum length to load (after skip)
	 * @return Channels containing the wave channels
	 */
	static Channels   loadFfmpeg(const std::string &,float skip=0,float length=1e+99);

	/**
	 * Load a wave file from the file system using libavcodec, avoiding
	 * copy operations.
	 * @param name		file system name of file
	 * @param target	Channel object to save the data in
	 * @param skip      skip seconds
	 * @param length	maximum length to load (after skip)
	 * @return Channels references containing the wave channels
	 */
	static Channels & loadFfmpeg(const std::string &,Channels & target,float skip=0,float length=1e+99);

	/**
	 * Save a multi-channel aac file to the file system using libffmpeg.
	 * The sample data is assumed to be in the range of [-32767,32767] and
	 * entries beyond are limited to the range.
	 * @param name		file system name of file
	 * @param channels	channels to be saved.
	 * @param bitrate	bitrate in bits per second
	 * @return	0 in case of success, 1 in case of error.
	 */
	static int      saveAac(const std::string &,Channels &,int);

	/**
	 * Save a single-channel aac file to the file system using libffmpeg.
	 * The sample data is assumed to be in the range of [-32767,32767] and
	 * entries beyond are limited to the range.
	 * @param name		file system name of file
	 * @param channel	channels to be saved.
	 * @param bitrate	bitrate in bits per second
	 * @return	0 in case of success, 1 in case of error.
	 */
	static int      saveAac(const std::string &,Channel &,int);

#endif // HAS_FFMPEG

	/**
	* Load a ascii wave file from the file system using libsndfile, avoiding
	* copy operations. This routine rescales the input to [-32000,32000].
	* @param name		file system name of file
	* @param samplerate sample rate of file
	* @param target	Channel object to save the data in
	* @param skip      skip seconds
	* @param length	maximum length to load (after skip)
	* @return Channels references containing the wave channels
    */
	static Channels & loadAscii(const std::string &name,int samplerate,Channels & target,float skip=0,float length=1e+99);

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
