/**
 * @file		Equalizer.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		3.3.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Equalizer for sound enhancement
 */


#include "Equalizer.h"
#include "Frequency.h"
#include "MonoMix.h"

Channel Equalizer::bandedEqualizer(const Channel & c,
			std::vector<float> frequencies,
			std::vector<float> factors)
{
	Channels bands=Frequency::split(c,frequencies);

	MonoMix target;
	for(unsigned i=0;i<factors.size() && i<bands.size();i++)
		target.mix(bands[i],factors[i]);

	return target.getTarget()[0];
}

Channel Equalizer::voiceEnhance(const Channel & c)
{
	// Attenuation similar to
	// https://larryjordan.com/articles/eq-warm-a-voice-and-improve-diction/
	std::vector<float> freqs(4);
	freqs[0]=100;
	freqs[1]=400;
	freqs[2]=3000;
	freqs[3]=4000;
	std::vector<float> factors(5);
	factors[0]=0.75;
	factors[1]=1.5;
	factors[2]=1;
	factors[3]=1.75;
	factors[4]=0.75;
	return bandedEqualizer(c,freqs,factors);
}

Channels Equalizer::voiceEnhance(const Channels & c)
{
	Channels target(c.size());
	for(unsigned i=0;i<c.size();i++)
		target[i]=voiceEnhance(c[i]);
	return target;
}
