/**
 * @file		SelectiveLeveler.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		7.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Selective Leveler working on windowed l2 energy of signal
 */

#ifndef SELECTIVELEVELER_H_
#define SELECTIVELEVELER_H_

#include "Channel.h"

/**
 * @brief  Selective Leveling by windowed average l2 energy
 * Contains experimental code for constant leveling in tolerance area
 * @image html leveler-result.png
 * @image latex leveler-result.png "Result of selective leveler" width=10cm
 */
class SelectiveLeveler
{
public:
	/**
	 * Compute windowed average l2 energy. If the energy is below silent
	 * fraction, the signal is muted. If the energy is between silent fraction
	 * to minFraction compared to the maximal windows l2 energy it is linearily
	 * damped. The actual damping factor is windowed by forward and backwards
	 * window interval.
	 * @param aChannels channels to do the individual leveling on
	 * @param targetL2 target average l2 energy
	 * @param windowSec window size in seconds for l2 average energy
	 * @param minFraction fraction compared to l2 maximal value assumed signal
	 * @param silentFraction fraction compared to l2 maximal value assumed silence
	 * @param forwardWindowSec average forward part of window for factor application
	 * @param backWindowSec average backward part of window for factor application
	 */
	static void level(Channels &aChannels,float targetL2,double windowSec,float minFraction,float silentFraction,float forwardWindowSec,float backWindowSec);

	/**
	 * Compute windowed average l2 energy. If the energy is below silent
	 * fraction, the signal is muted. If the energy is between silent fraction
	 * to minFraction compared to the maximal windows l2 energy it is linearily
	 * damped. The actual damping factor is windowed by forward and backwards
	 * window interval.
	 * @param aChannel channel to do the individual leveling on
	 * @param targetL2 target average l2 energy
	 * @param windowSec window size in seconds for l2 average energy
	 * @param minFraction fraction compared to l2 maximal value assumed signal
	 * @param silentFraction fraction compared to l2 maximal value assumed silence
	 * @param forwardWindowSec average forward part of window for factor application
	 * @param backWindowSec average backward part of window for factor application
	 */
	static void level(Channel &aChannel,float targetL2,double windowSec,float minFraction,float silentFraction,float forwardWindowSec,float backWindowSec);

	/**
	 * Compute windowed average l2 energy. If the energy is below silent
	 * fraction, the signal is muted. If the energy is between silent fraction
	 * to minFraction compared to the maximal windows l2 energy it is linearily
	 * damped. The actual damping factor is windowed by forward and backwards
	 * window interval. This function each considers two channels for analysis.
	 * @param aChannels channels to do the individual leveling on
	 * @param targetL2 target average l2 energy
	 * @param windowSec window size in seconds for l2 average energy
	 * @param minFraction fraction compared to l2 maximal value assumed signal
	 * @param silentFraction fraction compared to l2 maximal value assumed silence
	 * @param forwardWindowSec average forward part of window for factor application
	 * @param backWindowSec average backward part of window for factor application
	 */
	static void levelStereo(Channels &aChannels,float targetL2,double windowSec,float minFraction,float silentFraction,float forwardWindowSec,float backWindowSec);

	/**
	 * Compute windowed average l2 energy. If the energy is below silent
	 * fraction, the signal is muted. If the energy is between silent fraction
	 * to minFraction compared to the maximal windows l2 energy it is linearily
	 * damped. The actual damping factor is windowed by forward and backwards
	 * window interval. This function each considers two channels for analysis.
	 * @param aChannel left channel to do the joint leveling on
	 * @param bChannel right channel to do the joint leveling on
	 * @param targetL2 target average l2 energy
	 * @param windowSec window size in seconds for l2 average energy
	 * @param minFraction fraction compared to l2 maximal value assumed signal
	 * @param silentFraction fraction compared to l2 maximal value assumed silence
	 * @param forwardWindowSec average forward part of window for factor application
	 * @param backWindowSec average backward part of window for factor application
	 */
	static void levelStereo(Channel &aChannel,Channel &bChannel,float targetL2,double windowSec,float minFraction,float silentFraction,float forwardWindowSec,float backWindowSec);
private:
	template<class T>
	static T sqr(const T&a){return a*a;}
	template<class T>
	static T min(const T&a,const T&b){return a<b?a:b;}
};

#endif /* SELECTIVELEVELER_H_ */
