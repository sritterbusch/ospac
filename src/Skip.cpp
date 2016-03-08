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
