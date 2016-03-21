/**
 * @file		Wave.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		5.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Wave file management via libsndfile
 */


#include <iostream>
#include <fstream>

extern "C" {
	#include <sndfile.h>
}

#include "Wave.h"
#include "Log.h"

Channels & Wave::load(const std::string &name, Channels & channels,float skip,float length)
{
	LOG(logINFO) << "Loading "<< name << std::endl;

	SNDFILE *sf;
	SF_INFO  info;
	info.format = 0;
	sf = sf_open(name.c_str(),SFM_READ,&info);
	if(sf==NULL)
	{
		LOG(logERROR) << "Could not read " << name << std::endl;
		return channels;
	}

	//if(info.frames>44100*60*10)
	//	info.frames=44100*60*10;

	short * buf=new short[info.channels*65536];

	LOG(logINFO) << "Reading "<<info.frames<<" frames with "<<info.channels<<" channels with rate "<< info.samplerate << std::endl;
	//std::cerr << info.format << std::endl;

	unsigned o=channels.size();

	unsigned skiplen=skip*(int)info.samplerate;
	unsigned fulllen=length*(int)info.samplerate;

	if(skiplen>info.frames)
		skiplen=info.frames;
	if(fulllen>info.frames-skiplen || length>1e+90)
		fulllen=info.frames-skiplen;

	for(int i=0;i<info.channels;i++)
		channels.push_back(Channel((int)info.samplerate,std::vector<float>(fulllen)));

	for(int i=0;i<info.frames && i<skiplen+fulllen;)
	{
		int items=65536;

		if(i+items>=info.frames)
			items=info.frames-i;

		//LOG(logDEBUG) << i << ": Reading "<< items << " ";

		int hasread=sf_read_short(sf,buf,items*info.channels);
		hasread=hasread+0;

		LOG(logDEBUG2) << i << ": Read    "<< hasread << std::endl;

		for(int j=0,t=0;j<items;i++,j++)
			for(int c=0;c<info.channels;c++)
				if(i>=skiplen && i<=skiplen+fulllen)
					channels[o+c][i-skiplen]=buf[t++];
	}

	delete [] buf;

	sf_close(sf);
	LOG(logDEBUG) << "Loading done" << std::endl;
	return channels;
}

Channels & Wave::loadAscii(const std::string &name,int samplerate,Channels & channels,float skip,float maxlength)
{
	int length=0;
	double min=1e99;
	double max=-1e99;
	double sum=0;
	unsigned count=0;

	unsigned skiplen=skip*samplerate;
	unsigned fulllen=maxlength*samplerate;

	{
		std::ifstream in(name.c_str());

		if(in.fail())
			return channels;

		double dummy;
		for(;!in.eof();length++)
		{
			while(!in.eof() && (in.peek()=='#' || in.peek()==' '
			   || in.peek()=='\n' || in.peek()=='\r'
			   || in.peek()=='\t'))
			{
				char c;
				c=in.get();

				if(c=='#')
				{
					while(!in.eof() && in.peek()!='\n' && in.peek()!='\r')
						in.get();
				}
			}
			in >> dummy;
			if(length>=skiplen && (length<fulllen || maxlength>1e+90))
			{
				sum+=dummy;
				count++;
				if(dummy>max)
				{
					max=dummy;
				}
				if(dummy<min)
				{
					min=dummy;
				}
			}
		}
	}
	if(skip>length)
		skip=length;
	if(fulllen>length-skip || maxlength>1e+90)
		fulllen=length-skip;

	if(count==0)
		count=1;
	double null=sum/count;
	if(min>null)
		min=null-1;
	if(max<null)
		max=null+1;

	// Kill DC signal...
	if(2*null-min>max)
		max=2*null-min;
	if(2*null-max<min)
		min=2*null-max;

	unsigned o=channels.size();
	channels.push_back(Channel(samplerate,fulllen));
	std::ifstream in(name.c_str());
	int i=0;
	for(;!in.eof() && i<length && i<skiplen+fulllen;i++)
	{
		while(!in.eof() && (in.peek()=='#' || in.peek()==' '
		   || in.peek()=='\n' || in.peek()=='\r'
		   || in.peek()=='\t'))
		{
			char c;
			c=in.get();

			if(c=='#')
			{
				while(!in.eof() && in.peek()!='\n' && in.peek()!='\r')
					in.get();
			}
		}

		double d;
		in >> d;
		if(i>=skiplen && i<skiplen+fulllen)
		channels[o][i-skiplen]=(d-min)/(max-min)*64000-32000;
	}

	return channels;
}

Channels Wave::load(const std::string &name,float skip,float length)
{
	std::vector<Channel> channels;

	LOG(logINFO) << "Loading "<< name << std::endl;

	return load(name,channels,skip,length);
}

int Wave::save(const std::string &name,Channel & channel)
{
	Channels channels=Channels(1);
	channels[0]=channel;
	return save(name,channels);
}


int Wave::save(const std::string &name,Channels & channels)
{
	SF_INFO  info;
	if(channels.size()==0)
		return 1;

	int no_channels=info.channels=channels.size();
	info.samplerate=channels[0].samplerate();
	int frames=info.frames=channels[0].size();

	LOG(logINFO) << "Writing " << name << " with "<< info.channels << " channels in " << info.samplerate << "Hz and " << info.frames << "frames " << std::endl;

	for(unsigned i=0;i<channels.size();i++)
	{
		if(channels[i].samplerate()!=(unsigned)info.samplerate)
			channels[i]=channels[i].resampleTo(info.samplerate);
		if(channels[i].size()!=info.frames)
			channels[i]=channels[i].resizeTo(info.frames);
	}

	SNDFILE *sf;

	info.format=SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	info.sections=1;
	info.seekable=1;

	//std::cerr << info.format << std::endl;


	sf=sf_open(name.c_str(),SFM_WRITE,&info);

	if(sf==NULL)
	{
		LOG(logERROR) << "Could not write " << name << std::endl;
		return 1;
	}

	short * buf=new short[info.channels*65536];

	//std::cerr << "Starting" << std::endl;

	for(int i=0;i<frames;)
	{
		int items=65536;

		if(i+items>=frames)
			items=frames-i;

		//std::cerr << items << std::endl;

		for(int j=0,t=0;j<items;i++,j++)
			for(int c=0;c<no_channels;c++)
			{
				if(((int)channels[c][i])<-32767)
					buf[t++]=-32767;
				else if(((int)channels[c][i])>32767)
					buf[t++]=32767;
				else
					buf[t++]=channels[c][i];
			}

		int haswritten=sf_write_short(sf,buf,items*no_channels);
		//std::cerr << "wrote " << haswritten << std::endl;
		haswritten=haswritten+0;
	}

	delete [] buf;

	sf_close(sf);


	return 0;
}
