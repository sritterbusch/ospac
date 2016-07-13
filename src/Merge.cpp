/**
 * @file		Merge.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		11.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief       Merging of audio segments either with overlap or fading
 */

#include "Merge.h"
#include "Log.h"

Channels Merge::overlap(Channels &a,Channels &b,float sec)
{
	unsigned samplerate=unifySamplerate(a,b);
	unsigned overlap=sec*samplerate;
	//Channels target(a);
	if(a.size()==0)
		return b;
	if(b.size()==0)
		return a;

	LOG(logDEBUG) << "Size a: " << a[0].size() << " Size b: " << b[0].size() << " Overlap: " << overlap << std::endl;

	if(a[0].size()+b[0].size()<overlap)
		overlap=a[0].size()+b[0].size();

	Channels target(a);

	for(unsigned i=0;i<a.size();i++)
	{
		unsigned fs=a[i].size()+b[i].size()-overlap;
		target[i]=Channel(samplerate,a[i].size()+b[i].size()-overlap);
		unsigned j;
		unsigned as=a[0].size();
		for(j=0;j<as-overlap;j++)
			target[i][j]=a[i][j];
		for(;j<as;j++)
			target[i][j]=a[i][j]+b[i][j-(as-overlap)];
		for(;j<fs;j++)
			target[i][j]=b[i][j-(as-overlap)];
	}
	return target;
}

Channels Merge::fade(Channels &a,Channels &b,float sec)
{
	unsigned samplerate=unifySamplerate(a,b);
	unsigned overlap=sec*samplerate;

	if(a.size()==0)
		return a;

	LOG(logDEBUG) << "Size a: " << a[0].size() << " Size b: " << b[0].size() << " Overlap: " << overlap << std::endl;


	if(a[0].size()+b[0].size()<overlap)
		overlap=a[0].size()+b[0].size();

	Channels target(a);

	for(unsigned i=0;i<a.size();i++)
	{
		unsigned fs=a[i].size()+b[i].size()-overlap;
		target[i]=Channel(samplerate,a[i].size()+b[i].size()-overlap);
		unsigned j;
		unsigned as=a[0].size();
		for(j=0;j<as-overlap;j++)
			target[i][j]=a[i][j];
		for(;j<as;j++)
		{
			float f=float(as-j)/overlap;
			target[i][j]=a[i][j]*f+b[i][j-(as-overlap)]*(1-f);
		}
		for(;j<fs;j++)
			target[i][j]=b[i][j-(as-overlap)];
	}
	return target;
}

Channels Merge::parallel(Channels &a, Channels &b)
{
	unifySamplerate(a,b);
	Channels target(a);
	for(unsigned i=0;i<a.size();i++)
	{
		unsigned offset=0;
		if(a[i].size()>b[i].size())
			offset=a[i].size()-b[i].size();
		for(unsigned j=0;j<offset;j++)
			target[i][j]=a[i][j];
		for(unsigned j=offset;j<a[i].size();j++)
			target[i][j]=a[i][j]+b[i][j-offset];
	}

	return target;
}


int Merge::unifySamplerate(Channels &a,Channels &b)
{
	while(b.size()<a.size())
		b.push_back(Channel(1));
	while(a.size()<b.size())
		a.push_back(Channel(1));

	unsigned freq=1;
	for(unsigned i=0;i<a.size();i++)
		if(a[i].samplerate()>freq)
			freq=a[i].samplerate();

	for(unsigned i=0;i<b.size();i++)
		if(b[i].samplerate()>freq)
			freq=b[i].samplerate();

	for(unsigned i=0;i<a.size();i++)
		if(a[i].samplerate()!=freq)
			a[i]=a[i].resampleTo(freq);

	for(unsigned i=0;i<b.size();i++)
		if(b[i].samplerate()!=freq)
			b[i]=b[i].resampleTo(freq);

	return freq;
}
