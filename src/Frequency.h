/**
 * @file		Frequency.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		14.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Frequency filters
 */


#ifndef FREQUENCY_H_
#define FREQUENCY_H_

#include "Channel.h"

/**
 * @brief Frequency filter class
 */
class Frequency
{
private:
	static Channel	convolution(const Channel &a,const Channel &kernel);
	static double   kernelF(double i,double f,double M);
	static double   kernel0(double f);
public:
	/**
	 * Split the given channel in a high-frequency and low-frequency part
	 * @param a given channel
	 * @param cutoff frequency
	 * @param width  transition bandwidth
	 * @return two channels with lower and higher frequency part
	 */
	static Channels split(const Channel &a,float cutoff,float width=1000);

	/**
	 * Band filter a given channel with respect to cutoff frequencies
	 * @param a given channel
	 * @param cutoff frequency vector (strictly ascending frequencies!)
	 * @param width transition bandwidth
	 * @return band filtered channels
	 */
	static Channels split(Channel a,std::vector<float> cutoff,float width=1000);
};

#endif /* FREQUENCY_H_ */
