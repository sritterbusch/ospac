/**
 * @file		Encode.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		29.3.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief 		Encoding to various formats using external tools
 */

#ifndef ENCODE_H_
#define ENCODE_H_

#include <String>

#include "Channel.h"

/**
 * @brief 		Encoding to various formats using external tools
 */

class Encode
{
public:
	enum Quality { LOW, STANDARD, HIGH, INSANE};

	/**
	 * Encode given audio segment to mp3 using an external lame encoder
	 * @param c			channels to encode
	 * @param filename	destination filename
	 * @param quality	quality preset (LOW, STANDARD, HIGH, INSANE)
	 * @param title		title of the track
	 * @param artist	artist of the track
	 * @param album		album/podcast of the track
	 * @param comment	comment/license of the track
	 * @param image		optional image file
	 * @param category	category (such as Speech)
	 * @param episode	track/episode number
	 * @param year		year of the recording/publication
	 * @return			program return code
	 */
	static int lame(Channels &c,
					std::string filename,
					Quality quality,
					std::string title,
					std::string artist,
					std::string album,
					std::string comment,
					std::string image,
					std::string category,
					std::string episode,
					std::string year);

	/**
	 * Encode given audio segment to ogg using an external oggenc encoder
	 * @param c			channels to encode
	 * @param filename	destination filename
	 * @param quality	quality preset (LOW, STANDARD, HIGH, INSANE)
	 * @param title		title of the track
	 * @param artist	artist of the track
	 * @param album		album/podcast of the track
	 * @param comment	comment/license of the track
	 * @param category	category (such as Speech)
	 * @param episode	track/episode number
	 * @return			program return code
	 */
	static int oggenc(Channels &c,
					std::string filename,
					Quality quality,
					std::string title,
					std::string artist,
					std::string album,
					std::string comment,
					std::string category,
					std::string episode);
};

#endif /* ENCODE_H_ */
