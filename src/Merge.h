/**
 * @file		Merge.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		11.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief       Merging of audio segments either with overlap or fading
 */


#ifndef MERGE_H_
#define MERGE_H_

#include "Channel.h"

/**
 * @brief Merging of sound data segments (overlapping or fading)
 */
class Merge
{
public:
	/**
	 * Render the overlap (i.e. both on full volume) of two sound data segments
	 * @param a prior sound data segment
	 * @param b later sound data segment
	 * @param sec seconds of overlap
	 * @return resulting overlapped sound data segment
	 */
	static Channels overlap(Channels &a,Channels &b,float sec);

	/**
	 * Render the fading (i.e. with de- and increasing volume) of two sound
	 * data segments
	 * @param a prior sound data segment
	 * @param b later sound data segment
	 * @param sec seconds of fading
	 * @return resulting faded sound data segment
	 */
	static Channels fade(Channels &a,Channels &b,float sec);

	/**
	 * Render two channel segments in parallel
	 * @param a first sound data segment
	 * @param b second sound data segment
	 * @return resulting faded sound data segment
	 */
	static Channels parallel(Channels &a,Channels &b);

private:
	static int    unifySamplerate(Channels &a,Channels &b);

};

#endif /* MERGE_H_ */
