/**
 * @file		StereoMix.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		7.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Stereo mix-down
 */

#ifndef STEREOMIX_H_
#define STEREOMIX_H_

#include "Channel.h"

/**
 * @brief  Create stereo mixdown of channels
 */
class StereoMix
{
private:
	Channels target;
public:
	/**
	 * Create initial target
	 */
	StereoMix();

	/**
	 * Mix single Channel into the target
	 * @param c 			Channel
	 * @param leftFactor	Rendering intensity left target channel
	 * @param rightFactor	Rendering intensity right target channel
	 * @param leftDistance	Distance in meter from left channel
	 * @param rightDistance Distance in meter from right channel
	 */
	void 		mix(Channel &c,float leftFactor,float rightFactor,float leftDistance,float rightDistance);

	/**
	 * Mix single Channel into target with frequency dependence
	 * @param c				Channel
	 * @param leftFactor	Rendering intensity left target channel
	 * @param rightFactor	Rendering intensity right target channel
	 * @param leftDistance	Distance in meter from left channel
	 * @param rightDistance	Distance in meter from right channel
	 */
	void 		mixBanded(Channel &c,float leftFactor,float rightFactor,float leftDistance,float rightDistance);

	/**
	 * Mix channels into target using equidistant positions
	 * @param c			Channels
	 * @param spatial	Use spatial delay?
	 * @param banded	Use frequency dependence?
	 */
	void        mix(Channels &c,bool spatial=false,bool banded=false);

	/**
	 * Request current stereo mixdown
	 * @return stereo mixdown
	 */
	Channels &	getTarget() { return target; }
};

#endif /* STEREOMIX_H_ */
