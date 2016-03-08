/**
 * @file		Plot.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		6.3.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Simple plots of audio channels
 */

#ifndef PLOT_H_
#define PLOT_H_

#include <vector>

#include "Channel.h"

/**
 * @brief Simple plots of audio channels
 */
class Plot
{
private:
	static std::vector<std::vector<unsigned> > histogramm(const Channel &channel,unsigned sizeX,unsigned sizeY,unsigned size=0);
public:
	/**
	 * Create PGM plot of audio channels
	 * @param channels	the channels to plot
	 * @param name		target file name
	 * @param sizeX		width
	 * @param sizeY		height per channel
	 */
	static void createPGMPlot(const Channels &channels, std::string name,unsigned sizeX=1280, unsigned sizeY=251);

	/**
	 * Create PGM plot of an audio channel
	 * @param channel	the channel to plot
	 * @param name		target file name
	 * @param sizeX		width
	 * @param sizeY		height
	 */
	static void createPGMPlot(const Channel &channel, std::string name,unsigned sizeX=1280, unsigned sizeY=251);

	/**
	 * Create PPM plot of audio channels
	 * @param channels	the channels to plot
	 * @param name		target file name
	 * @param sizeX		width
	 * @param sizeY		height per channel
	 */
	static void createPPMPlot(const Channels &channels, std::string name,unsigned sizeX=1280, unsigned sizeY=251);

	/**
	 * Create PPM plot of an audio channel
	 * @param channel	the channel to plot
	 * @param name		target file name
	 * @param sizeX		width
	 * @param sizeY		height
	 */
	static void createPPMPlot(const Channel &channel, std::string name,unsigned sizeX=1280, unsigned sizeY=251);

};

#endif /* PLOT_H_ */
