/**
 * @file		Equalizer.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		3.3.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Equalizer for sound enhancement
 */

#ifndef EQUALIZER_H_
#define EQUALIZER_H_

#include <vector>

#include "Channel.h"

/**
 * @brief  Preset equalizer using frequency banding
 */
class Equalizer
{
public:
	/**
	 * Amplification for seperate frequency bands
	 * @param c				audio channel to work on
	 * @param frequencies	n cut-off frequencies
	 * @param factors		n+1 amplication factors
	 * @return	resulting audio channel
	 */
	static Channel bandedEqualizer(const Channel & c,
			std::vector<float> frequencies,
			std::vector<float> factors);
	/**
	 * Preset equalizer for voice channel
	 * @param c	audio channel to work on
	 * @return	resulting audio channel
	 */
	static Channel voiceEnhance(const Channel & c);

	/**
	 * Preset equalizer for voice channels
	 * @param c	audio channels to work on
	 * @return	resulting audio channel
	 */
	static Channels voiceEnhance(const Channels & c);
};

#endif /* EQUALIZER_H_ */
