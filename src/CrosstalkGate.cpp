/**
 * @file		CrosstalkGate.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		9.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Crosstalk gate mutes less active channels
 */


#include "CrosstalkGate.h"
#include "Log.h"
#include "Wave.h"

void CrosstalkGate::gate(Channels &channels,unsigned downsampleLevel,double windowsec,double mixsec)
{
	if(channels.size()==0)
		return;

	unsigned workWindow=windowsec*channels[0].samplerate()/downsampleLevel;
	unsigned mixWindow=mixsec*channels[0].samplerate();

	LOG(logINFO) << "Crossgate with workWindow " << workWindow << std::endl;

	Channels activity=Channels(channels.size());
	Channels downsample=Channels(channels.size());
	//Channels muteFactor=Channels(channels.size());

	std::vector<double> l2norm=std::vector<double>(channels.size());
	std::vector<double> l2upnorm=std::vector<double>(channels.size());
	std::vector<double> l2downnorm=std::vector<double>(channels.size());

	for(unsigned i=0;i<channels.size();i++)
	{
		downsample[i]=channels[i].downsampleEnergy(downsampleLevel);
		//muteFactor[i]=Channel(downsample[i].samplerate(),downsample[i].size());
		activity[i]=Channel(downsample[i].samplerate(),downsample[i].size());

		l2norm[i]=downsample[i].l2norm();
		l2upnorm[i]=downsample[i].l2upnorm(l2norm[i]);
		l2downnorm[i]=downsample[i].l2downnorm(l2norm[i]);

		LOG(logDEBUG) << i << " " << l2norm[i] << " " << l2upnorm[i] << " " << l2downnorm[i] << std::endl;


		double sum=0;

		for(unsigned j=0;j<downsample[i].size();j++)
		{
			sum+=downsample[i][j+workWindow]-downsample[i][j-workWindow];
			if(sum<0)
				sum=0;
			activity[i][j]=(sum/workWindow/2)/l2downnorm[i]*1000;
		}
	}

	// Wave::save("activity.wav",activity);

	LOG(logINFO) << "Crossgate analysis done"<< std::endl;

	std::vector<float> factor(channels.size());
	std::vector<std::vector<float> > memory(channels.size());
	unsigned n=0;
	unsigned p=0;

	for(unsigned i=0;i<channels.size();i++)
	{
		memory[i]=std::vector<float>(mixWindow);
		for(unsigned j=0;j<mixWindow;j++)
			memory[i][j]=0;
	}


	for(unsigned i=0;i<channels[0].size();i++)
	{
		double maxActivity=1e-10;
		unsigned j=i/downsampleLevel;
		float    f=double(i-j*downsampleLevel)/downsampleLevel;
		for(unsigned c=0;c<channels.size();c++)
		{
			float my_act=activity[c][j]*(1-f)+activity[c][j+1]*f;
			if(my_act>maxActivity)
				maxActivity=my_act;
		}
		n++;
		if(n>mixWindow)
			n=mixWindow;
		for(unsigned c=0;c<channels.size();c++)
		{
			float my_act=activity[c][j]*(1-f)+activity[c][j+1]*f;
			float f=my_act/maxActivity;

			memory[c][p]=f;
			factor[c]+=f;
			factor[c]-=memory[c][(p+1)%mixWindow];


			channels[c][i]*=(factor[c]/n);//my_act/maxActivity;
		}
		p=(p+1)%mixWindow;
	}

	LOG(logINFO) << "Crossgate done"<< std::endl;

}
