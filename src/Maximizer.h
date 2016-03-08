/**
 * @file		Maximizer.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		7.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief       Amplification and normalization
 */

#ifndef MAXIMIZER_H_
#define MAXIMIZER_H_

#include "Channel.h"

/**
 * @brief Amplification with constant factor and soft clipping by sigmoid function
 *
 * This is the result of the factor filter:
 * @image html factor-result.png
 * @image latex factor-result.png "Result of factors 2 and 4" width=10cm
 *
 * This is the result of the normalize filter:
 * @image html normalize-result.png
 * @image latex normalize-result.png "Result of normalization" width=10cm
 */
class Maximizer
{
public:
	/**
	 * Multiplication of signal by constant factor and soft limiting by
	 * sigmoid function:
	 * @f[ v(x):=\frac{c\cdot u(x)}{\sqrt[n]{1+\left|\frac{c\cdot u(x)}{32000}\right|^n}} @f]
	 * @param channel audio segment to be multiplied
	 * @param factor factor
	 * @param order of sigmoid function
	 */
	static void amplify(Channel &channel,float factor,int order=4);

	/**
	 * Multiplication of signal by constant factor and soft limiting by
	 * sigmoid function:
	 * @f[ v(x):=\frac{c\cdot u(x)}{\sqrt[n]{1+\left|\frac{c\cdot u(x)}{32000}\right|^n}} @f]
	 * @param channels audio segments to be multiplied
	 * @param factor factor
	 * @param order of sigmoid function
	 */
	static void amplify(Channels &channels,float factor,int order=4);


	/**
	 * Multiplication of signal by constant factor and soft limiting by
	 * sigmoid multiplied with quadratic root function:
	 * @f[ v(x):=\frac{c\cdot u(x)}{\sqrt[n]{1+\left|\frac{c\cdot u(x)}{32000}\right|^n}} \cdot \frac{\left(u(x)\right)^2}{\left(u(x)\right)^2+\varrho^2} @f]
	 * @param channel audio segment to be multiplied
	 * @param factor factor
	 * @param minlevel noise voltage level
	 * @param order of sigmoid function
	 */
	static void amplifyDenoise(Channel &channel,float factor,float minlevel,int order=4);

	/**
	 * Multiplication of signal by constant factor and soft limiting by
	 * sigmoid multiplied with quadratic root function:
	 * @f[ v(x):=\frac{c\cdot u(x)}{\sqrt[n]{1+\left|\frac{c\cdot u(x)}{32000}\right|^n}} \cdot \frac{\left(u(x)\right)^2}{\left(u(x)\right)^2+\varrho^2} @f]
	 * @param channels audio segments to be multiplied
	 * @param factor factor
	 * @param minlevel noise voltage level
	 * @param order of sigmoid function
	 */
	static void amplifyDenoise(Channels &channels,float factor,float minlevel,int order=4);

	/**
	 * Normalize the maximum absolute value to given level.
	 * @param channel audio segment to be normalized
	 * @param level target voltage level
	 */
	static void normalize(Channel &channel,float level=32767.);

	/**
	 * Normalize the maximum absolute value to given level.
	 * @param channels audio segments to be normalized
	 * @param level target voltage level
	 */
	static void normalize(Channels &channels,float level=32767.);

private:
	static double		expander(float c,float factor,int order);
	static double		expanderDenoiser(float c,float factor,float minlevel,int order);

	template<class T>
	static T sqr(const T&a){return a*a;}
};

#endif /* MAXIMIZER_H_ */
