/**
 * @file		Frequency.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		14.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Frequency filters
 */

#include <math.h>


#include "Frequency.h"
#include "Log.h"
#include "Wave.h"

double   Frequency::kernelF(double i,double f,double M)
{
	if(i==M/2)
		return kernel0(f);


	return sin(2*M_PI*f*(i-M/2))/(i-M/2)
		  *(0.42-0.5*cos(2*M_PI*i/M)+0.08*cos(4*M_PI*i/M));
}

double   Frequency::kernel0(double f)
{
	return 2*M_PI*f;
}


Channel	Frequency::convolution(const Channel &a,const Channel &kernel)
{
	Channel target(a);
	int m2=kernel.size()/2;
	for(unsigned x=0;x<a.size();x++)
	{
		double sum=0;
		for(unsigned y=0;y<kernel.size() && y<=x+m2 && x+m2-y<a.size();y++)
			sum+=a[x+m2-y]*kernel[y];
		target[x]=sum;
	}
	return target;
}

unsigned Frequency::windowSize(const Channel & a,float f,float width)
{
	if(width>f)
		width=f;

	f=f/a.samplerate();

	float BW=width/a.samplerate();
	int    N=4/BW;
	if(N==0)
		N=1;
	return N;
}

Channels Frequency::split(const Channel & a,float f,float width,bool fade)
{
	if(width>f)
			width=f;
	LOG(logDEBUG) << "Frequency split at " << f << "Hz "
			      << " width "<< width << "Hz" << std::endl;
	// http://www.dspguide.com/ch16/2.htm

	unsigned N=windowSize(a,f,width);

	f=f/a.samplerate();

	LOG(logDEBUG) << "Convolution window size " << N << std::endl;

	Channel	kernel(a.samplerate(),N);
	double sum=0;
	for(unsigned i=0;i<N;i++)
	{
		kernel[i]=kernelF(i,f,N);
		sum+=kernel[i];
		//LOG(logDEBUG) << "Kernel " << i << " " << kernel[i] << std::endl;
	}
	//LOG(logDEBUG) << "sum: "<< sum << std::endl;
	for(unsigned i=0;i<N;i++)
		kernel[i]/=sum;

	Channels target(2);
	target[0]=convolution(a,kernel);
	target[1]=a;

	if(fade)
	{
		for(unsigned i=0;i<N*2 && i<target[0].size();i++)
		{
			double f=(double(i)-N)/N;
			if(f<0)
				f=0;

			target[0][i]*=f;
			target[0][target[0].size()-1-i]*=f;
		}
	}

	for(unsigned x=0;x<a.size();x++)
		target[1][x]-=target[0][x];

	Channel temp=convolution(target[1],kernel);

	if(fade)
	{
		for(unsigned i=0;i<N*2 && i<temp.size();i++)
		{
			double f=(double(i)-N)/N;
			if(f<0)
				f=0;

			temp[i]*=f;
			temp[temp.size()-1-i]*=f;
		}
	}


	for(unsigned x=0;x<a.size();x++)
	{
		target[0][x]+=temp[x];
		target[1][x]-=temp[x];
	}

	return target;

}

Channels Frequency::split(Channel a,std::vector<float> cutoff,float width,bool fade)
{
	Channels target(cutoff.size()+1);
	unsigned i;
	for(i=0;i<cutoff.size();i++)
	{
		Channels temp=split(a,cutoff[i],width,false);
		target[i]=temp[0];
		a=temp[1];
		/*temp=split(a,cutoff[i]);
		for(unsigned j=0;j<a.size();j++)
			target[i][j]+=temp[0][j];
		a=temp[1];*/
		if(fade)
		{
			unsigned N=windowSize(a,cutoff[i],width);
			for(unsigned j=0;j<N*2 && j<target[i].size();j++)
			{
				double f=(double(j)-N)/N;
				if(f<0)
					f=0;

				target[i][j]*=f;
				target[i][target[i].size()-1-j]*=f;
			}
		}
	}
	target[i]=a;


	//Wave::save("bands.wav",target);

	return target;
}
