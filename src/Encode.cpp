/**
 * @file		Encode.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		29.3.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief 		Encoding to various formats using external tools
 */

#include <unistd.h>
#include <stdlib.h>
#include <sstream>

#include "Encode.h"
#include "Wave.h"
#include "Log.h"

int Encode::lame(Channels &c,
					std::string filename,
					Quality quality,
					std::string title,
					std::string artist,
					std::string album,
					std::string comment,
					std::string image,
					std::string category,
					std::string episode,
					std::string year)
{

	if(c.size()<1)
		return 0;

	std::string tempfile=filename+".wav";

	Wave::save(tempfile,c);

	unsigned rate=c[0].samplerate();
	if(rate>44100)
		rate=44100;

	unsigned halfrate=rate/2;

	std::string Vrate="9";
	switch(quality)
	{
	case LOW:
		Vrate="9";
		break;
	case STANDARD:
		Vrate="8.5";
		break;
	case HIGH:
		Vrate="5";
		break;
	case INSANE:
		Vrate="1";
		break;
	}

	std::stringstream stream;

	stream << "lame -V" << Vrate
		   << " -q0"
		   << " --lowpass "<< halfrate
		   << " --resample "<< rate;


	if(title!="")
		stream << " --tt \""<<title<<"\"";
	if(artist!="")
		stream << " --ta \""<<artist<<"\"";
	if(comment!="")
		stream << " --tc \""<<comment<<"\"";
	if(year!="")
		stream << " --ty \""<<year<<"\"";
	if(album!="")
		stream << " --tl \""<<album<<"\"";
	if(category!="")
		stream << " --tg \""<<category<<"\"";
	if(image!="")
		stream << " --ti \""<<image<<"\"";
	if(episode!="")
		stream << " --tn \""<<episode<<"\"";

	stream << " "<<tempfile<<" "<<filename;

	LOG(logINFO) << stream.str() << std::endl;

	unsigned result=system(stream.str().c_str());

	unlink(tempfile.c_str());

	return result;
}
