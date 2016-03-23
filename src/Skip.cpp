/**
 * @file		Skip.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		18.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Skip silence
 */

#include <math.h>

#include "Skip.h"
#include "Log.h"

float Skip::silence(Channels & a,float level,float minsec,float mintransition,float reductionOrder)
{
	if(a.size()==0)
		return 0;
	if(reductionOrder>1)
	{
		LOG(logWARNING) << "Reduction reduction order to maximum 1" << std::endl;
		reductionOrder=1;
	}


	unsigned skip=0;
	unsigned len=0;
	unsigned samplerate=0;

	for(unsigned c=0;c<a.size();c++)
		if(a[c].samplerate()>samplerate)
			samplerate=a[c].samplerate();


	mintransition*=samplerate;
	mintransition=(int)mintransition;

	for(unsigned c=0;c<a.size();c++)
		if(a[c].samplerate()<samplerate)
			a[c]=a[c].resampleTo(samplerate);

	for(unsigned c=0;c<a.size();c++)
		if(a[c].size()>len)
			len=a[c].size();

	for(unsigned c=0;c<a.size();c++)
		if(a[c].size()<len)
			a[c]=a[c].resizeTo(len);

	float max=0;

	for(unsigned i=0;i<len;i++)
	{
		float sum=0;
		for(unsigned c=0;c<a.size();c++)
			sum+=fabs(a[c][i]);
		if(sum>max)
			max=sum;
	}
	max/=a.size();

	level*=max;

	int mincount=minsec*samplerate;

	for(unsigned i=0;i+skip<len;i++)
	{
		float sum;
		int   d=-1;
		do {
			sum=0;
			d++;
			for(unsigned c=0;c<a.size();c++)
				sum+=fabs(a[c][i+d+skip]);
			sum/=a.size();
		} while(sum<level && (d+int(i+skip))<int(len));

		if(d>mincount+mintransition)
		{
			float delta;

			if(i*1./samplerate>0.1)
				delta=float(d-mincount)/samplerate;
			else
				delta=float(d)/samplerate;
			float ndelta=pow(delta+1,reductionOrder)-1;
			if(ndelta>delta)
				ndelta=delta;
			LOG(logDEBUG) << "Found silence at " << double(i)/samplerate
					      << " (" << double(i+skip)/samplerate << ")"
					      << " for " << double(d)/samplerate << "s "
						  << " cutting " << double(ndelta) << "s"
						  << std::endl;

			ndelta*=samplerate;
			ndelta=(int)ndelta;
			int nskip=skip+ndelta;
			LOG(logDEBUG) << "From " << skip << " to " << nskip << std::endl;

			int transition=d-ndelta;
			//if(transition<mintransition)
			//	transition=mintransition;

			int padding=(d-ndelta-transition)/2;

			LOG(logDEBUG) << "Transition: " << transition << " padding: "<<padding<< std::endl;

			for(unsigned j=0;(int)j<padding;j++,i++,d--)
				for(unsigned c=0;c<a.size();c++)
					a[c][i]=a[c][i+skip];

			for(unsigned j=0;(int)j<transition;j++,i++,d--)
				for(unsigned c=0;c<a.size();c++)
					a[c][i]=((a[c][i+skip]*(transition-j))/transition+(a[c][i+nskip]*j)/transition);

			for(unsigned j=0;(int)j<padding;j++,i++,d--)
				for(unsigned c=0;c<a.size();c++)
					a[c][i]=a[c][i+nskip];

			LOG(logDEBUG) << "Position now is " << double(i)/samplerate << std::endl;

			skip=nskip;
			i--;

		} else
		for(;d>=0;d--,i++)
			for(unsigned c=0;c<a.size();c++)
				a[c][i]=a[c][i+skip];
		i--;

	}
	for(unsigned c=0;c<a.size();c++)
	{
		a[c]=a[c].resizeTo(len-skip);
	}
	LOG(logDEBUG) << "Size before: " << len << " Size after: " << a[0].size()
			<< std::endl;
	float skipped=float(skip)/samplerate;
	LOG(logINFO) << "Skipped " << skipped << "s" << std::endl;
	return skipped;
}

float Skip::noise(Channels &a,float level,float minsec,float transition)
{
	if(a.size()==0)
		return 0;


	unsigned skip=0;
	unsigned len=0;
	unsigned samplerate=0;

	for(unsigned c=0;c<a.size();c++)
		if(a[c].samplerate()>samplerate)
			samplerate=a[c].samplerate();

	if(transition>minsec/2)
		transition=minsec/2;

	minsec*=samplerate;
	minsec=(int)minsec;

	transition*=samplerate;
	transition=(int)transition;

	for(unsigned c=0;c<a.size();c++)
		if(a[c].samplerate()<samplerate)
			a[c]=a[c].resampleTo(samplerate);

	for(unsigned c=0;c<a.size();c++)
		if(a[c].size()>len)
			len=a[c].size();

	for(unsigned c=0;c<a.size();c++)
		if(a[c].size()<len)
			a[c]=a[c].resizeTo(len);

	float max=0;

	for(unsigned i=0;i<len;i++)
	{
		float sum=0;
		for(unsigned c=0;c<a.size();c++)
			sum+=fabs(a[c][i]);
		if(sum>max)
			max=sum;
	}
	max/=a.size();

	level*=max;

	unsigned lastend=0;

	for(unsigned i=0;i<len;i++)
	{
		float sum;
		int   d=-1,s=-1;
		do {
			do {
				sum=0;
				d++;
				for(unsigned c=0;c<a.size();c++)
					sum+=fabs(a[c][i+d+skip]);
				sum/=a.size();
			} while(sum>level && (d+int(i+skip))<int(len));
			s=d;
			do {
				sum=0;
				s++;
				for(unsigned c=0;c<a.size();c++)
					sum+=fabs(a[c][i+s+skip]);
				sum/=a.size();
			} while(sum<=level && (s+int(i+skip))<int(len));

		} while(s-d<minsec && (s+int(i+skip))<int(len));
		LOG(logDEBUG) << double(i)/samplerate << "/"<<double(i+skip)/samplerate<<": Found signal until "<<double(i+d)/samplerate<<std::endl;
		LOG(logDEBUG) << double(i)/samplerate << "/"<<double(i+skip)/samplerate<<": Found silence until "<<double(i+s)/samplerate<<std::endl;

		if(s-d>=minsec)
		{
			skip+=d;

			lastend=i+s;

			if(i>transition)
				for(unsigned j=0;j<transition;j++)
				{
					double f=double(j)/transition;
					for(unsigned c=0;c<a.size();c++)
						a[c][i-transition+j]=a[c][i-transition+j]*(1-f)
											+a[c][i+skip+j]*f;
				}

			skip+=transition;
			lastend-=transition;

			for(;i<lastend-d;i++)
				for(unsigned c=0;c<a.size();c++)
					a[c][i]=a[c][i+skip];

			LOG(logDEBUG) << "Skip now: " << skip << " ("<<double(skip)/samplerate <<")"<< std::endl;
			LOG(logDEBUG) << "Position now: " << i << " (" <<double(i)/samplerate << ")" << std::endl;
		} else
		{
			i=len+1;
			skip+=s;
		}
		i--;
	}
	for(unsigned c=0;c<a.size();c++)
	{
		a[c]=a[c].resizeTo(len-skip);
	}
	LOG(logDEBUG) << "Size before: " << len << " Size after: " << a[0].size()
			<< std::endl;
	float skipped=float(skip)/samplerate;
	LOG(logINFO) << "Skipped " << skipped << "s" << std::endl;
	return skipped;
}


