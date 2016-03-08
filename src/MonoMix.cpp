/**
 * @file		MonoMix.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		12.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Mono mix-down
 */

#include "Log.h"
#include "MonoMix.h"

MonoMix::MonoMix() : target(1)
{
}

void MonoMix::mix(Channel &c,float factor)
{
	if(target[0].size()==0 || c.samplerate()>target[0].samplerate())
	{
		target[0]=target[0].resampleTo(c.samplerate());
		LOG(logDEBUG) << "Target samplerate set to " << c.samplerate() << std::endl;
	}
	if(c.samplerate()<target[0].samplerate())
		c.resampleTo(target[0].samplerate());
	if(target[0].size()<c.size())
	{
		target[0]=target[0].resizeTo(c.size());
		LOG(logDEBUG) << "Target size set to " << c.size()/c.samplerate() << "s"<< std::endl;
	}

	for(unsigned i=0;i<c.size();i++)
		target[0][i]+=factor*c[i];
}

void MonoMix::mix(Channels &c)
{
	if(c.size()==0)
		return;

	for(unsigned i=0;i<c.size();i++)
		mix(c[i]);
}
