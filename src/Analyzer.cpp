/**
 * @file		Analyzer.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		15.3.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief 		Frequency band activity analysis
 */

#include <math.h>
#include <vector>
#include "Analyzer.h"
#include "Frequency.h"
#include "Log.h"


std::vector<double> Analyzer::bandedAnalysis(const Channel & c,
											 std::vector<float> f)
{
	float 		l2=c.l2norm();
	Channels	bands=Frequency::split(c,f);
	unsigned  	n=bands.size();

	std::vector<double> target(n);
	for(unsigned i=0;i<n;i++)
		target[i]=0;


	l2*=l2;

	for(unsigned i=0;i<c.size();i++)
	{
		if(sqr(c[i])>l2)
		{
			for(unsigned j=0;j<n;j++)
				target[j]+=sqr(bands[j][i]);
		}
	}


	double sum=1e-99;
	for(unsigned i=0;i<n;i++)
	{
		sum+=target[i];
		target[i]=sqrt(target[i]);
	}

	sum=sqrt(sum);

	for(unsigned i=0;i<n;i++)
		target[i]/=sum;


	for(unsigned i=0;i<n;i++)
	{
		if(i==0)
		{
			LOG(logINFO) << "0-" << f[0] << "\t" << target[i]*100 << std::endl;
		} else if(i<n-1)
		{
			LOG(logINFO) << f[i-1] << "-" << f[i] << "\t" << target[i]*100 << std::endl;
		} else
		{
			LOG(logINFO) << f[i-1] << "-" << c.samplerate()/2 << "\t" << target[i]*100 << std::endl;
		}
	}


	return target;

}


std::vector<double> Analyzer::bandedAnalysis(const Channel & c)
{
	std::vector<float> f(4);
	f[0]=100;
	f[1]=500;
	f[2]=2500;
	f[3]=4000;
	return bandedAnalysis(c,f);
}

