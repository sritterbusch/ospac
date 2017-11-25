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

#include <string>

#include "Channel.h"
#include "Wave.h" // Nur fuer FFMPEG-Define und saveAac(...)

/**
 * @brief 		Encoding to various formats using external tools
 */

class Encode
{
public:
	/**
	 * General quality setting for all encoding formats
	 */
	enum QualitySetting { LOW, STANDARD, HIGH, INSANE};
private:
	Channels    &channels;
	std::string title;
	std::string artist;
	std::string album;
	std::string comment;
	std::string category;
	std::string episode;
	std::string year;
	std::string image;
	QualitySetting     quality;
	#ifdef HAS_FFMPEG
	int                aacBitrate;
	#endif

public:
	/**
	 * Builder pattern constructor from channels to encode.
	 * @param aChannels
	 */
	Encode(Channels &aChannels)
		: channels(aChannels),
		  title(""),artist(""),album(""),
		  comment("Encoded by ospac.net"),
		  category("Speech"),episode(""),
		  year(""),image(""),
		  quality(STANDARD)
#ifdef HAS_FFMPEG
		  ,aacBitrate(64000)
#endif	
	{

	}

	/**
	 * Set title meta tag of encoded file to given title
	 * @param aTitle title to be set
	 * @return modified builder object
	 */
	Encode & Title(std::string aTitle) { title=aTitle; return *this; }
	/**
	 * set artist meta tag of encoded file to given artist
	 * @param aArtist artist to be set
	 * @return modified builder object
	 */
	Encode & Artist(std::string aArtist) { artist=aArtist; return *this; }
	/**
	 * set album meta tag of encoded file to given album
	 * @param aAlbum album to be set
	 * @return modified builder object
	 */
	Encode & Album(std::string aAlbum) { album=aAlbum; return *this; }
	/**
	 * set comment meta tag of encoded file to given comment
	 * @param aComment comment to be set
	 * @return modified builder object
	 */
	Encode & Comment(std::string aComment) { comment=aComment; return *this; }
	/**
	 * set category meta tag of encoded file to given category
	 * @param aCategory category to be set
	 * @return modified builder object
	 */
	Encode & Category(std::string aCategory) { category=aCategory; return *this; }
	/**
	 * set episode meta tag of encoded file to given episode
	 * @param aEpisode episode to be set
	 * @return modified builder object
	 */
	Encode & Episode(std::string aEpisode) { episode=aEpisode; return *this; }
	/**
	 * set year meta tag of encoded file to given year
	 * @param aYear year to be set
	 * @return modified builder object
	 */
	Encode & Year(std::string aYear) { year=aYear; return *this; }
	/**
	 * set image meta tag of encoded file to given image if possible
	 * @param aImage image to be set
	 * @return modified builder object
	 */
	Encode & Image(std::string aImage) { image=aImage; return *this; }
	/**
	 * set quality of encoding to given meta value
	 * @param aQuality quality meta value to be used
	 * @return modified builder object
	 */
	Encode & Quality(QualitySetting aQuality) { quality=aQuality; return *this; }
	#ifdef HAS_FFMPEG
	
	/**
	 * set aac bitrate to given value
	 * @param aBitrate bitrate value to be used
	 * @return modified builder object
	 */
	Encode & Bitrate(int aBitrate) { aacBitrate=aBitrate; return *this; }
	#endif
	
	/**
	 * Create mp3 file from builder
	 * @param filename under which the encoded file shall be saved
	 * @return return value of external command
	 */
	int    mp3(std::string filename)
	{
		return lame(channels,filename,quality,title,artist,album,comment,
				image,category,episode,year);

	}

	/**
	 * Create ogg file from builder
	 * @param filename under which the encoded file shall be saved
	 * @return return value of external command
	 */
	int    ogg(std::string filename)
	{
		return oggenc(channels,filename,quality,title,artist,album,comment,
					  category,episode);

	}
	
	#ifdef HAS_FFMPEG
	/**
	 * Create aac file from builder
	 * @param filename under which the encoded file shall be saved
	 * @return return value of command
	 */
	int    aac(std::string filename)
	{
		return Wave::saveAac(filename,channels,aacBitrate);
	}
	#endif

protected:

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
					QualitySetting quality,
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
					QualitySetting quality,
					std::string title,
					std::string artist,
					std::string album,
					std::string comment,
					std::string category,
					std::string episode);
};

#endif /* ENCODE_H_ */
