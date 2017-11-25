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
#include <map>

extern "C" {
	#include <sndfile.h>
}

#include "Wave.h"
#include "Log.h"

Channels & Wave::load(const std::string &name, Channels & channels,
		              float skip,float length)
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

	unsigned o=channels.size();

	unsigned skiplen=skip*(int)info.samplerate;
	unsigned fulllen=length*(int)info.samplerate;

	if(skiplen>info.frames)
		skiplen=info.frames;
	if(fulllen>info.frames-skiplen || length>1e+90)
		fulllen=info.frames-skiplen;

	for(int i=0;i<info.channels;i++)
		channels.push_back(Channel((int)info.samplerate,std::vector<float>(fulllen)));

	for(unsigned i=0;i<info.frames && i<skiplen+fulllen;)
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
	unsigned length=0;
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
	unsigned i=0;
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

	// LOG(logINFO) << "Loading "<< name << std::endl;

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

	char lengthString[40];

	float secs=frames/float(info.samplerate);
	int   mins=secs/60;
	secs-=mins*60;
	int   hours=mins/60;
	mins-=hours*60;

	if(hours>0)
		sprintf(lengthString,"%d:%02d:%05.2f",hours,mins,secs);
	else
		sprintf(lengthString,"%d:%05.2f",mins,secs);

	LOG(logINFO) << "Writing " << name << " with "<< info.channels << " channels in " << info.samplerate << "Hz and " << info.frames << "frames" << std::endl;
	LOG(logINFO) << "Final length: " <<lengthString << std::endl;

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

#ifdef HAS_FFMPEG

extern "C" {
	#include <libavformat/avformat.h>
	#include <libavcodec/avcodec.h>
}

#undef av_err2str 
#define av_err2str(errnum) av_make_error_string((char*)__builtin_alloca(AV_ERROR_MAX_STRING_SIZE), AV_ERROR_MAX_STRING_SIZE, errnum) 
         
Channels Wave::loadFfmpeg(const std::string & name,float skip,float length)
{
	std::vector<Channel> channels;

	return loadFfmpeg(name,channels,skip,length);
}

float getSample(const AVCodecContext* codecCtx, uint8_t* buffer, int sampleIndex)
{
	int64_t val = 0;
	float ret = 0;
	int sampleSize = av_get_bytes_per_sample(codecCtx->sample_fmt);
	switch(sampleSize)
	{
		case 1:
			// 8bit samples are always unsigned
			val = buffer[sampleIndex];
	        // make signed
	        val -= 127;
	        break;

	    case 2:
	        val = ((int16_t*)(buffer))[sampleIndex];
	        break;

	    case 4:
	        val = ((int32_t*)(buffer))[sampleIndex];
	        break;

	    case 8:
	        val = ((int64_t*)(buffer))[sampleIndex];
	        break;

	    default:
	    	LOG(logERROR) << "Invalid sample size " << sampleSize << std::endl;
	        return 0;
	}

	// Check which data type is in the sample.
	switch(codecCtx->sample_fmt)
	{
		case AV_SAMPLE_FMT_U8:
		case AV_SAMPLE_FMT_S16:
		case AV_SAMPLE_FMT_S32:
		case AV_SAMPLE_FMT_U8P:
		case AV_SAMPLE_FMT_S16P:
		case AV_SAMPLE_FMT_S32P:
			// integer => Scale to [-1, 1] and convert to float.
			ret = val / (float)((1 << (sampleSize*8-1))-1) *32767;
			break;

		case AV_SAMPLE_FMT_FLT:
		case AV_SAMPLE_FMT_FLTP:
			// float => reinterpret
			ret = *((float*)(&val))*32767;
			break;

		case AV_SAMPLE_FMT_DBL:
		case AV_SAMPLE_FMT_DBLP:
			// double => reinterpret and then static cast down
			ret = (float)*((double*)(&val))*32767;
			break;

		default:
			LOG(logERROR) << "Invalid sample format "<<av_get_sample_fmt_name(codecCtx->sample_fmt)<<std::endl;
			return 0;
	}

	return ret;
}

int handleFrame(AVCodecContext * codecContext,AVFrame * frame,int stream_index,std::map<int,Channels> & channels,std::map<int,unsigned> & position)
{
	int error=0;
    int padding=codecContext->initial_padding;

	while((error = avcodec_receive_frame(codecContext,frame)) == 0)
	{
		//LOG(logDEBUG) << "Reading block" << std::endl;
		if(av_sample_fmt_is_planar(codecContext->sample_fmt)==1)
		{
			int channelCount=codecContext->channels;
			Channels & now=channels[stream_index];
			int nowPosition=position[stream_index];
			/*if(frame->pts !=nowPosition)
			{
				LOG(logINFO) << frame->pts/44100 << " " << frame->pkt_dts << " " << frame->pkt_pos<< " vs " << nowPosition/44100.0 << " - "<< float(frame->pkt_dts)/nowPosition << " - "<< float(nowPosition)/frame->pkt_pos << " I "<< codecContext->time_base.num<<"/"<<codecContext->time_base.den<< std::endl;
			}*/
			if(nowPosition+frame->nb_samples>now[0].size())
			{
				LOG(logWARNING) << "Expected size of " << now[0].size() << std::endl;// << " but will read " << nowPosition+frame->nb_samples << std::endl;

				for(int c=0;c<channelCount;c++)
					now[c]=now[c].resizeTo(now[c].size()+now[c].size()/16);
				LOG(logWARNING) << " resized to " << now[0].size()<< std::endl;
				//LOG(logWARNING) << "Expected size of " << now[0].size() << " but will read " << nowPosition+frame->nb_samples << std::endl;
			}
			//LOG(logDEBUG) << "Planar at " << nowPosition << std::endl;
			for(int s=0;s<frame->nb_samples;s++)
				for(int c=0;c<channelCount;c++)
					if(nowPosition+s-padding>=0)
						now[c][nowPosition+s-padding]=getSample(codecContext,frame->extended_data[c],s);

			position[stream_index]+=frame->nb_samples;
		} else
		{
			int channelCount=codecContext->channels;
			Channels & now=channels[stream_index];
			int nowPosition=position[stream_index];
			//LOG(logDEBUG) << "Unplanar at " << nowPosition << std::endl;

			for(int s=0;s<frame->nb_samples;s++)
				for(int c=0;c<channelCount;c++)
					if(nowPosition+s-padding>=0)
						now[c][nowPosition+s-padding]=getSample(codecContext,frame->extended_data[0],s*channelCount+c);

			position[stream_index]+=frame->nb_samples;
		}

		av_frame_unref(frame);
	}

	return error;
}

Channels & Wave::loadFfmpeg(const std::string & name,Channels & target,float skip,float length)
{
	av_register_all();

	int error=0;

	AVFormatContext *formatContext = NULL;

	if ((error = avformat_open_input(&formatContext,name.c_str(),NULL,0)) != 0)
	{
		LOG(logERROR) << "Could not open " << name << std::endl;
		return target;
	}

	LOG(logINFO) << "Reading " << name << std::endl;

	avformat_find_stream_info(formatContext,NULL);

	std::map<int,AVCodecContext *> codecContexts;
	std::map<int,Channels> channels;
	std::map<int,unsigned> position;

	for(int i=0;i<formatContext->nb_streams;i++)
	{
		if(formatContext->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_AUDIO)
		{
			LOG(logINFO) << "Stream:        " << i << std::endl;

			AVCodec * codec = avcodec_find_decoder(formatContext->streams[i]->codecpar->codec_id);
			if ( codec == NULL )
			{
				LOG(logERROR) << "No decoder found for this stream." << std::endl;
				continue;
			}
			LOG(logINFO) << "Codec:         " << codec->long_name << std::endl;
			AVCodecContext * codecContext = avcodec_alloc_context3(codec);
			if ( codecContext == NULL )
			{
				LOG(logERROR) << "Codec " << codec->long_name << " failed context allocation." << std::endl;
				continue;
			}

			if ((error = avcodec_parameters_to_context(codecContext,formatContext->streams[i]->codecpar)) != 0)
			{
				LOG(logERROR) << "Codec parameter could not be set, error " << error << std::endl;
				avcodec_close(codecContext);
				avcodec_free_context(&codecContext);
				continue;
			}

			codecContext->request_sample_fmt = av_get_alt_sample_fmt(codecContext->sample_fmt,0);

			if(( error = avcodec_open2(codecContext,codec,NULL))!=0)
			{
				LOG(logERROR) << "Opening codec failed, error " << error << std::endl;
				avcodec_close(codecContext);
				avcodec_free_context(&codecContext);
				continue;
			}

			LOG(logINFO) << "Sample format: " << av_get_sample_fmt_name(codecContext->sample_fmt) << std::endl;
			LOG(logINFO) << "Sample rate:   " << codecContext->sample_rate << std::endl;
			LOG(logINFO) << "Sample count:  " << formatContext->duration*codecContext->sample_rate/AV_TIME_BASE << std::endl;
			LOG(logINFO) << "Sample size:   " << av_get_bytes_per_sample(codecContext->sample_fmt) << std::endl;
			LOG(logINFO) << "Bit rate:      " << codecContext->bit_rate << std::endl;
			LOG(logINFO) << "Channels:      " << codecContext->channels << std::endl;
			LOG(logINFO) << "Planar:        " << av_sample_fmt_is_planar(codecContext->sample_fmt) << std::endl;
			LOG(logINFO) << "Padding:       " << codecContext->initial_padding << std::endl;
			LOG(logINFO) << "Delay:         " << codecContext->delay << std::endl;
			LOG(logINFO) << "trailing padding: " << codecContext->trailing_padding << std::endl;
												//LOG(logINFO) << "Float output:  " << (av_sample_fmt_is_planar(codecContext->sample_fmt) ? "yes" : "no") << std::endl;

			LOG(logDEBUG) << "Putting "<<codecContext << " in as " << i << std::endl;
			codecContexts[i]=codecContext;
			LOG(logDEBUG) << "Look at " << i << ": " << codecContexts[i] << std::endl;
			Channels newChannels;
			for(int j=0;j<codecContext->channels;j++)
				newChannels.push_back(Channel(codecContext->sample_rate, formatContext->duration*codecContext->sample_rate/AV_TIME_BASE));
			channels.insert(std::pair<int, Channels>(i, newChannels));
			position[i]=0;


		}
	}

	AVFrame * frame=NULL;
	if((frame=av_frame_alloc())==NULL)
	{
		for (std::map<int,AVCodecContext *>::iterator it=codecContexts.begin(); it!=codecContexts.end(); ++it)
		{
			avcodec_close(it->second);
			avcodec_free_context(&it->second);
			avformat_close_input(&formatContext);
			return target;
		}
	}


	AVPacket packet;

	av_init_packet(&packet);
	LOG(logDEBUG) << "Start read frame "<< std::endl;
	while (( error = av_read_frame(formatContext,&packet))!=AVERROR_EOF)
	{
		if(error)
		{
			LOG(logERROR) << "Error during read " << error << std::endl;
			break;
		}
		//LOG(logDEBUG) << "Stream index "<<packet.stream_index<< std::endl;
		if(codecContexts.count(packet.stream_index)==0)
		{
			//LOG(logDEBUG) << "skip"<< std::endl;

			av_packet_unref(&packet);
			continue;
		}
		//LOG(logDEBUG) << "Start sending "<< std::endl;

		int stream_index=packet.stream_index;
		if((error=avcodec_send_packet(codecContexts[packet.stream_index],&packet))==0)
		{
			av_packet_unref(&packet);
		} else
		{
			LOG(logERROR) << "Error codec send " << error << std::endl;
			break;
		}
		//LOG(logDEBUG) << "Start receiving "<< std::endl;
		//LOG(logDEBUG) << "Using codec " << codecContexts[stream_index] << std::endl;
		//LOG(logDEBUG) << "Using codec " << codecContexts[stream_index]->codec_name << std::endl;

		error=handleFrame(codecContexts[stream_index],frame,stream_index,channels,position);

		if(error!=AVERROR(EAGAIN))
		{
			LOG(logERROR) << "Receive error " << error << std::endl;
			break;
		}
	}


	LOG(logINFO) << "Done reading" << std::endl;


	for (std::map<int,AVCodecContext *>::iterator it=codecContexts.begin(); it!=codecContexts.end(); ++it)
	{
		for(int c=0;c<codecContexts[it->first]->channels;c++)
			if(channels[it->first][c].size()>position[it->first])
			{
				LOG(logWARNING) << "Resizing from "<<channels[it->first][c].size()<<" to "<<position[it->first]<< std::endl;
				channels[it->first][c]=channels[it->first][c].resizeTo(position[it->first]);
			}
		if((error=avcodec_send_packet(codecContexts[it->first],NULL))==0)
		{
			error=handleFrame(codecContexts[it->first],frame,it->first,channels,position);
			if(error!=AVERROR(EAGAIN) && error!=AVERROR_EOF)
			{
				LOG(logERROR) << "Receive error " << error << std::endl;
			}
		}
		if(position[it->first]<channels[it->first][0].size())
		{
			LOG(logWARNING) << "Expected size of " << channels[it->first][0].size() << " but read only " << position[it->first] << std::endl;
		}
		for(int j=0;j<channels[it->first].size();j++)
			target.push_back(channels[it->first][j]);
		avcodec_close(it->second);
		avcodec_free_context(&it->second);
	}

	av_frame_free(&frame);


	avformat_close_input(&formatContext);

	return target;
}

int Wave::saveAac(const std::string &name,Channel & channel,int bitrate)
{
	Channels channels=Channels(1);
	channels[0]=channel;
	return saveAac(name,channels,bitrate);
}

int encode(AVCodecContext *codecContext,AVFrame *frame,AVPacket *packet,FILE *file)
{
	int ret;
	ret=avcodec_send_frame(codecContext,frame);
	if(ret<0)
	{
		LOG(logERROR) << "Error sending frame to encoder"<<std::endl;
		return 1;
	}
	while(ret>=0)
	{
		ret=avcodec_receive_packet(codecContext,packet);
		if(ret==AVERROR(EAGAIN)||ret==AVERROR_EOF)
			return 0;
		else
			if(ret < 0)
			{
				LOG(logERROR) << "Error encoding frame" << std::endl;
				return 1;
			}
		fwrite(packet->data,1,packet->size,file);
		av_packet_unref(packet);
	}
	return 0;
}

/** Global timestamp for the audio frames */
static int64_t pts = 0;

int Wave::saveAac(const std::string &name,Channels & channels,int bitrate)
{
	AVIOContext *output_io_context;
	int error;

	avcodec_register_all();

	if ((error = avio_open(&output_io_context, name.c_str(), AVIO_FLAG_WRITE)) < 0)
	{
		LOG(logERROR) << "Could not open output file '"<<name<<"'"
				      << "(error '"<<av_err2str(error)<<"')"<<std::endl;
	    return error;
	}

	AVFormatContext *output_format_context;

	/*if (!(output_format_context = avformat_alloc_context()))
	{
		LOG(logERROR) << "Could not allocate output format context"<<std::endl;
	    return AVERROR(ENOMEM);
	}

	output_format_context->pb=output_io_context;*/

	AVOutputFormat *output_format;

	//if (!((output_format_context)->oformat = av_guess_format("adts",NULL,NULL)/*av_guess_format(NULL, name.c_str(),NULL)*/))
	if (!(output_format = av_guess_format("adts",NULL,NULL)/*av_guess_format(NULL, name.c_str(),NULL)*/))
	{
	    LOG(logERROR) << "Could not find output file format adts" << std::endl;
	    if (!(output_format = av_guess_format(NULL, name.c_str(),NULL)))
	    {
	    	    LOG(logERROR) << "Could not find output file format for " << name << std::endl;

	            avio_closep(&output_io_context/*&output_format_context->pb*/);
	            //avformat_free_context(output_format_context);
	            return error;
	    }
	}

	if ((error = avformat_alloc_output_context2(&output_format_context, output_format, "", NULL)) < 0)
	{
		LOG(logERROR) << "Could not allocate output format context"<<std::endl;
		return AVERROR(ENOMEM);
	}

	output_format_context->pb=output_io_context;

	//av_strlcpy(output_format_context->filename, name.c_str(),sizeof(output_format_context->filename));
	strncpy(output_format_context->filename, name.c_str(),sizeof(output_format_context->filename));

	const AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
	if(!codec)
	{
		LOG(logERROR) << "AAC Codec not found" << std::endl;
		return 1;
	}
	LOG(logINFO) << "Writing to "<<name << " using "<<codec->long_name<<std::endl;

	AVStream *stream;
	if (!(stream = avformat_new_stream(output_format_context, NULL)))
	{
		LOG(logERROR) << "Could not create new stream"<<std::endl;
	    error = AVERROR(ENOMEM);
	    avio_closep(&output_format_context->pb);
	   	avformat_free_context(output_format_context);
	   	return error;
	}

	stream->id=output_format_context->nb_streams-1;// why?

	AVCodecContext *codecContext=NULL;

	codecContext=avcodec_alloc_context3(codec);
	if(!codecContext)
	{
		LOG(logERROR) << "Could not allocate codec context" << std::endl;
		return 1;
	}

	codecContext->bit_rate=bitrate;
	LOG(logINFO) << "Using AAC bitrate of " << codecContext->bit_rate << std::endl;
	codecContext->sample_fmt=AV_SAMPLE_FMT_FLTP;//AV_SAMPLE_FMT_FLTP;
			//AV_SAMPLE_FMT_S16;

	bool found=false;

	for(const enum AVSampleFormat *p=codec->sample_fmts;!found && *p!=AV_SAMPLE_FMT_NONE;p++)
		found=(*p==codecContext->sample_fmt);

	if(!found)
	{
		LOG(logERROR) << "Could not select sample format" << std::endl;
		return 1;
	}

	int sampleRate=0;
	int optimalRate=unifiedSamplerate(channels);

	if(codec->supported_samplerates)
	{
		for(const int *p=codec->supported_samplerates;*p;p++)
			if(abs(optimalRate-*p)<abs(optimalRate-sampleRate))
				sampleRate=*p;
	} else
		sampleRate=44100;

	for(int i=0;i<channels.size();i++)
		if(channels[i].samplerate()!=sampleRate)
			channels[i]=channels[i].resampleTo(sampleRate);

	codecContext->sample_rate=sampleRate;
	LOG(logINFO) << "Using AAC samplerate of " << codecContext->sample_rate << std::endl;

	unsigned optimalChannels=channels.size();
	unsigned currentChannels=2;
	uint64_t channel_layout=AV_CH_LAYOUT_STEREO;

	if(codec->channel_layouts)
	{

		for(const uint64_t *p=codec->channel_layouts;*p;p++)
		{
			int channels=av_get_channel_layout_nb_channels(*p);
			if(channels>=optimalChannels && (currentChannels<optimalChannels
					                      || channels<currentChannels))
			{
				channel_layout=*p;
				currentChannels=channels;
			}
		}
	}

	codecContext->channel_layout = channel_layout;
	codecContext->channels=currentChannels;
	LOG(logINFO) << "Using AAC channel count " << codecContext->channels << std::endl;

	codecContext->strict_std_compliance=FF_COMPLIANCE_EXPERIMENTAL;

	codecContext->time_base.den=sampleRate;
	codecContext->time_base.num=1;

	LOG(logINFO) << "Default initial padding: " << codecContext->initial_padding << std::endl;

	codecContext->initial_padding=0;

	codecContext->codec_type=AVMEDIA_TYPE_AUDIO;

    if ((output_format_context)->oformat->flags & AVFMT_GLOBALHEADER)
        codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	if ((error=avcodec_open2(codecContext,codec,NULL))<0)
	{
		LOG(logERROR) << "Could not open codec "
				      << "(error '"<<(av_err2str(error))<<"')"<< std::endl;

		return 1;
	}

	error=avcodec_parameters_from_context(stream->codecpar,codecContext);

	/*FILE *f=fopen(name.c_str(),"wb");
	if(!f)
	{
		LOG(logERROR) << "Could not open target file "<<name << std::endl;

		avcodec_free_context(&codecContext);

		return 1;
	}*/

	output_format_context->duration=channels[0].size()*AV_TIME_BASE/channels[0].samplerate();
	output_format_context->audio_preload=0;

	if ((error = avformat_write_header(output_format_context, NULL)) < 0)
	{
		LOG(logERROR) << "Could not write output file header (error '"
				      << av_err2str(error) << "')"<<std::endl;
	    return error;
	}

	//AVPacket *packet=av_packet_alloc();
	AVPacket packet;
	av_init_packet(&packet);
	packet.data=NULL;
	packet.size=0;

	/*if(!packet)
	{
		LOG(logERROR) << "Could not allocate packet" << std::endl;

		fclose(f);
		avcodec_free_context(&codecContext);

		return 1;
	}*/

	AVFrame *frame=av_frame_alloc();

	if(!frame)
	{
		LOG(logERROR) << "Could not allocate frame" << std::endl;
		avio_closep(&output_format_context->pb);
		//fclose(f);
		//av_packet_free(&packet);
		av_packet_unref(&packet);
		avcodec_free_context(&codecContext);

		return 1;
	}

	unsigned remainingSamples=unifiedLength(channels);
	unsigned position=0;

	frame->nb_samples     = codecContext->frame_size;

	if(frame->nb_samples>remainingSamples)
		frame->nb_samples=remainingSamples;

	frame->format         = codecContext->sample_fmt;
	frame->channel_layout = codecContext->channel_layout;
	frame->sample_rate    = sampleRate;

	if(av_frame_get_buffer(frame,0)<0)
	{
		LOG(logERROR) << "Could not allocate audio data buffers" << std::endl;
		avio_closep(&output_format_context->pb);
		//fclose(f);
		av_frame_free(&frame);
		//av_packet_free(&packet);
		av_packet_unref(&packet);
		avcodec_free_context(&codecContext);

		return 1;
	}

    int64_t curr_pts=0;
    int encoded_pkt_counter = 1;

	while(remainingSamples>0)
	{
		if(frame->nb_samples>remainingSamples)
			frame->nb_samples=remainingSamples;

		frame->pts=pts;
		pts+=frame->nb_samples;

		if(av_frame_make_writable(frame)<0)
		{
			LOG(logERROR) << "Could not make frame writable"<<std::endl;
			//fclose(f);
			avio_closep(&output_format_context->pb);
			av_frame_free(&frame);
			//av_packet_free(&packet);
			av_packet_unref(&packet);
			avcodec_free_context(&codecContext);

			return 1;
		}

		//float* samples=(float*)frame->data[0];
		int pos=0;
		//LOG(logINFO) << "Position: " << position << " Length: "<<frame->nb_samples << " Remaining: "<<remainingSamples<< std::endl;
		for(int i=0;i<frame->nb_samples;i++)
		{
			for(int c=0;c<codecContext->channels;c++)
			{
				if(c<channels.size())
					//samples[pos++]
					((float*)frame->data[c])[pos]=channels[c][position]/32767.0;
				else
					//samples[pos++]=0;
					((float*)frame->data[c])[pos]=0;
			}
			pos++;
			position++;
			remainingSamples--;
		}
		int data_present;
		data_present=1;
		if((error=avcodec_send_frame(codecContext,frame))==0)
			error=avcodec_receive_packet(codecContext,&packet);
		else
		{
			LOG(logERROR) << "Could not encode frame (error '"
				        		      << av_err2str(error) << "')"<<std::endl;
			av_packet_unref(&packet);
				        avio_closep(&output_format_context->pb);
		    return error;
		}
		/*if ((error = avcodec_encode_audio2(codecContext, &packet,frame, &data_present)) < 0)
		{
	        LOG(logERROR) << "Could not encode frame (error '"
	        		      << av_err2str(error) << "')"<<std::endl;
	        av_packet_unref(&packet);
	        avio_closep(&output_format_context->pb);
	        return error;
		}*/
		if (/*data_present*/ error==0)
		{
			curr_pts=pts;//frame->nb_samples*(encoded_pkt_counter-1);
			packet.pts=packet.dts=curr_pts;
			if ((error = av_write_frame(output_format_context, &packet)) < 0)
			{
		        LOG(logERROR) << "Could write frame (error '"
		        		      << av_err2str(error) << "')"<<std::endl;
		        av_packet_unref(&packet);
		        avio_closep(&output_format_context->pb);
		        return error;
			}
			encoded_pkt_counter++;
		} else
		{
			if(error!=AVERROR(EAGAIN))
			{
				LOG(logERROR) << "Error receiving encoded packet (error '"
						      << av_err2str(error)<<"')"<<std::endl;
			}
		}
		av_packet_unref(&packet);
		av_init_packet(&packet);
		packet.data=NULL;
		packet.size=0;
/*		if(encode(codecContext,NULL,packet,f))
		{
			fclose(f);
			av_frame_free(&frame);
			av_packet_free(&packet);
			avcodec_free_context(&codecContext);

			return 1;
		}*/
	}
	LOG(logINFO) << "Flushing" << std::endl;
	// int data_present;


	if((error=avcodec_send_frame(codecContext,NULL))==0)
		do {
			error=avcodec_receive_packet(codecContext,&packet);
			if(error==0)
			{
				pts+=frame->nb_samples;
				curr_pts=pts;//frame->nb_samples*(encoded_pkt_counter-1);
				packet.pts=packet.dts=curr_pts;
				if((error=av_write_frame(output_format_context, &packet)) < 0)
				{
					LOG(logERROR) << "Could write frame (error '"
							        		      << av_err2str(error) << "')"<<std::endl;
				}

				encoded_pkt_counter++;
			}
 		} while(error==0);
	/*do {
	data_present=0;
	if ((error = avcodec_encode_audio2(codecContext, &packet,
										   NULL, &data_present)) < 0)
	{
		LOG(logERROR) << "Could not encode frame (error '"
					  << av_err2str(error) << "')"<<std::endl;
		av_packet_unref(&packet);
		return error;
	}
	if (data_present)
	{
		if ((error = av_write_frame(output_format_context, &packet)) < 0)
		{
			LOG(logERROR) << "Could write frame (error '"
						  << av_err2str(error) << "')"<<std::endl;
			av_packet_unref(&packet);
			return error;
		}
	}
	} while(data_present);*/
	/*av_packet_unref(&packet);
	av_init_packet(&packet);
	packet.data=NULL;
	packet.size=0;*/

	/*if(encode(codecContext,NULL,packet,f))
	{
		fclose(f);
		av_frame_free(&frame);
		av_packet_free(&packet);
		avcodec_free_context(&codecContext);

		return 1;
	}*/
	//fclose(f);

	av_write_trailer(output_format_context);

	av_frame_free(&frame);
	//av_packet_free(&packet);
	av_packet_unref(&packet);
	avio_closep(&output_format_context->pb);
	avcodec_free_context(&codecContext);
	return 0;
}
#endif // HAS_FFMPEG
