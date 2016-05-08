/**
 * @file		Channel.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		5.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief 		Audio channel abstraction
 */


#include <math.h>
#include <iostream>

#include "Channel.h"
#include "Log.h"

float Channel::zero=0;

Channel::Channel()
{
	rate=44100;
}

Channel::Channel(unsigned aRate) : rate(aRate)
{
}

Channel::Channel(unsigned aRate,const std::vector<float> & aData) : rate(aRate), data(aData)
{
}

Channel::Channel(unsigned aRate,unsigned size) : rate(aRate), data(std::vector<float>(size))
{
}

Channel::~Channel()
{
}

float & Channel::operator [](int index)
{
	if(index<0 || (unsigned)index>=data.size())
	{
		zero=0;
		return zero;
	}
	return data[index];
}
float   Channel::operator [](int index) const
{
	if(index<0 || (unsigned)index>=data.size())
	{
		return 0;
	}
	return data[index];
}

unsigned Channel::size() const
{
	return data.size();
}

unsigned Channel::samplerate() const
{
	return rate;
}

double Channel::l2norm(void) const
{
	double sum=0.0;
	for(unsigned i=0;i<data.size();i++)
		sum+=sqr<float>(data[i]);
	if(sum!=0.0)
		sum/=data.size();
	return sqrt(sum);
}

double  Channel::l2upnorm(float limit) const
{
	double sum=0.0;
	int count=0;
	float v;
	limit*=limit;
	for(unsigned i=0;i<data.size();i++)
	{
		v=sqr<float>(data[i]);
		if(v>limit)
		{
			sum+=v;
			count++;
		}
	}
	if(sum!=0.0)
		sum/=count;
	return sqrt(sum);
}
double  Channel::l2downnorm(float limit) const
{
	double sum=0.0;
	int count=0;
	float v;
	limit*=limit;
	for(unsigned i=0;i<data.size();i++)
	{
		v=sqr<float>(data[i]);
		if(v<limit)
		{
			sum+=v;
			count++;
		}
	}
	if(sum!=0.0)
		sum/=count;
	return sqrt(sum);
}


double Channel::linfnorm(void) const
{
	float max=0;
	for(unsigned i=0;i<data.size();i++)
	{
		if(data[i]>max)
			max=data[i];
		if(-data[i]>max)
			max=-data[i];
	}
	return max;
}

Channel Channel::downsample(unsigned factor) const
{
	if(factor>0)
	{
		unsigned newSize=data.size()/factor;
		std::vector<float> target=std::vector<float>(newSize);
		float acc;
		for(unsigned i=0,j=0;j<newSize;j++)
		{
			acc=0;
			for(unsigned k=0;k<factor && i<data.size();k++)
				acc+=data[i++];
			target[j]=acc/factor;
		}
		return Channel(rate/factor,target);
	} else
		return *this;
}

Channel Channel::downsampleEnergy(unsigned factor) const
{
	if(factor>0)
	{
		unsigned newSize=data.size()/factor;
		std::vector<float> target=std::vector<float>(newSize);
		float acc;
		for(unsigned i=0,j=0;j<newSize;j++)
		{
			acc=0;
			for(unsigned k=0;k<factor && i<data.size();k++)
				acc+=sqr(data[i++]);
			target[j]=sqrt(acc/factor);
		}
		return Channel(rate/factor,target);
	} else
		return *this;
}

Channel Channel::resizeTo(unsigned size) const
{
	std::vector<float> target=std::vector<float>(size);
	unsigned i;
	for(i=0;i<size && i<data.size();i++)
		target[i]=data[i];
	for(;i<size;i++)
		target[i]=0;
	return Channel(rate,target);
}

Channel Channel::resampleTo(unsigned newRate) const
{
	unsigned newSize=(data.size()*newRate)/rate;
	std::vector<float> target=std::vector<float>(newSize);
	LOG(logDEBUG) << "Old rate "<< rate << " New Rate: " << newRate << std::endl;
	LOG(logDEBUG) << "Old size " << data.size() << " New Size: " << newSize << std::endl;

	unsigned oldSize=data.size();

	// TODO: Only nearest "interpolation"...
	for(unsigned i=0;i<newSize;i++)
	{
		int j=(long(i)*oldSize)/newSize;
		target[i]=data[j];
	}

	LOG(logDEBUG) << "done"<< std::endl;
	return Channel(newRate,target);
}

unsigned unifiedSamplerate(Channels &a)
{
	unsigned samplerate=0;

	for(unsigned c=0;c<a.size();c++)
		if(a[c].samplerate()>samplerate)
			samplerate=a[c].samplerate();

	for(unsigned c=0;c<a.size();c++)
		if(a[c].samplerate()<samplerate)
			a[c]=a[c].resampleTo(samplerate);

	return samplerate;
}

unsigned unifiedLength(Channels &a)
{
	unsigned len=0;

	for(unsigned c=0;c<a.size();c++)
		if(a[c].size()>len)
			len=a[c].size();

	for(unsigned c=0;c<a.size();c++)
		if(a[c].size()<len)
			a[c]=a[c].resizeTo(len);

	return len;
}

void unify(Channels &a)
{
	unifiedLength(a);
	unifiedSamplerate(a);
}
