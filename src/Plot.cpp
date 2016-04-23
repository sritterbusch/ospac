/**
 * @file		Plot.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		6.3.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Simple plots of audio channels
 */


#include <fstream>
#include <math.h>

#include "Plot.h"

std::vector<std::vector<unsigned> > Plot::histogramm(const Channel &channel,unsigned sizeX,unsigned sizeY,unsigned size)
{
	std::vector<std::vector<unsigned> > map=std::vector<std::vector<unsigned> >(sizeX);
	for(unsigned x=0;x<sizeX;x++)
	{
		map[x]=std::vector<unsigned>(sizeY);
		for(unsigned y=0;y<sizeY;y++)
			map[x][y]=0;
	}

	double max=32768;
	if(size==0)
		size=channel.size();
	unsigned maxv=1;
	unsigned zero=(int)(0.5*sizeY);

	int last=zero;

	for(unsigned long t=0;t<size;t++)
	{
		int value=(int)((channel[t]/max/2+0.5)*sizeY);
		unsigned x=t*sizeX/size;
		if(value<0)
			value=0;
		if(((unsigned)value)>=sizeY)
			value=sizeY-1;
		int newLast=value;
		if(value>=last)
		{
			for(;value>=last;value--)
			{
				unsigned v=(map[x][value]++);
				if(v>maxv)
					maxv=v;
			}
		} else
		{
			for(;value<=last;value++)
			{
				unsigned v=(map[x][value]++);
				if(v>maxv)
					maxv=v;
			}
		}
		last=newLast;

	}
	for(unsigned x=0;x<sizeX;x++)
		for(unsigned y=0;y<sizeY;y++)
		{
			unsigned v=map[x][y];
			if(v!=0)
			{
				v=log(v+1.0)*255.0/log(maxv+1.0);
				if(v==0)
					v=1;
				map[x][y]=v;
			}
		}

	return map;
}

void Plot::createPGMPlot(const Channels &channels, std::string name,unsigned sizeX, unsigned sizeY)
{
	if((sizeY%2)==0)
		sizeY++;
	unsigned totalY=sizeY*channels.size();
	std::vector<std::vector<unsigned> > image=std::vector<std::vector<unsigned> >(sizeX);
	for(unsigned x=0;x<sizeX;x++)
		image[x]=std::vector<unsigned>(totalY);

	unsigned size=0;
	for(unsigned c=0;c<channels.size();c++)
		if(channels[c].size()>size)
			size=channels[c].size();

	for(unsigned c=0;c<channels.size();c++)
	{
		std::vector<std::vector<unsigned> > hist=histogramm(channels[c],sizeX,sizeY,size);
		for(unsigned x=0;x<sizeX;x++)
			for(unsigned y=0;y<sizeY;y++)
			{
				unsigned v=hist[x][y];
				if(v>0)
					v=v/2+128;
				image[x][(c+1)*sizeY-y]=255-v;
			}
	}
	std::ofstream out(name.c_str());
	out << "P2" << std::endl;
	out << sizeX << " " << totalY << std::endl;
	out << 255 << std::endl;
	for(unsigned y=0;y<totalY;y++)
	{
		for(unsigned x=0;x<sizeX;x++)
			out << image[x][y] << " ";
		out << std::endl;
	}
	out.close();
}

void Plot::createPGMPlot(const Channel &channel, std::string name,unsigned sizeX, unsigned sizeY)
{
	Channels temp=Channels(1);
	temp[0]=channel;
	createPGMPlot(temp,name);
}

void Plot::createPPMPlot(const Channels &channels, std::string name,unsigned sizeX, unsigned sizeY)
{
	if((sizeY%2)==0)
		sizeY++;
	unsigned totalY=sizeY*channels.size();
	std::vector<std::vector<unsigned> > imageR=std::vector<std::vector<unsigned> >(sizeX);
	std::vector<std::vector<unsigned> > imageG=std::vector<std::vector<unsigned> >(sizeX);
	std::vector<std::vector<unsigned> > imageB=std::vector<std::vector<unsigned> >(sizeX);
	for(unsigned x=0;x<sizeX;x++)
	{
		imageR[x]=std::vector<unsigned>(totalY);
		imageG[x]=std::vector<unsigned>(totalY);
		imageB[x]=std::vector<unsigned>(totalY);
	}

	unsigned size=0;
	unsigned samplerate=channels[0].samplerate();
	for(unsigned c=0;c<channels.size();c++)
		if(channels[c].size()>size)
			size=channels[c].size();
	float timelength=size/(samplerate+1e-10);

	unsigned back=255-7;
	for(unsigned c=0;c<channels.size();c++)
	{
		std::vector<std::vector<unsigned> > hist=histogramm(channels[c],sizeX,sizeY,size);
		for(unsigned x=0;x<sizeX;x++)
			for(unsigned y=0;y<sizeY;y++)
			{
				unsigned v=hist[x][sizeY-1-y];

				if(v>0)
				{
					imageR[x][y+c*sizeY]=64-v/4;
					imageG[x][y+c*sizeY]=65-(v+1)/4;
					imageB[x][y+c*sizeY]=64+((v*3)/4);//127+v/4;
				} else
				{
					if(x!=0 && x!=sizeX-1 && y!=0 && y!=sizeY-1)
					{
						imageR[x][y+c*sizeY]=back;
						imageG[x][y+c*sizeY]=back;
						imageB[x][y+c*sizeY]=back;
					} else
					{
						imageR[x][y+c*sizeY]=128+64;
						imageG[x][y+c*sizeY]=128+64;
						imageB[x][y+c*sizeY]=128+64;
					}

				}
			}
	}
	unsigned secondsG=back-16;
	unsigned secondsG10=secondsG-16;
	unsigned minutesG=secondsG10-16;
	unsigned minutesG10=minutesG-16;
	unsigned hoursG=minutesG10-16;
	if(timelength/3600<sizeX/10)
		for(unsigned hours=1;hours<timelength/3600;hours++)
		{
			unsigned x=hours*3600*sizeX/timelength;
			for(unsigned y=0;y<totalY;y++)
				if(imageR[x][y]==back
				&& imageG[x][y]==back
				&& imageB[x][y]==back)
					imageR[x][y]=imageG[x][y]=imageB[x][y]=hoursG;
		}
	if(timelength/600<sizeX/10)
		for(unsigned minutes10=1;minutes10<timelength/600;minutes10++)
		{
			unsigned x=minutes10*600*sizeX/timelength;
			for(unsigned y=0;y<totalY;y++)
				if(imageR[x][y]==back
				&& imageG[x][y]==back
				&& imageB[x][y]==back)
					imageR[x][y]=imageG[x][y]=imageB[x][y]=minutesG10;
		}
	if(timelength/60<sizeX/10)
		for(unsigned minutes=1;minutes<timelength/60;minutes++)
		{
			unsigned x=minutes*60*sizeX/timelength;
			for(unsigned y=0;y<totalY;y++)
				if(imageR[x][y]==back
				&& imageG[x][y]==back
				&& imageB[x][y]==back)
					imageR[x][y]=imageG[x][y]=imageB[x][y]=minutesG;
		}
	if(timelength/10<sizeX/10)
		for(unsigned seconds10=1;seconds10<timelength/10;seconds10++)
		{
			unsigned x=seconds10*10*sizeX/timelength;
			for(unsigned y=0;y<totalY;y++)
				if(imageR[x][y]==back
				&& imageG[x][y]==back
				&& imageB[x][y]==back)
					imageR[x][y]=imageG[x][y]=imageB[x][y]=secondsG10;
		}
	if(timelength<sizeX/10)
		for(unsigned seconds=1;seconds<timelength;seconds++)
		{
			unsigned x=seconds*sizeX/timelength;
			for(unsigned y=0;y<totalY;y++)
				if(imageR[x][y]==back
				&& imageG[x][y]==back
				&& imageB[x][y]==back)
					imageR[x][y]=imageG[x][y]=imageB[x][y]=secondsG;
		}

	std::ofstream out(name.c_str());
	/*out << "P3" << std::endl;
	out << sizeX << " " << totalY << std::endl;
	out << 255 << std::endl;
	for(unsigned y=0;y<totalY;y++)
	{
		for(unsigned x=0;x<sizeX;x++)
			out << imageR[x][y] << " "
			    << imageG[x][y] << " "
				<< imageB[x][y] << " ";
		out << std::endl;
	}
	out.close();*/
	out << "P6" << std::endl;
	out << sizeX << " " << totalY << std::endl;
	out << 255 << std::endl;
	for(unsigned y=0;y<totalY;y++)
	{
		for(unsigned x=0;x<sizeX;x++)
			out << (unsigned char)imageR[x][y]
			    << (unsigned char)imageG[x][y]
				<< (unsigned char)imageB[x][y] ;
	}
	out.close();
}

void Plot::createPPMPlot(const Channel &channel, std::string name,unsigned sizeX, unsigned sizeY)
{
	Channels temp=Channels(1);
	temp[0]=channel;
	createPPMPlot(temp,name);
}
