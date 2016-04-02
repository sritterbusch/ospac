/**
 * @file		OspacMain.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		5.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Command line interface
 */

#ifndef OSPACMAIN_H_
#define OSPACMAIN_H_

#include <string>
#include <vector>

#include "Channel.h"
#include "Encode.h"

/**
 * @brief Main program class for dealing with command line options
 *
 * This class represents the command line interface of ospac,
 * and defines standard values for its settings.
 */
class OspacMain
{
protected:
	/**
	 * Vector of all command line arguments
	 */
	std::vector<std::string> arg;

	/**
	 * List of all valid command line tokens
	 */
	static std::string options[];

	/**
	 * Tests the given parameter if it is an options by checking with options
	 * list
	 * @param s parameter string to check
	 * @return true if an option was detected
	 */
	bool isOption(std::string &s);

	/**
	 * Downmix mode for voice channels.
	 */
	enum MixMode { SPATIAL, STEREO, MONO, MULTI , MaxMixMode};

	/**
	 * Argument mode what kind of acoustic data is to be processed.
	 */
	enum ArgMode { VOICE, MIX, RAW, MaxArgMode};

	/**
	 * Transition mode between acoustic data segments
	 */
	enum TransitionMode { NONE, OVERLAP, FADE, PARALLEL, MaxTransitionMode};

	/**
	 * Downmix mode for current acoustic data segment
	 */
	MixMode			mixMode;

	/**
	 * Kind of acoustic data in this acoustic data segment
	 */
	ArgMode 		argMode;

	/**
	 * Transition mode from last acoustic data segment
	 */
	TransitionMode 	transitionMode;

	/**
	 * Transition time from last acoustic data segment
	 */
	float			transitionSeconds;

	/**
	 * Transition mode to next acoustic data segment
	 */
	TransitionMode 	nextTransitionMode;

	/**
	 * Transition time to next acoustic data segment
	 */
	float			nextTransitionSeconds;

	/**
	 * Current factor for maximizer
	 */
	float   maximizer;

	/**
	 * Should current segment be normalized
	 */
	bool    normalizer;

	/**
	 * Should the current segment be levelled
	 */
	bool    leveler;

	/**
	 * Leveling target energy
	 */
	float   levelTarget;

	/**
	 * Should the current segment be cross-gated
	 */
	bool    xGate;

	/**
	 * Should the current segment be filtered by the experimental cross-filter
	 */
	bool    xFilter;

	/**
	 * Should the current segment apply the all but silence-filter
	 */
	bool    noise;

	/**
	 * Should the current segment apply the skip-filter
	 */
	bool    skip;

	/**
	 * Silence detection for skipping filter
	 */
	float   skipSilence;

	/**
	 * Skip order (1 for all, 0.5 for sqrt(time) skip)
	 */
	float   skipOrder;

	/**
	 * Should voice equalizer run over the channels?
	 */
	bool	voiceEq;

	/**
	 * Bandpass low limit in Hertz
	 */
	float	bandpassLow;

	/**
	 * Bandpass high limit in Hertz
	 */
	float	bandpassHigh;

	/**
	 * Bandpass filter transition in Hertz
	 */
	float	bandpassTransition;

	/**
	 * Current level factor for stereo or spatial amplituds
	 */
	float	stereoLevel;

	/**
	 * Current maximum interaural detail for spatial stereo
	 */
	float	stereoSpatial;

	/**
	 * Current setting for seconds to skip at loading
	 */
	float   loadSkipSeconds;

	/**
	 * Current setting for maximal seconds to load
	 */
	float   loadMaxSeconds;

	/**
	 * Encoding meta tag title
	 */
	std::string title;

	/**
	 * Encoding meta tag artist
	 */
	std::string artist;

	/**
	 * Encoding meta tag album
	 */
	std::string album;

	/**
	 * Encoding meta tag comment
	 */
	std::string comment;

	/**
	 * Encoding meta tag category
	 */
	std::string category;

	/**
	 * Encoding meta tag episode
	 */
	std::string episode;

	/**
	 * Encoding meta tag year
	 */
	std::string year;

	/**
	 * Encoding meta tag image
	 */
	std::string image;

	/**
	 * Encoding quality setting
	 */
	Encode::QualitySetting     quality;

	/**
	 * Set all variables to their standard setting dependent on data mode
	 */
	void setStandard();

	/**
	 * Render last segment and current segment to the target segment
	 * according to all settings regarding current segment and transition
	 * @param work		previous audio data segment
	 * @param operand	current audio data segment
	 * @param target	target audio data segment
	 */
	void render(Channels & work,Channels & operand,Channels & target);

	/**
	 * Standard maximizer factor
	 */
	float	stdMaximizer[MaxArgMode];

	/**
	 * Standard normalizer flag
	 */
	bool	stdNormalizer[MaxArgMode];

	/**
	 * Standard leveler flag
	 */
	bool	stdLeveler[MaxArgMode];

	/**
	 * Standard leveler target
	 */
	float	stdLevelTarget[MaxArgMode];

	/**
	 * Standard cross gate flag
	 */
	bool	stdXGate[MaxArgMode];

	/**
	 * Standard cross filter flat
	 */
	bool	stdXFilter[MaxArgMode];

	/**
	 * Standard skip filter flag
	 */
	bool	stdSkip[MaxArgMode];

	/**
	 * Standard silence level for skip filter
	 */
	float   stdSkipSilence[MaxArgMode];

	/**
	 * Standard voice eq setting
	 */
	bool    stdVoiceEq[MaxArgMode];

public:
	/**
	 * Set up data and prepare everything for the run method.
	 * @param aArg vector of command line options.
	 */
	OspacMain(std::vector<std::string>);

	virtual ~OspacMain();

	/**
	 * Run the application and do all actions that were requested
	 * my the option given.
	 * @return 0 in case of success, 1 in case of error.
	 */
	int run(void);

private:
	/**
	 * target cache
	 */
	Channels target;
};

#endif /* OSPACMAIN_H_ */
