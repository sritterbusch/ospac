/**
 * @file		Maximizer.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		7.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief       Amplification and normalization
 */

#include <math.h>

#include "Maximizer.h"

double Maximizer::expander(float c,float factor,int order) // Sigmoid-Funktion
{
	switch(order)
	{
	case 1:
		return factor*c*32000/(32000.+factor*fabs(c));
	case 2:
		return factor*c*32000/sqrt(sqr(32000.)+sqr(factor)*sqr(c));
	case 4:
		return factor*c*32000/sqrt(sqrt(sqr(sqr(32000.))+sqr(sqr(factor*c)) ));
	case 8:
		return factor*c*32000/sqrt(sqrt(sqrt(sqr(sqr(sqr(32000.)))+sqr(sqr(sqr(factor*c))) )));
	default:
		return factor*c*32000/pow(pow(32000.,order)+pow(factor*c,order),1./order);
	}
}

double Maximizer::expanderDenoiser(float c,float factor,float minlevel,int order) // Sigmoid-Funktion mit mehrfacher Nullstelle an 0
{
	float cs=sqr(c);

	switch(order)
	{
	case 1:
		return factor*c*32000/(32000.+factor*fabs(c))*cs/(sqr(minlevel)+cs);
	case 2:
		return factor*c*32000/sqrt(sqr(32000.)+sqr(factor)*cs)*cs/(sqr(minlevel)+cs);
	case 4:
		return factor*c*32000/sqrt(sqrt(sqr(sqr(32000.))+sqr(sqr(factor)*cs) ))*cs/(sqr(minlevel)+cs);
	case 8:
		return factor*c*32000/sqrt(sqrt(sqrt(sqr(sqr(sqr(32000.)))+sqr(sqr(sqr(factor)*cs)) )))*cs/(sqr(minlevel)+cs);
	default:
		return factor*c*32000/pow(pow(32000.,order)+pow(factor*c,order),1./order)*cs/(sqr(minlevel)+cs);
	}
}


void Maximizer::amplify(Channel &c,float factor,int order)
{
	for(unsigned i=0;i<c.size();i++)
		c[i]=expander(c[i],factor,order);
}

void Maximizer::amplify(Channels &c,float factor,int order)
{
	unsigned samplerate=0;
	unsigned length=0;
	for(unsigned i=0;i<c.size();i++)
		if(c[i].samplerate()>samplerate)
			samplerate=c[i].samplerate();

	for(unsigned i=0;i<c.size();i++)
		if(c[i].samplerate()!=samplerate)
			c[i]=c[i].resampleTo(samplerate);

	for(unsigned i=0;i<c.size();i++)
		if(c[i].size()>length)
			length=c[i].size();

	for(unsigned j=0;j<length;j++)
	{
		float localfactor=factor;
		for(unsigned i=0;i<c.size();i++)
		{
			if(c[i][j]!=0)
			{
				float f=expander(c[i][j],factor,order)/c[i][j];
				if(f<localfactor)
					localfactor=f;
			}
		}
		for(unsigned i=0;i<c.size();i++)
			c[i][j]*=localfactor;
	}
}

void Maximizer::amplifyDenoise(Channel &c,float factor,float minlevel,int order)
{
	for(unsigned i=0;i<c.size();i++)
		c[i]=expanderDenoiser(c[i],factor,minlevel,order);
}

void Maximizer::amplifyDenoise(Channels &c,float factor,float minlevel,int order)
{
	for(unsigned i=0;i<c.size();i++)
		amplifyDenoise(c[i],factor,minlevel,order);
}

void Maximizer::normalize(Channels & c,float level)
{
	float max=1e-10;
	for(unsigned i=0;i<c.size();i++)
	{
		float e=c[i].linfnorm();
		if(e>max)
			max=e;
	}

	if(max!=level)
	{
		float factor=level/max;

		for(unsigned i=0;i<c.size();i++)
			for(unsigned j=0;j<c[i].size();j++)
				c[i][j]*=factor;
	}
}

void Maximizer::normalize(Channel & c,float level)
{
	float max=1e-10;
	for(unsigned i=0;i<c.size();i++)
		if(fabs(c[i])>max)
			max=fabs(c[i]);

	if(max!=level)
	{
		float factor=level/max;
		for(unsigned i=0;i<c.size();i++)
			c[i]*=factor;
	}
}
