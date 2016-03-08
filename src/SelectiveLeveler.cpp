/**
 * @file		SelectiveLeveler.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		7.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Selective Leveler working on windowed l2 energy of signal
 */

#include <math.h>
#include <fstream>

#include "SelectiveLeveler.h"
#include "Log.h"
#include "Wave.h"


void SelectiveLeveler::level(Channels &aChannels,float targetL2,double windowSec,float minFraction,float silentFraction,float forwardWindowSec,float backWindowSec)
{
	for(unsigned i=0;i<aChannels.size();i++)
	{
		LOG(logINFO) << "Working on channel " << i << std::endl;
		level(aChannels[i],targetL2,windowSec,minFraction,silentFraction,forwardWindowSec,backWindowSec);
	}
}

void SelectiveLeveler::levelStereo(Channels &aChannels,float targetL2,double windowSec,float minFraction,float silentFraction,float forwardWindowSec,float backWindowSec)
{
	LOG(logINFO) << "Here!" << std::endl;
	for(unsigned i=0;i<aChannels.size();i+=2)
	{
		if(i+1<aChannels.size())
		{
			LOG(logINFO) << "Working on channels " << i << " and " << i+1 << std::endl;
			levelStereo(aChannels[i],aChannels[i+1],targetL2,windowSec,minFraction,silentFraction,forwardWindowSec,backWindowSec);
		} else
		{
			LOG(logINFO) << "Working on channel " << i << std::endl;
			level(aChannels[i],targetL2,windowSec,minFraction,silentFraction,forwardWindowSec,backWindowSec);
		}
	}
}

void SelectiveLeveler::level(Channel &c,float targetL2,double windowSec,float minFraction,float silentFraction,float forwardWindowSec,float backWindowSec)
{
	float maxL2=0;
	if(windowSec>float(c.size())/c.samplerate()/4)
		windowSec=float(c.size())/c.samplerate()/4;
	const unsigned window=windowSec*c.samplerate();
	const unsigned forwardWindow=forwardWindowSec*c.samplerate();
	const unsigned backWindow=backWindowSec*c.samplerate();

	Channel factors(c);
	Channel factors2(c);
	double l2=0;

	for(unsigned j=0;j<window;j++)
		l2+=sqr(c[j]);

	for(unsigned i=window/2;i<c.size()-window/2-1;i++)
	{
		factors[i]=sqrt(l2/window);
		if(factors[i]>maxL2)
			maxL2=factors[i];
		l2+=sqr(c[i+window/2])-sqr(c[i-window/2]);
		if(l2<0)
			l2=0;
	}

	// float maxL2save=maxL2;

	for(unsigned i=0;i<window/2;i++)
	{
		factors[i]=factors[window/2];//(factors[window/2]*i)/(window/2);
	}
	for(unsigned i=c.size()-window/2-1;i<c.size();i++)
	{
		factors[i]=factors[c.size()-window/2-2];//(factors[c.size()-window/2-2]*(c.size()-i))/(window/2);
	}
	double sum=0;
	int    count=0;
	for(unsigned i=0;i<window;i++)
	{
		if(c[i]>=factors[i])
		{
			sum+=c[i]*c[i];
			count++;
		}
	}
	for(unsigned i=window/2;i<c.size()-window/2-1;i++)
	{
		if(c[i-window/2]>=factors[i-window/2])
		{
			sum-=c[i-window/2]*c[i-window/2];
			count--;
		}
		if(c[i+window/2]>=factors[i+window/2])
		{
			sum+=c[i+window/2]*c[i+window/2];
			count++;
		}
		if(count<1)
			count=1;
		if(sum<0)
			sum=0;

		factors2[i]=sqrt(sum/count);
		if(factors2[i]>maxL2)
			maxL2=factors2[i];

	}

	/*factors2=factors;
	maxL2=maxL2save;*/

	float minLevel=maxL2*minFraction;
	float silentLevel=maxL2*silentFraction;



	// Wave::save("levels.wav",factors);

	LOG(logINFO) << "Maximum windowed L2 energy: " << maxL2 << std::endl;
	LOG(logINFO) << "Level minimum             : " << minLevel << std::endl;
	LOG(logINFO) << "Level silence             : " << silentLevel << std::endl;

	int c0=0,c1=0,c2=0,o=0;

	for(unsigned i=window/2;i<c.size()-window/2-1;i++)
	{
		if(factors2[i]<silentLevel)
		{
			factors2[i]=0;
			c0++;
		} else
		if(factors2[i]<minLevel)
		{
			factors2[i]=(targetL2/factors2[i])*(factors2[i]-silentLevel)/(minLevel-silentLevel);
			c1++;
		} else
		{
			factors2[i]=targetL2/factors2[i];
			c2++;
		}
		if(fabs(factors2[i]*c[i])>32000)
		{
			factors2[i]=32000/fabs(c[i]);
			o++;
		}
	}
	for(unsigned i=0;i<window/2;i++)
		factors2[i]=(factors2[window/2]*i)/(window/2);
	for(unsigned i=c.size()-window/2-1;i<c.size();i++)
		factors2[i]=(factors2[c.size()-window/2-2]*(c.size()-i))/(window/2);

	// Wave::save("factors.wav",factors);


	LOG(logINFO) << "Silence                   : " << double(c0)/c.samplerate() << "s" << std::endl;
	LOG(logINFO) << "Transition                : " << double(c1)/c.samplerate() << "s" << std::endl;
	LOG(logINFO) << "Full                      : " << double(c2)/c.samplerate() << "s" << std::endl;
	LOG(logINFO) << "Over                      : " << double(o)/c.samplerate() << "s" << std::endl;

	int 	windowcount=forwardWindow;
	double 	factorSum=0;
	for(unsigned i=0;i<forwardWindow;i++)
		factorSum+=factors2[i];

	float movingF=0;
	float tolerance=1.10;//0.25+1;



	for(unsigned i=0;i<c.size();i++)
	{
		float f=(factorSum/windowcount);
		if(f>factors2[i])
			f=factors2[i];

		movingF=(65535*movingF+f)/65536;

		if(movingF<f/tolerance)
			movingF=movingF/0.995+0.0001;//movingF=f/tolerance;
		if(movingF>f*tolerance)
			movingF*=0.999;//movingF=f*tolerance;
		if(fabs(movingF*c[i])>32000)
			movingF=fabs(32000./c[i]);
		c[i]*=movingF;//f;
		//if((i%500)==0)
		//	out << double(i)/c.samplerate() << "\t" << factors[i]<< "\t" << f << "\t" << movingF<< std::endl;
		/*if((i%c.samplerate())==0)
		{
			LOG(logDEBUG) << i/c.samplerate() << " " << factors[i] << " " << (factorSum/windowcount) << " " << factorSum << " " << windowcount << std::endl;
		}*/
		if((int)i-(int)backWindow>=0)
		{
			factorSum-=factors2[i-backWindow];
			windowcount--;
			if(factorSum<0)
				factorSum=0;
			if(windowcount<1)
				windowcount=1;

		}
		if(i+forwardWindow<c.size())
		{
			factorSum+=factors2[i+forwardWindow];
			windowcount++;
		}
	}
}
void SelectiveLeveler::levelStereo(Channel &a,Channel &b,float targetL2,double windowSec,float minFraction,float silentFraction,float forwardWindowSec,float backWindowSec)
{
	float maxL2=0;
	if(a.samplerate()>b.samplerate())
		b=b.resampleTo(a.samplerate());
	if(a.samplerate()<b.samplerate())
		a=a.resampleTo(a.samplerate());

	unsigned size=a.size();
	if(b.size()>a.size())
		size=b.size();

	if(size==0)
		return;

	if(windowSec>float(size)/a.samplerate()/4)
		windowSec=float(size)/a.samplerate()/4;
	const unsigned window=windowSec*a.samplerate();
	const unsigned forwardWindow=forwardWindowSec*a.samplerate();
	const unsigned backWindow=backWindowSec*a.samplerate();

	Channel factors(a.samplerate(),size);
	double l2=0;

	for(unsigned j=0;j<window;j++)
		l2+=sqr(a[j])+sqr(b[j]);

	for(unsigned i=window/2;i<size-window/2-1;i++)
	{
		factors[i]=sqrt(l2/window/2);
		if(factors[i]>maxL2)
			maxL2=factors[i];
		l2+=sqr(a[i+window/2])+sqr(b[i+window/2])-sqr(a[i-window/2])-sqr(b[i-window/2]);
		if(l2<0)
			l2=0;
	}
	float minLevel=maxL2*minFraction;
	float silentLevel=maxL2*silentFraction;

	// Wave::save("levels.wav",factors);

	LOG(logINFO) << "Maximum windowed L2 energy: " << maxL2 << std::endl;
	LOG(logINFO) << "Level minimum             : " << minLevel << std::endl;
	LOG(logINFO) << "Level silence             : " << silentLevel << std::endl;

	int c0=0,c1=0,c2=0,o=0;

	for(unsigned i=window/2;i<size-window/2-1;i++)
	{
		if(factors[i]<silentLevel)
		{
			factors[i]=0;
			c0++;
		} else
		if(factors[i]<minLevel)
		{
			factors[i]=(targetL2/factors[i])*(factors[i]-silentLevel)/(minLevel-silentLevel);
			c1++;
		} else
		{
			factors[i]=targetL2/factors[i];
			c2++;
		}
		if(fabs(factors[i]*a[i])>32000 || fabs(factors[i]*b[i])>32000)
		{
			factors[i]=min(32000/fabs(a[i]),32000/fabs(b[i]));
			o++;
		}

	}
	for(unsigned i=0;i<window/2;i++)
		factors[i]=(factors[window/2]*i)/(window/2);
	for(unsigned i=size-window/2-1;i<size;i++)
		factors[i]=(factors[size-window/2-2]*(size-i))/(window/2);

	// Wave::save("factors.wav",factors);


	LOG(logINFO) << "Silence                   : " << double(c0)/a.samplerate() << "s" << std::endl;
	LOG(logINFO) << "Transition                : " << double(c1)/a.samplerate() << "s" << std::endl;
	LOG(logINFO) << "Full                      : " << double(c2)/a.samplerate() << "s" << std::endl;
	LOG(logINFO) << "Over                      : " << double(o)/a.samplerate() << "s" << std::endl;

	int 	windowcount=forwardWindow;
	double 	factorSum=0;
	for(unsigned i=0;i<forwardWindow;i++)
		factorSum+=factors[i];

	for(unsigned i=0;i<size;i++)
	{
		float f=(factorSum/windowcount);
		if(f>factors[i])
			f=factors[i];
		a[i]*=f;
		b[i]*=f;
		/*if((i%c.samplerate())==0)
		{
			LOG(logDEBUG) << i/c.samplerate() << " " << factors[i] << " " << (factorSum/windowcount) << " " << factorSum << " " << windowcount << std::endl;
		}*/
		if((int)i-(int)backWindow>=0)
		{
			factorSum-=factors[i-backWindow];
			windowcount--;
			if(factorSum<0)
				factorSum=0;
		}
		if(i+forwardWindow<size)
		{
			factorSum+=factors[i+forwardWindow];
			windowcount++;
		}
	}
}
