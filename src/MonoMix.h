/**
 * @file		MonoMix.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		12.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Mono mix-down
 */

#ifndef MONOMIX_H_
#define MONOMIX_H_

#include "Channel.h"

/**
 * @brief Create mono mix-down
 */
class MonoMix
{
private:
	Channels target;
public:
	MonoMix();

	/**
	 * Mix one channel into the mix-down
	 * @param c			Channel
	 * @param factor	intensity of rendering
	 */
	void 		mix(Channel &c,float factor=1.0);

	/**
	 * Mix several channels into the mix-down
	 * @param c			Channels
	 */
	void        mix(Channels &c);

	/**
	 * Return current mono mix-down
	 * @return mix-down Channel
	 */
	Channels &	getTarget() { return target; }
};

#endif /* MONOMIX_H_ */
