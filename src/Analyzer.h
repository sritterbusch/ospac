/**
 * @file		Analyzer.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		15.3.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief 		Frequency band activity analysis
 */

#ifndef ANALYZER_H_
#define ANALYZER_H_

#include "Channel.h"

/**
 * @brief Frequency band activity analysis
 */
class Analyzer
{
private:
	static float sqr(const float & a) { return a*a; }
public:
	/**
	 * Analysis of frequency band distribution if activity is detected
	 * @param c				audio channel to work on
	 * @param frequencies	n cut-off frequencies
	 * @return resulting l2 normalized n+1 l2 energy levels
	 */
	static std::vector<double> bandedAnalysis(const Channel & c,
											 std::vector<float> frequencies);

	/**
	 * Analysis of frequency band distribution if activity is detected
	 * for fixed cut-off frequencies 100Hz, 500Hz, 2.5k, 4k
	 * @param c				audio channel to work on
	 * @param frequencies	n cut-off frequencies
	 * @return resulting l2 normalized n+1 l2 energy levels
	 */
	static std::vector<double> bandedAnalysis(const Channel & c);

};

#endif /* ANALYZER_H_ */
