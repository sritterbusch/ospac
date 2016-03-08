/**
 * @file		CrosstalkFilter.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		6.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Filter to actively identify crosstalk in other channels
 */

#ifndef CROSSTALKFILTER_H_
#define CROSSTALKFILTER_H_

#include <vector>
#include <string>
#include "Channel.h"

/**
 * @brief The CrosstalkFilter tries to identify time-delayed crosstalk of each channel in other channels
 * by comparing integrals of l2power and mutes identified sections.
 *
 * Main issue is that it does not recognize the channel with more quality. It tends to mute channels
 * less with more open mics than channels with directed mics. This is unwanted behaviour.
 *
 * Starting from this two channel example, where the second channel consists
 * of crosstalk from the first channel and a short original input.
 * @image html crosstalk-example.png
 * @image latex crosstalk-example.png "Exemplary two channel input" width=10cm
 *
 * The crosstalk filter decreases the volume of the passages where mainly
 * previous signals from other channels are detected.
 * @image html xfilter-result.png
 * @image latex xfilter-result.png "Result of crosstalk filter" width=10cm
 *
 *
 */
class CrosstalkFilter
{
private:
	unsigned downsampleLevel;
	Channels & channels;
	Channels downsample;
	Channels muteFactor;
	Channels activity;
	unsigned workWindow;
	unsigned minShift;
	unsigned maxShift;
	float	 muteStartRatio;
	float    muteFullRatio;

	std::vector<double> l2norm;
	std::vector<double> l2upnorm;
	std::vector<double> l2downnorm;

public:
	/** CrosstalkFilter Constructor with sample settings (please consider using the variant with physical settings!)
	 * 		\param aChannels 			std::vector of channels the filter will operate on
	 * 		\param aDownsampleLevel 	factor of downsampling for the analysis
	 * 		\param aWorkwindow         window size in samples the comparism is made on
	 * 		\param aMinShift          number of minimum time-delay in samples (dependent on aDownsampleLevel)
	 * 		\param aMaxShift          number of maximum time-delay in samples (dependent on aDownsampleLevel)
	 * 		\param aMuteStartRatio    ratio of integrals of original to rest from where muting will linearly start
	 * 		\param aMuteFullRatio     ratio of integrals of original to rest where linearity ends and full mute will occur
	 *
	 * 		\return CrosstalkFilter object
	 * 		\sa analyze() and mute()
	 */
	CrosstalkFilter(Channels &aChannels,unsigned aDownsampleLevel,unsigned aWorkwindow,unsigned aMinShift,unsigned aMaxShift,float aMuteStartRatio=1.2,float aMuteFullRatio=1.5);

	/** CrosstalkFilter Constructor with sample settings (please consider using the variant with physical settings!)
	 * 		\param aChannels 		  std::vector of channels the filter will operate on
	 * 		\param aDownsampleLevel   factor of downsampling for the analysis
	 * 		\param windowsec          window size in seconds the comparism is made on
	 * 		\param mindistance        minimum assumed spatial distance between channels (in meters)
	 * 		\param maxdistance        maximum assumed spatial distance between channels (in meters)
	 * 		\param aMuteStartRatio    ratio of integrals of original to rest from where muting will linearly start
	 * 		\param aMuteFullRatio     ratio of integrals of original to rest where linearity ends and full mute will occur
	 *
	 * 		\return CrosstalkFilter object
	 * 		\sa analyze() and mute()
	 */
	CrosstalkFilter(Channels &aChannels,unsigned aDownsampleLevel,double windowsec=0.1,double mindistance=1.5,double maxdistance=5.0,float aMuteStartRatio=1.2,float aMuteFullRatio=1.5);

	void	analyze2();
	void	analyze();
	void	save(std::string);
	void	mute();

private:
	template<class T>
	static T sqr(const T&a){return a*a;}

	void 	prepareVectors();


};

#endif /* CROSSTALKFILTER_H_ */
