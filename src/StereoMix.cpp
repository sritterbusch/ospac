/**
 * @file		StereoMix.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		7.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Stereo mix-down
 */

#include <math.h>

#include "StereoMix.h"
#include "Physics.h"
#include "Log.h"
#include "Frequency.h"

StereoMix::StereoMix() : target(2)
{
}

// http://www.feilding.net/sfuad/musi3012-01/html/lectures/008_hearing_III.htm

void StereoMix::mix(Channel &c,float leftFactor,float rightFactor,float leftDistance,float rightDistance)
{
	if(target[0].size()==0 || c.samplerate()>target[0].samplerate())
	{
		target[0]=target[0].resampleTo(c.samplerate());
		target[1]=target[1].resampleTo(c.samplerate());
		LOG(logDEBUG) << "Target samplerate set to " << c.samplerate() << std::endl;
	}
	if(c.samplerate()<target[0].samplerate())
		c.resampleTo(target[0].samplerate());
	if(target[0].size()<c.size())
	{
		target[0]=target[0].resizeTo(c.size());
		target[1]=target[1].resizeTo(c.size());
		LOG(logDEBUG) << "Target size set to " << c.size()/c.samplerate() << "s"<< std::endl;
	}

	unsigned leftShift=Physics::meterToSec(leftDistance)*c.samplerate();
	unsigned rightShift=Physics::meterToSec(rightDistance)*c.samplerate();

	for(unsigned i=0;i<c.size();i++)
	{
		if(i>=leftShift)
			target[0][i]+=leftFactor*c[i-leftShift];
		if(i>=rightShift)
			target[1][i]+=rightFactor*c[i-rightShift];
	}
}

void StereoMix::mixBanded(Channel &c,float leftFactor,float rightFactor,float leftDistance,float rightDistance)
{
	std::vector<float> freqs(3);
	freqs[0]=500;
	freqs[1]=2000;
	freqs[2]=7000;

	Channels b=Frequency::split(c,freqs);

	if(target[0].size()==0 || c.samplerate()>target[0].samplerate())
	{
		target[0]=target[0].resampleTo(c.samplerate());
		target[1]=target[1].resampleTo(c.samplerate());
		LOG(logDEBUG) << "Target samplerate set to " << c.samplerate() << std::endl;
	}
	if(c.samplerate()<target[0].samplerate())
		c.resampleTo(target[0].samplerate());
	if(target[0].size()<c.size())
	{
		target[0]=target[0].resizeTo(c.size());
		target[1]=target[1].resizeTo(c.size());
		LOG(logDEBUG) << "Target size set to " << c.size()/c.samplerate() << "s"<< std::endl;
	}

	unsigned leftShift=Physics::meterToSec(leftDistance)*c.samplerate();
	unsigned rightShift=Physics::meterToSec(rightDistance)*c.samplerate();

	float l0=pow(leftFactor,0.4);
	float l1=pow(leftFactor,0.8);
	float l2=pow(leftFactor,1.2);
	float l3=pow(leftFactor,1.6);

	float r0=pow(rightFactor,0.4);
	float r1=pow(rightFactor,0.8);
	float r2=pow(rightFactor,1.2);
	float r3=pow(rightFactor,1.6);

	for(unsigned i=0;i<c.size();i++)
	{
		if(i>=leftShift)
		{
			if(leftShift==0)
			{
				target[0][i]+=leftFactor*c[i-leftShift];
			} else
			{
				target[0][i]+=l0*b[0][i-leftShift]
							 +l1*b[1][i-leftShift]
							 +l2*b[2][i-leftShift]
							 +l3*b[3][i-leftShift];
			}
		}
		if(i>=rightShift)
		{
			if(rightShift==0)
			{
				target[1][i]+=rightFactor*c[i-rightShift];
			} else
			{
				target[1][i]+=r0*b[0][i-rightShift]
							 +r1*b[1][i-rightShift]
							 +r2*b[2][i-rightShift]
							 +r3*b[3][i-rightShift];

			}
		}
	}
}


void StereoMix::mix(Channels &c,bool spatial,bool banded)
{
	if(c.size()==0)
		return;

	const int sets=3;
	const int vars=4;

	static const double settingsSpatial[sets][vars]={
			{0.9,1,0.03,0},
			{0.95,0.95,0.0015,0.0015},
			{1,0.9,0,0.03}};

	static const double settingsStereo[sets][vars]={
			{0.9,1.1,0.00,0},
			{1.0,1.0,0.0000,0.0000},
			{1.1,0.9,0,0.00}};

	double settings[sets][vars];

	if(spatial)
		for(int i=0;i<sets;i++)
			for(int j=0;j<vars;j++)
				settings[i][j]=settingsSpatial[i][j];
	else
		for(int i=0;i<sets;i++)
			for(int j=0;j<vars;j++)
				settings[i][j]=settingsStereo[i][j];

	if(c.size()==1)
		mix(c[0],1,1,0,0);
	else
	{
		for(unsigned i=0;i<c.size();i++)
		{
			double p=double(i)*(sets-1)/(c.size()-1);
			int    pi=int(p);
			if(pi==sets)
				pi--;
			double f=p-pi;
			LOG(logINFO) << "Mixing channel " << i
					     << " Vl=" << settings[pi][0]*(1-f)+settings[pi+1][0]*f
						 << " Vr=" << settings[pi][1]*(1-f)+settings[pi+1][1]*f
						 << " tl=" << settings[pi][2]*(1-f)+settings[pi+1][2]*f << "s"
						 << " tr=" << settings[pi][3]*(1-f)+settings[pi+1][3]*f << "s" << std::endl;

			if(banded)
				mixBanded(c[i],settings[pi][0]*(1-f)+settings[pi+1][0]*f
												,settings[pi][1]*(1-f)+settings[pi+1][1]*f
												,settings[pi][2]*(1-f)+settings[pi+1][2]*f
												,settings[pi][3]*(1-f)+settings[pi+1][3]*f);
			else
				mix(c[i],settings[pi][0]*(1-f)+settings[pi+1][0]*f
						,settings[pi][1]*(1-f)+settings[pi+1][1]*f
						,settings[pi][2]*(1-f)+settings[pi+1][2]*f
						,settings[pi][3]*(1-f)+settings[pi+1][3]*f);
		}
	}
}

