/**
 * @file		OspacMain.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		5.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Main function and command line interface
 */

#include <iostream>
#include <string>

#include "OspacMain.h"
#include "Wave.h"
#include "CrosstalkFilter.h"
#include "Log.h"
#include "SelectiveLeveler.h"
#include "StereoMix.h"
#include "MonoMix.h"
#include "Maximizer.h"
#include "CrosstalkGate.h"
#include "Merge.h"
#include "Skip.h"
#include "Equalizer.h"
#include "Plot.h"
#include "Frequency.h"
#include "Analyzer.h"
#include "Encode.h"
#include <stdlib.h>


/**
 * @mainpage Ospac project documentation
 *
 * @tableofcontents
 *
 * @section Introduction Introduction
 *
 * Ospac will take a multi-channel recording of a conversation and master this
 * to a high-quality mix-down with support for intro and outro. It was
 * developed due to the need of a batch solution for audio podcast creation.
 * It is a rewrite and compilation of the scripts and methods used for the
 * Modellansatz podcast (http://modellansatz.de/ or http://modellansatz.de/en).
 *
 * @section Concept Concept
 *
 * A main issue is to get things done without too much hassle. Therefore, the
 * external dependencies are minimal: It depends on libsndfile only. Also,
 * the channels are simply std::vectors of floats, therefore all is done in
 * memory and unnecessary copy operations will take place. Furthermore,
 * there is not yet an object oriented concept of generators, analyzers,
 * filters, and consumers, and thus many filters are just static methods
 * getting their thing done.
 *
 * @subsection Fundamentals Fundamental classes
 *
 * Single channels are represented by the Channel class. It consists of
 * std::vector<float> and the corresponding bitrate. Multiple channels such
 * as in stereo are represented by Channels, a std::vector<Channel>.
 *
 * Loading and saving of Channels is done by the Wave class. It uses
 * libsndfile to load wave files with an arbitrary number of channels and
 * likewise saving of such channels.
 *
 * All logging takes place using the Log class, that takes care of also
 * showing the source code line number, run time and logging levels.
 *
 * The actual command line interface is implemented in the OspacMain class.
 * All defaults and actual workflows for audio processing are defined in there.
 *
 * The conversion of physical quantities is done in the Physics class. So far,
 * the only conversion is the distance of sound to time and vice versa with
 * respect of the speed of sound at normal room conditions.
 *
 * The Plot class delivers plain visualizations of Channels objects in netpbm
 * format. All plots in this document were create by this functionality.
 *
 * @subsection Filter Single channel filter classes
 *
 * In the following some of the active filter classes are illustrated. The
 * examples given are in the test directory of the repository.
 *
 * @subsubsection Leveling Leveling
 *
 * The leveling is done in the SelectiveLeveler class: First of all, the
 * filter identifies levels for silence, transition and signal. Then it tries
 * to normalize the average l2 energy in a window of detected signals to a
 * given energy level, while muting detected silence. The target energy level
 * of transition sections is linearily dependent on their respective level.
 * The actual muting or amplification is smoothened in a tolerance window to
 * prevent continuous leveling.
 *
 * This is an exemplary leveling result (top: original, bottom: result):
 * @image html leveler-result.png
 * @image latex leveler-result.png "Result of selective leveler" width=10cm
 *
 * @subsubsection Amplification Normalization and amplification
 *
 * The Maximizer class deals with amplification and normalization of audio
 * signals. Since amplification can easily lead to distortion it is always
 * combined with a sigmoid limiter. (Of course this introduces distortion as
 * well, but without wrap-arounds would occur leading to far worse clicking.)
 *
 * Examples of amplification by factor 2 and 4 (top: original, below 6dB or 12dB
 * attenuation):
 * @image html factor-result.png
 * @image latex factor-result.png "Result of amplification by 2 or 4" width=10cm
 *
 * The normalization simply scales the signal to the full 16bit value range.
 *
 * Example of normalization (top: original, bottom: result):
 * @image html normalize-result.png
 * @image latex normalize-result.png "Result of normalization" width=10cm
 *
 * @subsubsection Skipping Soft silence skipping
 *
 * The Skip class offers a way to reduce long silent passages without influencing
 * the natural way of speaking. First of all, this is done by only considering
 * passages that have silence for longer than a certain time (0.5s) and then
 * reducing only relatively to the time the silence extends longer. Therefore,
 * longer pauses will remain longer pauses compared to others, but they will
 * be shorter altogether.
 *
 * Example of soft skipping (top: original, bottom: result):
 * @image html skip-result.png
 * @image latex skip-result.png "Result of soft skipping" width=10cm
 *
 * @subsection Multifilter Crosstalk filter classes
 *
 * Crosstalk occurs when one microphone records signals from other channels
 * as well. The following example shows two channels, where the second
 * channel mainly consists of crosstalk of the first, and a small segment
 * of original input.
 *
 * @image html crosstalk-example.png
 * @image latex crosstalk-example.png "Exemplary two channel input" width=10cm
 *
 * Ospac has two filters that deal with this issue. The traditional and
 * robust crosstalk gate based on the activity on one channel compared to the
 * others, and the experimental crosstalk filter that actively searches for
 * crosstalk occurences.
 *
 * @subsubsection Crossgate Crosstalk gate
 *
 * The CrosstalkGate computes activity levels on each channel and then mutes
 * channels linearily with less activity compared to the current maximum
 * activity. This filter was successfully applied to many podcast episodes.
 *
 * The gate results in this output from above example:
 * @image html xgate-result.png
 * @image latex xgate-result.png "Result of crosstalk gate filter" width=10cm
 *
 * @subsubsection Crossfilter Crosstalk filter
 *
 * The CrosstalkFilter tries to actively identify crosstalk in other channels
 * and mutes signals that mainly consist of crosstalk. The identification is
 * done by scalar product on a comparison windows between the current signal
 * and previous windows of other channels as a negative indicator, as well as
 * the current signal and future crosstalk on other channels as a positive
 * indicator for original input. This filter is a new development and
 * should be considered experimental.
 *
 * The filter results in this output from above example:
 * @image html xfilter-result.png
 * @image latex xfilter-result.png "Result of crosstalk filter" width=10cm
 *
 * @subsection Mixdown Mix down filter
 *
 * The Merge class merges two audio segments by either overlap or fading.
 * In the overlap mode, both parts remain at full volume and should start and
 * end with zero intensity. In the fading mode, the parts are linearly faded
 * in and out in the transition phase. When the transition time is zero, the
 * two parts are just joint after each other.
 *
 * The MonoMix class simply superimposes all channels onto one mono channel.
 * To prevent clipping this should be followed by normalization or
 * amplification with sigmoid mapping.
 *
 * The StereoMix class offers several modes of stereo mapping: In its
 * simplest mode the various channels are mapped onto equidistant positions
 * that are rendered with intensity differences onto the stereo channels.
 *
 * A more sophisticated mode takes interaural delays into account yielding
 * a far more realistic positioning of the speakers. This comes by cost of
 * worse mono reproduction and decreased compressibility of the outcome.
 *
 * Another mode takes occlusion of higher frequencies into account, this
 * improves mono reproduction but currently the frequency bank filters are
 * quite bad and therefore the outcome is improvable.
 *
 * @section Future Plans for the future
 *
 * <strong>Improved ospac user interface</strong>
 *
 * The command line interface does not offer all capabilities the filters
 * can offer, by far. For example, one cannot include pre-mixed channels
 * for music and voice channels in the same part. Also the additional meta
 * information could be included, but would make the calling parameters very
 * long. Therefore, a settings file, based on a markup language, seems like
 * a good idea to improve the user interface.
 *
 * <strong>Object orientation</strong>
 *
 * Design proper object oriented representation of concepts: Split analysis
 * and filter methods in order to enable free combination of various concepts.
 * And decrease the number of unnecessary copy operations.
 *
 * <strong>More detailed merging parameters</strong>
 *
 * At the moment, merging of parts can either be faded or overlapped on both
 * sides. There are some cases, where more modes or asymmetric configurations
 * could be helpful.
 *
 * <strong>Direct invocation of encoders</strong>
 *
 * Due to different command line interface of encoders, it might be a good
 * idea to include the invocation of mp3, ogg, ... encoders into ospac. For
 * this, the user interface should be improved due to the large number of
 * meta tags that should be included in the interface.
 *
 * <strong>Support for HRTF functions</strong>
 *
 * As an additional 3d rendering solution the support for head related transfer
 * functions (HRTF) would be highly interesting.
 *
 * <strong>Synchronization of double enders</strong>
 *
 * The skipping filter shows how channels can be shortened or enlenghted
 * without perceivable change to the voice. At the same time, the crosstalk
 * filter accurately matches signals from different channels on sample
 * accuracy. Therefore everything is there to offer time dynamic
 * synchronization of double enders that include low quality recordings
 * of the other channels.
 *
 * <strong>Multi threading support</strong>
 *
 * Most of the filters can easily benefit from multi threading- either by
 * parallel treatment of channels, or by time splitting in a channel for
 * filters that do not have time-dependent side-effects.
 *
 * <strong>Channel virtualization</strong>
 *
 * To avoid excessive memory usage, the basic channel structure could be
 * extended to support hard drive mapping of results. Only segments that
 * are currently worked on are in memory and all other parts remain on disc.
 */



OspacMain::OspacMain(std::vector<std::string> aArg) : arg(aArg)
{
	mixMode=STEREO;
	argMode=VOICE;
	nextTransitionMode=transitionMode=NONE;
	nextTransitionSeconds=transitionSeconds=0;

	stdMaximizer[VOICE]=1.25;
	stdMaximizer[MIX]  =0.0; // disable
	stdMaximizer[RAW]  =0.0; // disable

	stdNormalizer[VOICE]=true;
	stdNormalizer[MIX]  =false;
	stdNormalizer[RAW]  =false;

	stdLeveler[VOICE]=true;
	stdLeveler[MIX]  =false;
	stdLeveler[RAW]  =false;

	stdLevelTarget[VOICE]=3000;
	stdLevelTarget[MIX]  =3000;
	stdLevelTarget[RAW]  =3000;

	stdXGate[VOICE]=true;
	stdXGate[MIX]  =false;
	stdXGate[RAW]  =false;

	stdXFilter[VOICE]=false;
	stdXFilter[MIX]  =false;
	stdXFilter[RAW]  =false;

	stdSkip[VOICE]=false;
	stdSkip[MIX]  =false;
	stdSkip[RAW]  =false;

	stdSkipSilence[VOICE]=0.01;
	stdSkipSilence[MIX]  =0.01;
	stdSkipSilence[RAW]  =0.01;

	stdVoiceEq[VOICE]=false;
	stdVoiceEq[MIX]=false;
	stdVoiceEq[RAW]=false;

	stereoLevel=0.9;
	stereoSpatial=0.03;

	setStandard();
}

void OspacMain::setStandard()
{
	maximizer=stdMaximizer[argMode];
	normalizer=stdNormalizer[argMode];
	leveler=stdLeveler[argMode];
	levelTarget=stdLevelTarget[argMode];
	xGate=stdXGate[argMode];
	xFilter=stdXFilter[argMode];
	skip=stdSkip[argMode];
	skipSilence=stdSkipSilence[argMode];
	voiceEq=stdVoiceEq[argMode];

	transitionMode=nextTransitionMode;
	transitionSeconds=nextTransitionSeconds;

	nextTransitionMode=NONE;
	nextTransitionSeconds=0;

	bandpassLow=bandpassHigh=bandpassTransition=0;

	skipOrder=0.75;

	loadSkipSeconds=0;

	loadMaxSeconds=1e+99;

	noise=false;

}

OspacMain::~OspacMain()
{
}

std::string OspacMain::options[]={"spatial","stereo","mono","multi",
							  "set-stereo-level","set-stereo-spatial",
							  "voice","mix","raw",
							  "ascii",
							  "fade","overlap","parallel",
							  "factor", "no-factor",
							  "leveler","no-leveler","target",
							  "normalize","no-normalize",
							  "skip","no-skip","skip-level","skip-order",
							  "noise",
							  "xgate","no-xgate",
							  "xfilter","no-xfilter",
							  "eqvoice","no-eqvoice",
							  "bandpass","analyze",
							  "output","mp3","ogg",
							  "help","verbosity","plot"};

int OspacMain::run(void)
{
	Channels operand;
	Channels work;

	for(unsigned i=0;i<arg.size();i++)
	{
		if(isOption(arg[i]))
		{
			LOG(logDEBUG) << "Option: --" << arg[i] << std::endl;
			if(arg[i]=="help")
			{
				std::cout << std::endl;
				std::cout << "ospac - Open source podcast audio chain" << std::endl;
				std::cout << "---------------------------------------" << std::endl;
				std::cout << std::endl;
				std::cout << "Distributed under the MIT License." << std::endl;
				std::cout << std::endl;
				std::cout << " Information:" << std::endl;
				std::cout << "  --help          This information" << std::endl;
				std::cout << "  --verbosity [n] Set verbosity to level [n]" << std::endl;
				std::cout << std::endl;
				std::cout << " Output modes:" << std::endl;
				std::cout << "  --spatial       Create 3d stereo with interaural delays"<< std::endl;
				std::cout << "  --stereo        Create intensity stereo (default)" << std::endl;
				std::cout << "  --multi         Create multi channel output" << std::endl;
				std::cout << "  --mono          Create mono output" << std::endl;
				std::cout << "  --output [file] Write final output to [file] in wave format" << std::endl;
				std::cout << "  --mp3 [file]    Write final output to [file] using external lame" << std::endl;
				std::cout << "  --ogg [file]    Write final output to [file] using external oggenc" << std::endl;
				std::cout << std::endl;
				std::cout << "  --set-stereo-level [n]  Set maximum channel volume factor (0.9)" << std::endl;
				std::cout << "  --set-stereo-spatial [n] Set maximum interaural delay distance (0.03)" << std::endl;
				std::cout << std::endl;
				std::cout << " New segment selection:" << std::endl;
				std::cout << "  --voice         Start of voice channel segment (default)"<<std::endl;
				std::cout << "  --mix           Start pre-mixed channel segment"<<std::endl;
				std::cout << "  --raw           Start of raw channel segment" << std::endl;
				std::cout << std::endl;
				std::cout << " Segment transition:" << std::endl;
				std::cout << "  --fade [s]      Fading transition over [s] seconds" << std::endl;
				std::cout << "  --overlap [s]   Overlapping transition over [s] seconds" << std::endl;
				std::cout << "  --parallel      Render previous and next segment in parallel" << std::endl;
				std::cout << std::endl;
				std::cout << " Crosstalk filter:" << std::endl;
				std::cout << "  --xgate         Enable robust crosstalk gate" << std::endl;
				std::cout << "  --no-xgate      Disable crosstalk gate" << std::endl;
				std::cout << "  --xfilter       Enable experimental crosstalk filter" << std::endl;
				std::cout << "  --no-xfilter    Disable crosstalk filter"<< std::endl;
				std::cout << std::endl;
				std::cout << " Adaptive silence skip:" << std::endl;
				std::cout << "  --skip          Soft skip silent passages over 0.5s length" << std::endl;
				std::cout << "  --skip-level    Fraction of maximum level considered silence (0.01)" << std::endl;
				std::cout << "  --skip-order    Order of reduction (0-1, default: 0.75)" << std::endl;
				std::cout << "  --no-skip       Do not skip any content" << std::endl;
				std::cout << "  --noise         Skip all but silence" << std::endl;
				std::cout << std::endl;
				std::cout << " Leveling, equalizer and normalization:" << std::endl;
				std::cout << "  --leveler       Enable selective leveler" << std::endl;
				std::cout << "  --target [n]    Set average target L2 energy [n] for leveler (3000)" << std::endl;
				std::cout << "  --no-leveler    Disable selective leveler" << std::endl;
				std::cout << "  --factor [n]    Multiply channels by factor [n] with sigmoid limiter (1.25)" << std::endl;
				std::cout << "  --no-factor     Disable channel multiplier" << std::endl;
				std::cout << "  --eqvoice       Attenuate voice frequency bands" << std::endl;
				std::cout << "  --no-eqvoice    Do not attenuate frequency bands" << std::endl;
				std::cout << "  --analyze       Analyze frequency band components" << std::endl;
				std::cout << "  --normalize     Normalize final mix" << std::endl;
				std::cout << "  --no-normalize  Disable final normalization" << std::endl;
				std::cout << "  --bandpass [l] [h] [t] Bandpass from l to h Hertz, sharpness t Hertz" << std::endl;
				std::cout << " Import audio:" << std::endl;
				std::cout << "  [file]          Load wave file" << std::endl;
				std::cout << "  --ascii [s] [file] Load ascii wave file with sample rate s" << std::endl;
				std::cout << std::endl;
				std::cout << "Examples:" << std::endl;
				std::cout << " Mix 2 mono voice recordings with crosstalk filter, leveling and normalization:" << std::endl;
				std::cout << "  ospac person1.wav person2.wav --output target.wav" << std::endl;
				std::cout << std::endl;
				std::cout << " Mix podcast with stereo intro and outro:" << std::endl;
				std::cout << "  ospac --mix in.wav --overlap 4 \\" << std::endl;
				std::cout << "        --voice person1.wav person2.wav --overlap 4 \\" << std::endl;
				std::cout << "        --mix out.wav --output target.wav" << std::endl;
				std::cout << std::endl;
				std::cout << " Again with shortened options:" << std::endl;
				std::cout << "  ospac -mi in.wav -ov 4 -v person1.wav person2.wav -ov 4 -mi -ot target.wav" << std::endl;
				std::cout << std::endl;
				std::cout << " Just run the crosstalk filter and create an un-mixed multi-channel output:" << std::endl;
				std::cout << "  ospac --multi --raw t1.wav t2.wav t3.wav t4.wav --xfilter --output multi.wav" << std::endl;
				std::cout << std::endl;
			} else
			if(arg[i]=="verbosity")
			{
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;
					int level=atoi(arg[i].c_str());
					switch(level)
					{
					case 0:
						Log::setLoglevel(logFATAL);
						break;
					case 1:
						Log::setLoglevel(logERROR);
						break;
					case 2:
						Log::setLoglevel(logWARNING);
						break;
					case 3:
						Log::setLoglevel(logINFO);
						break;
					case 4:
						Log::setLoglevel(logDEBUG);
						break;
					case 5:
						Log::setLoglevel(logDEBUG1);
						break;
					case 6:
						Log::setLoglevel(logDEBUG2);
						break;
					default:
						Log::setLoglevel(logDEBUG3);
						break;
					}
				} else
					Log::setLoglevel(logDEBUG);

				LOG(logINFO) << "Setting loglevel to " << Log::getLoglevel() << std::endl;
			} else
			if(arg[i]=="plot")
			{
				Channels temp;
				render(work,operand,temp);
				if(i+1<arg.size())
				{
					i++;
					Plot::createPPMPlot(temp,arg[i]);
				} else
					Plot::createPPMPlot(temp,"output.pgm");
			} else
			if(arg[i]=="spatial")
			{
				LOG(logDEBUG) << "Setting mixMode to SPATIAL" << std::endl;
				mixMode=SPATIAL;
			} else
			if(arg[i]=="stereo")
			{
				LOG(logDEBUG) << "Setting mixMode to STEREO" << std::endl;
				mixMode=STEREO;
			} else
			if(arg[i]=="mono")
			{
				LOG(logDEBUG) << "Setting mixMode to MONO" << std::endl;
				mixMode=MONO;
			} else
			if(arg[i]=="multi")
			{
				LOG(logDEBUG) << "Setting mixMode to MULTI" << std::endl;
				mixMode=MULTI;
			} else
			if(arg[i]=="voice")
			{
				render(work,operand,operand);
				argMode=VOICE;
				LOG(logDEBUG) << "Next segment type VOICE" << std::endl;
				setStandard();
				work=Channels();
			} else
			if(arg[i]=="mix")
			{
				render(work,operand,operand);
				argMode=MIX;
				LOG(logDEBUG) << "Next segment type MIX" << std::endl;
				setStandard();
				work=Channels();
			} else
			if(arg[i]=="raw")
			{
				render(work,operand,operand);
				argMode=RAW;
				LOG(logDEBUG) << "Next segment type RAW" << std::endl;
				setStandard();
				work=Channels();
			} else
			if(arg[i]=="fade")
			{
				nextTransitionMode=FADE;
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;
					nextTransitionSeconds=atof(arg[i].c_str());

				}
			} else
			if(arg[i]=="overlap")
			{
				nextTransitionMode=OVERLAP;
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;
					nextTransitionSeconds=atof(arg[i].c_str());
				}
				LOG(logDEBUG) << "nextMode: " << nextTransitionMode << " Mode: " << transitionMode << std::endl;
			} else
			if(arg[i]=="parallel")
			{
				nextTransitionMode=PARALLEL;
				LOG(logDEBUG) << "nextMode: " << nextTransitionMode << " Mode: " << transitionMode << std::endl;
				if(noise)
				{
					LOG(logWARNING) << "WARNING: The --noise filter will yield an out-of-sync result to --parallel channels!"<<std::endl;
				}
				if(skip)
				{
					LOG(logWARNING) << "WARNING: The --skip filter will yield an out-of-sync result to --parallel channels!"<<std::endl;
				}
			} else
			if(arg[i]=="factor")
			{
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;
					maximizer=atof(arg[i].c_str());
				}
			} else
			if(arg[i]=="no-factor")
			{
				maximizer=0;
			}else
			if(arg[i]=="set-stereo-level")
			{
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;
					stereoLevel=atof(arg[i].c_str());
				}
			} else
			if(arg[i]=="set-stereo-spatial")
			{
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;
					stereoSpatial=atof(arg[i].c_str());
				}
			} else
			if(arg[i]=="leveler")
			{
				leveler=true;
			} else
			if(arg[i]=="no-leveler")
			{
				leveler=false;
			} else
			if(arg[i]=="target")
			{
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;
					levelTarget=atof(arg[i].c_str());
				}
			} else
			if(arg[i]=="normalize")
			{
				normalizer=true;
			} else
			if(arg[i]=="no-normalize")
			{
				normalizer=false;
			} else
			if(arg[i]=="noise")
			{
				noise=true;
				if(nextTransitionMode==PARALLEL
				|| transitionMode==PARALLEL)
				{
					LOG(logWARNING) << "WARNING: The --noise filter will yield an out-of-sync result to --parallel channels!"<<std::endl;
				}
			} else
			if(arg[i]=="skip")
			{
				skip=true;
				if(nextTransitionMode==PARALLEL
				|| transitionMode==PARALLEL)
				{
					LOG(logWARNING) << "WARNING: The --skip filter will yield an out-of-sync result to --parallel channels!"<<std::endl;
				}
			} else
			if(arg[i]=="no-skip")
			{
				skip=false;
			} else
			if(arg[i]=="skip-level")
			{
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;
					skipSilence=atof(arg[i].c_str());
				}
			} else
			if(arg[i]=="skip-order")
			{
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;
					skipOrder=atof(arg[i].c_str());
				}
			} else
			if(arg[i]=="xgate")
			{
				xGate=true;
			} else
			if(arg[i]=="no-xgate")
			{
				xGate=false;
			} else
			if(arg[i]=="xfilter")
			{
				xFilter=true;
			} else
			if(arg[i]=="no-xfilter")
			{
				xFilter=false;
			} else
			if(arg[i]=="eqvoice")
			{
				voiceEq=true;
			} else
			if(arg[i]=="no-eqvoice")
			{
				voiceEq=false;
			} else
			if(arg[i]=="bandpass")
			{
				if(i+1<arg.size())
				{
					i++;
					bandpassLow=atof(arg[i].c_str());
					bandpassHigh=44100;
					bandpassTransition=1000;
					if(bandpassTransition>bandpassLow/2)
						bandpassTransition=bandpassLow/2;
					if(i+1<arg.size())
					{
						i++;
						bandpassHigh=atof(arg[i].c_str());
						if(i+1<arg.size())
						{
							i++;
							bandpassTransition=atof(arg[i].c_str());
						}

					}

				}
			} else
			if(arg[i]=="output")
			{
				Channels temp;
				render(work,operand,temp);
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;
					LOG(logDEBUG) << "nextMode: " << nextTransitionMode << " Mode: " << transitionMode << std::endl;

					Wave::save(arg[i],temp);
				}
			} else
			if(arg[i]=="mp3")
			{
				Channels temp;
				render(work,operand,temp);
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;

					int result=Encode(temp).mp3(arg[i]);
					if(result)
					{
						LOG(logDEBUG) << "Encoding failed with error code " << result;
					}
				}
			} else
			if(arg[i]=="ogg")
			{
				Channels temp;
				render(work,operand,temp);
				if(i+1<arg.size())
				{
					i++;
					LOG(logDEBUG) << "Value: " << arg[i] << std::endl;

					int result=Encode(temp).ogg(arg[i]);
					if(result)
					{
						LOG(logDEBUG) << "Encoding failed with error code " << result;
					}

				}
			} else
			if(arg[i]=="analyze")
			{
				Channels temp;
				render(work,operand,temp);
				for(unsigned c=0;c<temp.size();c++)
					Analyzer::bandedAnalysis(temp[c]);
			} else
			if(arg[i]=="ascii")
			{
				int samplerate=44100;
				if(i+1<arg.size())
				{
					i++;
					samplerate=atoi(arg[i].c_str());
					if(i+1<arg.size())
					{
						i++;
						unsigned before=work.size();
						Wave::loadAscii(arg[i],samplerate,work,loadSkipSeconds,loadMaxSeconds);
						if(work.size()==before)
						{
							LOG(logERROR) << "Could not load " << arg[i] << std::endl;
							return 2;
						}
					}
				}

			} else
			{
				LOG(logERROR) << "command line option " << arg[i] << " was not recognized." << std::endl;
				return 1;
			}
		} else
		{
			unsigned before=work.size();
			Wave::load(arg[i],work,loadSkipSeconds,loadMaxSeconds);
			if(work.size()==before)
			{
				LOG(logERROR) << "Could not load " << arg[i] << std::endl;
				return 2;
			}
		}
	}

	LOG(logINFO) << "finished" << std::endl;

	return 0;
}

void OspacMain::render(Channels & work,Channels & operand,Channels & target)
{
	if(work.size()==0 || work[0].size()==0)
	{
		target=operand;
		return;
	}


	if(voiceEq)
	{
		work=Equalizer::voiceEnhance(work);
	}
	if(bandpassTransition!=0)
	{
		std::vector<float> freqs(2);
		freqs[0]=bandpassLow;
		freqs[1]=bandpassHigh;
		for(unsigned c=0;c<work.size();c++)
		{
			Channels bands=Frequency::split(work[c],freqs,bandpassTransition,true);
			work[c]=bands[1];
		}
	}
	if(xFilter)
	{
		LOG(logDEBUG) << "CrosstalkFilter" << std::endl;
		//CrosstalkFilter ctFilter(work,82,0.2,1.5,5.0);
		//CrosstalkFilter ctFilter(work,20,1.0,0.5,3.0);
		CrosstalkFilter ctFilter(work,3,0.66,0.5,3.0);
		ctFilter.analyze();
		//ctFilter.save("analysis.wav");
		ctFilter.mute();
	}
	if(xGate)
	{
		LOG(logDEBUG) << "CrosstalkGate" << std::endl;
//		CrosstalkGate::gate(work,100,0.35);
		CrosstalkGate::gate(work,100,1.0);
	}
	if(leveler)
	{
		LOG(logDEBUG) << "Leveler" << std::endl;

		if(argMode==VOICE)
			SelectiveLeveler::level(work,levelTarget,1.0,0.05,0.025,0.2,0.4);
		else if(argMode==MIX && (work.size()%2)==0)
			SelectiveLeveler::levelStereo(work,levelTarget,1.0,0.1,0.05,0.1,0.5);
		else
			SelectiveLeveler::level(work,levelTarget,1.0,0.05,0.025,0.2,0.4);
	}
	if(skip)
	{
		LOG(logDEBUG) << "Skip with parameter "<< skipSilence << std::endl;
		Skip::silence(work,skipSilence,0.5,0.05,skipOrder);
		skip=false;
	}
	if(noise)
	{
		LOG(logDEBUG) << "Skip signal" << std::endl;
		Skip::noise(work,skipSilence);
		noise=false;
	}

	switch(mixMode)
	{
	case SPATIAL:
		if(argMode==VOICE)
		{
			LOG(logDEBUG) << "StereoMix" << std::endl;
			StereoMix mix;
			mix.mix(work,stereoLevel,true,stereoSpatial);
			work=mix.getTarget();
		}
		break;
	case STEREO:
		if(argMode==VOICE)
		{
			LOG(logDEBUG) << "StereoMix" << std::endl;
			StereoMix mix;
			mix.mix(work,stereoLevel);
			work=mix.getTarget();
		}
		break;
	case MONO:
		{
			LOG(logDEBUG) << "MonoMix" << std::endl;
			MonoMix mix;
			mix.mix(work);
			work=mix.getTarget();
		}
		{
			if(operand.size()>1)
			{
				LOG(logINFO) << "Late --mono option detected. "
						     << "Fixing previous mixing. "
							 << "Please use --mono at the very beginning!"
							 << std::endl;
				MonoMix mix;
				mix.mix(operand);
				operand=mix.getTarget();
			}
		}
		break;
	case MULTI:
	default:
		break;
	}
	if(maximizer!=0.0)
	{
		LOG(logDEBUG) << "Maximizer" << std::endl;
		Maximizer::amplify(work,maximizer);
	}
	if(normalizer)
	{
		LOG(logDEBUG) << "Normalize" << std::endl;
		Maximizer::normalize(work);
	}
	switch(transitionMode)
	{
	case NONE:
		LOG(logDEBUG) << "Direct transition" << std::endl;
		target=Merge::overlap(operand,work,0);
		break;
	case OVERLAP:
		LOG(logDEBUG) << "Overlap transition" << std::endl;
		target=Merge::overlap(operand,work,transitionSeconds);
		break;
	case FADE:
		LOG(logDEBUG) << "Fade transition" << std::endl;
		target=Merge::fade(operand,work,transitionSeconds);
		break;
	case PARALLEL:
		LOG(logDEBUG) << "Parallel rendering" << std::endl;
		target=Merge::parallel(operand,work);
		break;
	default:
		break;
	}
}

/**
 * Main program entry point
 * @param argc number of arguments
 * @param argv argument strings
 * @return 0 for success, all others for errors
 */
int main(int argc,char * argv[])
{
	std::vector<std::string> arg;

	LOG(logINFO) << "ospac 1.0 built " << __DATE__ << " " << __TIME__ << std::endl;

	Log::setLoglevel(logDEBUG);
	Log::setShowFunction(true);
	Log::setShowRuntime(true);

	for(int i=1;i<argc;i++)
		arg.push_back(std::string(argv[i]));

	OspacMain podchain(arg);

	try
	{
		return podchain.run();
	}
	catch(std::bad_alloc& ba)
	{
		LOG(logERROR) << "Out of memory: " << ba.what() << std::endl;
		LOG(logINFO)  << "Try to increase your physical or virtual memory (swap)" << std::endl;
		return 3;
	}

}

bool OspacMain::isOption(std::string &o)
{
	std::string result(o);
	if(o.length()<2)
		return false;

	unsigned hits=0;
	unsigned best=0;

	if(o[0]=='-' && o[1]!='-') // short form
	{
		for(unsigned i=0;i<sizeof(options)/sizeof(options[0]);i++)
			if(o.length()-1>0)
			{
				unsigned j;
				for(j=1;j<o.length() && o[j]==options[i][j-1];j++);
				if(j>=best)
				{
					if(j>best)
						hits=0;
					best=j;
					hits++;
					result=options[i];
				}
			}
	} else if(o[0]=='-' && o[1]=='-') // long form
	{
		for(unsigned i=0;i<sizeof(options)/sizeof(options[0]);i++)
			if(o.length()-2>=options[i].length())
			{
				unsigned j;
				for(j=2;j<o.length() && o[j]==options[i][j-2];j++);
				if(j==o.length())
				{
					hits++;
					result=options[i];
				}
			}
	} else
		return false;

	if(hits>0)
	{
		if(hits==1)
			o=result;
		return true;
	}

	return false;
}
