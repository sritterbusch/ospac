/**
 * @file		CrosstalkGate.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		9.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Crosstalk gate mutes less active channels
 */

#ifndef CROSSTALKGATE_H_
#define CROSSTALKGATE_H_

#include "Channel.h"

/**
 * @brief Simple and robust crosstalk gate
 *
 * Starting from this two channel example, where the second channel consists
 * of crosstalk from the first channel and a short original input.
 * @image html crosstalk-example.png
 * @image latex crosstalk-example.png "Exemplary two channel input" width=10cm
 *
 * The crosstalk gate decreases the volume of the passages in the second channel
 * where its channel activity is below its maximum.
 * @image html xgate-result.png
 * @image latex xgate-result.png "Result of crosstalk gate filter" width=10cm
 *
 */
class CrosstalkGate
{
public:
	/**
	 * Crosstalk gate based on downsampled energy level of channels:
	 * For each channel the l2 energy of all sample below the
	 * averaged l2 level of the signal is taken as silence level.
	 * Then the activity in a window is assumed as factor above this silence
	 * level. (This should be limited in future.)
	 * The maximum activity will gain 100%, all other channels will be
	 * muted linearily due to their activity.
	 * The actual muting is averaged on a mixsec window to soften changes.
	 * @param aChannels				audio channels to work on
	 * @param aDownsampleLevel		downsample factor
	 * @param windowsec				activity window (in seconds)
	 * @param mixsec				mixing average window (in seconds)
	 */
	static void gate(Channels &aChannels,unsigned aDownsampleLevel,double windowsec=0.1,double mixsec=0.1);

};

#endif /* CROSSTALKGATE_H_ */
