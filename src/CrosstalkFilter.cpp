/**
 * @file		CrosstalkFilter.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		6.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Filter to actively identify crosstalk in other channels
 */

#include <math.h>
#include <string>

#include "CrosstalkFilter.h"
#include "Wave.h"
#include "Physics.h"
#include "Log.h"


CrosstalkFilter::CrosstalkFilter(Channels &aChannels,
								 unsigned aDownsampleLevel,
								 double windowsec,
								 double mindistance,
								 double maxdistance,
								 float aMuteStartRatio,
								 float aMuteFullRatio)
								: downsampleLevel(aDownsampleLevel),
								  channels(aChannels),
								  muteStartRatio(aMuteStartRatio),
								  muteFullRatio(aMuteFullRatio)
{
	int f=44100;
	if(channels.size()>0)
		f=channels[0].samplerate();

	f/=aDownsampleLevel;

	workWindow=int(windowsec*f+0.5);
	if(workWindow<2)
		workWindow=2;

	minShift=unsigned(Physics::meterToSec(mindistance)*f+0.5);
	maxShift=unsigned(Physics::meterToSec(maxdistance)*f+0.5);

	if(minShift<1)
		minShift=1;
	if(maxShift<=minShift)
		maxShift=minShift+1;

	LOG(logINFO) << "setting up " << workWindow << " samples workWindow, "
			     << minShift << " samples minShift and "
				 << maxShift << " samples maxShift"<< std::endl;

	prepareVectors();
}


CrosstalkFilter::CrosstalkFilter(Channels &aChannels,
								 unsigned aDownsampleLevel,
								 unsigned aWorkWindow,
								 unsigned aMinShift,
								 unsigned aMaxShift,
								 float aMuteStartRatio,
								 float aMuteFullRatio)
								: downsampleLevel(aDownsampleLevel),
								  channels(aChannels),
								  workWindow(aWorkWindow),
								  minShift(aMinShift),
								  maxShift(aMaxShift),
								  muteStartRatio(aMuteStartRatio),
								  muteFullRatio(aMuteFullRatio)
{
	prepareVectors();
}

void CrosstalkFilter::prepareVectors()
{
	downsample=Channels(channels.size());
	muteFactor=Channels(channels.size());
	activity=Channels(channels.size());
	l2norm=std::vector<double>(channels.size());
	l2upnorm=std::vector<double>(channels.size());
	l2downnorm=std::vector<double>(channels.size());

	for(unsigned i=0;i<channels.size();i++)
	{
		downsample[i]=channels[i].downsample(downsampleLevel);
		muteFactor[i]=Channel(downsample[i].samplerate(),downsample[i].size());
		activity[i]=Channel(downsample[i].samplerate(),downsample[i].size());

		l2norm[i]=downsample[i].l2norm();
		l2upnorm[i]=downsample[i].l2upnorm(l2norm[i]);
		l2downnorm[i]=downsample[i].l2downnorm(l2norm[i]);


		double sum=0;

		for(unsigned j=0;j<downsample[i].size();j++)
		{
			sum+=downsample[i][j+workWindow*10]-downsample[i][j-workWindow*10];
			if(sum<0)
				sum=0;
			activity[i][j]=(sum/workWindow/2/10)/l2downnorm[i]*1000;
		}
	}
}

/** CrosstalkFilter analysis
 * 	Looks through all channels if sound bits of other channels are contained and sets
 * 	mute vector correspondingly. Does not alter any channels.
 */
void CrosstalkFilter::analyze2()
{
	LOG(logDEBUG) << "We have " << channels.size() << " channels." << std::endl;
	for(unsigned i=0;i<channels.size();i++)
		for(unsigned j=0;j<channels.size();j++)
		if(i!=j)
		{
			LOG(logDEBUG) << i << " " << downsample[i].samplerate() << " vs " << j << std::endl;
			int max=0;
			double maxsum=0;
			for(unsigned k=minShift;k<maxShift;k++)
			{
				double sum=0;
				double n1=0,n2=0;
				for(unsigned l=0;l<downsample[i].size();l++)
				{
					sum+=downsample[i][l]*downsample[j][l+k];
					n1+=downsample[i][l];
					n2+=downsample[j][l+k];
				}
				sum/=(channels[i].size());
				if(sum>maxsum)
				{
					maxsum=sum;
					max=k;
				}
				LOG(logDEBUG) << i << " vs " << j << " in " << k << ": " << sum << " - " << n1 << " - " << n2 << std::endl;
			}
			LOG(logDEBUG) << "Maximum bei " << max << " also " << Physics::secToMeter(double(max)/downsample[i].samplerate()) << "m mit " << maxsum << std::endl;
		}


	for(unsigned i=0;i<channels.size();i++)
	{
		LOG(logDEBUG) << "Analyzing " << downsample[i].size() << " windows." << std::endl;
		int count=0;
		int shiftsum=0;
		for(unsigned j=0;j<downsample[i].size();j++)
		{
			double mute=1;

			for(unsigned k=minShift;k<maxShift && mute>0;k++)
			{
				for(unsigned c=0;c<channels.size() && mute>0;c++)
				{
					if(c!=i)
					{
						double n=0,n1=0;

						for(int l=-workWindow;l<(int)workWindow;l++)
						{
							n+=fabs(downsample[c][j+l-k]);
							n1+=fabs(downsample[i][j+l]);
						}
						if(n1>0)
						{
							double factor=n1/n;

							n1*=factor;
							n=0;

							for(int l=-workWindow;l<(int)workWindow;l++)
							{
								n+=fabs(downsample[i][j+l]-factor*downsample[c][j+l-k]);
							}


							double newmute=1;
							double r=n/n1;

							if(activity[i][j]>activity[c][j-k])
								r-=0.25;
							else
								r+=0.25;

							if(r>muteStartRatio && r<muteFullRatio)
							{
								newmute=1-(r-muteStartRatio)/(muteFullRatio-muteStartRatio);
							} else
							if(r>muteFullRatio)
							{
								newmute=0;
								count++;
								shiftsum+=k;
							}

							if(newmute<mute)
								mute=newmute;
						}
					}
				}

			}
			muteFactor[i][j]=mute;
		}
		if(count>0)
		{
			LOG(logDEBUG) << "Found: " << count << " with avg-shift " << double(shiftsum)/count << std::endl;
		}
	}
}

/** CrosstalkFilter analysis
 * 	Looks through all channels if sound bits of other channels are contained and sets
 * 	mute vector correspondingly. Does not alter any channels.
 */
void CrosstalkFilter::analyze()
{
	LOG(logDEBUG) << "We have " << channels.size() << " channels." << std::endl;
	std::vector<std::vector<unsigned> > shift(channels.size());
	for(unsigned i=0;i<channels.size();i++)
	{
		shift[i]=std::vector<unsigned>(channels.size());
		for(unsigned j=0;j<channels.size();j++)
		if(i!=j)
		{
			LOG(logDEBUG) << i << " " << downsample[i].samplerate() << " vs " << j << std::endl;
			int max=0;
			double maxsum=0;
			for(unsigned k=minShift;k<maxShift;k++)
			{
				double sum=0;
				for(unsigned l=0;l<downsample[i].size();l++)
				{
					sum+=downsample[i][l]*downsample[j][l-k];
				}
				sum/=(channels[i].size());
				sum=fabs(sum);
				if(sum>maxsum)
				{
					maxsum=sum;
					max=k;
				}

			}
			LOG(logDEBUG) << "Maximum bei " << max << " also " << Physics::secToMeter(double(max)/downsample[i].samplerate()) << "m mit " << maxsum << std::endl;
			shift[i][j]=max;
		}
	}

	for(unsigned i=0;i<channels.size();i++)
	{
		LOG(logDEBUG) << i << ": Analyzing " << downsample[i].size() << " windows." << std::endl;
		for(unsigned l=0;l<muteFactor[i].size();l++)
			muteFactor[i][l]=0;

		for(unsigned j=0;j<channels.size();j++)
		{
			double skpIn=0;
			double skpOut=0;
			double ni2=0,nj2=0,nr=0;
			int    deltaIn=20;
			int    deltaOut=20;

			if(i!=j)
			{
				LOG(logDEBUG) << i << " " << j << std::endl;
				unsigned sIn=shift[i][j];
				unsigned sOut=shift[j][i];
				if((unsigned)deltaIn<sIn)
					sIn=deltaIn;
				if((unsigned)deltaOut<sOut)
					sOut=deltaOut;
				std::vector<double> skpsIn(deltaIn*2+1);
				std::vector<double> njsIn2(deltaIn*2+1);
				std::vector<double> skpsOut(deltaOut*2+1);
				std::vector<double> njsOut2(deltaOut*2+1);
				for(int v=-deltaIn;v<=deltaIn;v++)
				{
					skpsIn[v+deltaIn]=0;
					njsIn2[v+deltaIn]=0;
				}
				for(int v=-deltaOut;v<=deltaOut;v++)
				{
					skpsOut[v+deltaOut]=0;
					njsOut2[v+deltaOut]=0;
				}

				for(int l=-workWindow;l<0;l++)
				{
					for(int v=-deltaIn;v<=deltaIn;v++)
					{
						skpsIn[v+deltaIn]+=downsample[i][l+workWindow]*downsample[j][l+workWindow-sIn+v];
						njsIn2[v+deltaIn]+=sqr(downsample[j][l+workWindow-sIn+v]);
					}
					for(int v=-deltaOut;v<=deltaOut;v++)
					{
						skpsOut[v+deltaOut]+=downsample[i][l+workWindow]*downsample[j][l+workWindow+sOut+v];
						njsOut2[v+deltaOut]+=sqr(downsample[j][l+workWindow+sOut+v]);
					}
					ni2+=sqr(downsample[i][l+workWindow]);
				}


				for(unsigned l=0;l<downsample[i].size();l++)
				{
					int vIn=0;
					int vOut=0;

					skpIn=0;
					skpOut=0;
					for(int v=-deltaIn;v<=deltaIn;v++)
					{
						skpsIn[v+deltaIn]+=downsample[i][l+workWindow]*downsample[j][l+workWindow-sIn+v];
						skpsIn[v+deltaIn]-=downsample[i][l-workWindow]*downsample[j][l-workWindow-sIn+v];
						if(fabs(skpsIn[v+deltaIn])>skpIn)
						{
							skpIn=fabs(skpsIn[v+deltaIn]);
							vIn=v+deltaIn;
						}
						njsIn2[v+deltaIn]+=sqr(downsample[j][l+workWindow-sIn+v]);
						njsIn2[v+deltaIn]-=sqr(downsample[j][l-workWindow-sIn+v]);
						if(njsIn2[v+deltaIn]<0)
							njsIn2[v+deltaIn]=0;
					}
					for(int v=-deltaOut;v<=deltaOut;v++)
					{
						skpsOut[v+deltaOut]+=downsample[i][l+workWindow]*downsample[j][l+workWindow+sOut+v];
						skpsOut[v+deltaOut]-=downsample[i][l-workWindow]*downsample[j][l-workWindow+sOut+v];
						if(fabs(skpsOut[v+deltaOut])>skpOut)
						{
							skpOut=fabs(skpsOut[v+deltaOut]);
							vOut=v+deltaOut;
						}
						njsOut2[v+deltaOut]+=sqr(downsample[j][l+workWindow+sOut+v]);
						njsOut2[v+deltaOut]-=sqr(downsample[j][l-workWindow+sOut+v]);
						if(njsOut2[v+deltaOut]<0)
							njsOut2[v+deltaOut]=0;
					}


					ni2+=sqr(downsample[i][l+workWindow]);
					ni2-=sqr(downsample[i][l-workWindow]);

					if(ni2<0)
						ni2=0;

					nr=ni2;
					if(njsIn2[vIn]>0)
					{

						nr=(nr-sqr(skpIn)/njsIn2[vIn]);

					}
					if(njsOut2[vOut]>0)
					{

						nr=(nr+sqr(skpOut)/njsOut2[vOut]);
					}

					if(!(muteFactor[i][l]==muteFactor[i][l]))
					{
						LOG(logERROR) << "nan in factor! " << i << " " << l << " " << nj2 << " " << ni2 << " " << nr << " " <<skpIn << std::endl;
						return;
					}

					if(ni2>0 && nr>0)
						muteFactor[i][l]+=(sqrt(ni2)-sqrt(nr))/sqrt(ni2);

					if(!(muteFactor[i][l]==muteFactor[i][l]))
					{
						LOG(logERROR) << "nan in factor! " << i << " " << l << " " << nj2 << " " << ni2 << " " << nr << " " <<skpIn << " " << vIn << " " << njsIn2[vIn] << std::endl;
						return;
					}
				}
			}
		}
	}
}

/** CrosstalkFilter save mute channels
 * 	saves muting filter as a wave file for analysis
 */
void CrosstalkFilter::save(std::string name)
{
	Channels amp=std::vector<Channel>(channels.size());
	for(unsigned c=0;c<muteFactor.size();c++)
	{
		amp[c]=muteFactor[c];
		for(unsigned i=0;i<amp[c].size();i++)
			amp[c][i]*=32000;
	}
	Wave::save(name,amp);
}

/** CrosstalkFilter mute channels
 * 	applies mute filter to previously given channels
 */
void CrosstalkFilter::mute()
{
	for(unsigned j=0;j<muteFactor[0].size();j++)
	{
		float max=-1;
		float min=1;
		for(unsigned c=0;c<channels.size();c++)
		{
			if(muteFactor[c][j]>max)
			{
				max=muteFactor[c][j];
			}
			if(muteFactor[c][j]<min)
			{
				min=muteFactor[c][j];
			}
		}
		if(max-min<0.1)
			max=min+0.1;
		for(unsigned c=0;c<channels.size();c++)
		{
			muteFactor[c][j]=1-(muteFactor[c][j]-min/*+0.1*/)/(max-min/*+0.1*/);
			if(muteFactor[c][j]<0)
				muteFactor[c][j]=0;
			if(!(muteFactor[c][j]==muteFactor[c][j]))
			{
				LOG(logERROR) << "nan in factor! " << c << " " << j << std::endl;
			}
		}
	}

	for(unsigned c=0;c<channels.size();c++)
	{
		int size=channels[c].size();
		int reds=muteFactor[c].size();
		int down=size/reds;

		for(unsigned j=0;j<channels[c].size();j++)
		{
			int   m=j/down;
			channels[c][j]*=muteFactor[c][m];
			if(!(channels[c][j]==channels[c][j]))
			{
				LOG(logERROR) << "nan in channels! " << c << " " << j << std::endl;
			}

		}
	}
}
