/**
 * @file		GuiMain.cpp
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		5.4.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		FLTK GUI for ospac
 */

#ifdef GUI

#include <string>
#include <vector>
#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>

#include "OspacMain.h"
#include "Log.h"

#define MAXPARTS	10
#define MAXFILES    9

int StdChoiceHeight=25;
int StdSep=5;

std::vector<std::string> call(0);

// Main Parts

Fl_Window 			* window;
Fl_Tabs   			* tabs;
Fl_Multiline_Input 	* commandLine;
Fl_Button			* renderButton;

// General settings

Fl_Choice 			* targetMode;
Fl_Input  			* stereoLevel;
Fl_Input  			* spatialDistance;

// Part-Tabs

Fl_Group 			* partTab[MAXPARTS];
std::string           partName[MAXPARTS];

Fl_Choice			* partType[MAXPARTS];
Fl_Choice			* partTransition[MAXPARTS];
Fl_Input			* partOverlap[MAXPARTS];

Fl_Choice           * fileFilter[MAXPARTS][MAXFILES];
Fl_Input			* fileName[MAXPARTS][MAXFILES];
Fl_Button			* fileSelect[MAXPARTS][MAXFILES];
Fl_Button			* fileRemove[MAXPARTS][MAXFILES];

Fl_Check_Button	    * levelerCheck[MAXPARTS];
Fl_Choice           * levelerChoice[MAXPARTS];
Fl_Check_Button		* xgateCheck[MAXPARTS];
Fl_Check_Button     * xfilterCheck[MAXPARTS];
Fl_Check_Button     * normalizeCheck[MAXPARTS];
Fl_Check_Button     * trimCheck[MAXPARTS];
Fl_Check_Button     * skipCheck[MAXPARTS];
Fl_Check_Button     * noiseCheck[MAXPARTS];


// Target-Tab

Fl_Group			* targetTab;

#define TARGETS 4
const char *targetNames[TARGETS]={"Wave: ","mp3: ","ogg: ","plot: "};
const char *targetFilter[TARGETS]={"*.wav","*.mp3","*.ogg","*.ppm"};
const char *targetCommand[TARGETS]={"--output","--mp3","--ogg","--plot"};

Fl_Input			* targetName[TARGETS];
Fl_Button 			* targetSelect[TARGETS];
Fl_Button			* targetRemove[TARGETS];

#define TAGS 6

const char *tagNames[TAGS]={"Title:","Speaker:","Podcast:",
                            "Comment:", "Year:", "Episode #:"};
const char *tagCommand[TAGS]={"--title","--artist","--album",
							  "--comment","--year","--episode"};
Fl_Input		    * metaTags[TAGS];
Fl_Input			* categoryTag;
Fl_Input			* imageTag;
Fl_Button			* imageButton;


void add(std::string & line,std::vector<std::string> & call,std::string str)
{
	call.push_back(str);
	if(str.find(" ")!=std::string::npos)
		str="\""+str+"\"";
	line+=" "+str;
}


void updateCommandLine(void)
{
	std::string line="ospac";

	call=std::vector<std::string>(0);

	switch(targetMode->value())
	{
		case 0:
			add(line,call,"--mono");
			break;
		case 1:
			// Default add(line,call,"--stereo");
			if(std::string(stereoLevel->value())!="0.9")
			{
				add(line,call,"--set-stereo-level");
				add(line,call,stereoLevel->value());
			}
			break;
		case 2:
			add(line,call,"--spatial");
			if(std::string(stereoLevel->value())!="0.9")
			{
				add(line,call,"--set-stereo-level");
				add(line,call,stereoLevel->value());
			}
			if(std::string(spatialDistance->value())!="0.03")
			{
				add(line,call,"--set-stereo-spatial");
				add(line,call,spatialDistance->value());
			}
			break;
		case 3:
			add(line,call,"--multi");
			break;
		default:
			break;

	}

	for(int i=0;i<MAXPARTS;i++)
	{
		switch(partType[i]->value())
		{
		case 0:
			if(i!=0)
			add(line,call,"--voice");
			break;
		case 1:
			add(line,call,"--mix");
			break;
		case 2:
			add(line,call,"--raw");
			break;
		}

		if(levelerCheck[i]->value())
		{
			if(partType[i]->value()!=0)
				add(line,call,"--leveler");

			if(partType[i]->value()==2
			|| (partType[i]->value()==1 && levelerChoice[i]->value()!=1)
			|| (partType[i]->value()==0 && levelerChoice[i]->value()!=0))
			{
				add(line,call,"--level-mode");
				switch(levelerChoice[i]->value())
				{
				case 0:
					add(line,call,"single");
					break;
				case 1:
					add(line,call,"stereo");
					break;
				case 2:
					add(line,call,"multi");
					break;
				}
			}
		} else
		{
			if(partType[i]->value()==0)
				add(line,call,"--no-leveler");
		}

		if(normalizeCheck[i]->value())
		{
			if(partType[i]->value()!=0)
				add(line,call,"--normalize");
		} else
		{
			if(partType[i]->value()==0)
				add(line,call,"--no-normalize");
		}

		if(xgateCheck[i]->value())
		{
			add(line,call,"--xgate");
		} else
		{
			add(line,call,"--no-xgate");
		}

		if(xfilterCheck[i]->value())
		{
			add(line,call,"--xfilter");
		}

		if(trimCheck[i]->value())
		{
			add(line,call,"--trim");
		}

		if(skipCheck[i]->value())
		{
			add(line,call,"--skip");
		}

		if(noiseCheck[i]->value())
		{
			add(line,call,"--noise");
		}

		for(int j=0;j<MAXFILES;j++)
		{
			if(std::string(fileName[i][j]->value())!="")
			{
				switch(fileFilter[i][j]->value())
				{
				case 0:
				default:
					break;
				case 1:
					add(line,call,"--left");
					break;
				case 2:
					add(line,call,"--right");
					break;
				case 3:
					add(line,call,"--to-mono");
					break;
				}
				add(line,call,fileName[i][j]->value());
			}
		}

		switch(partTransition[i]->value())
		{
		case 0:
		case 1:
		default:
			break;
		case 2:
			add(line,call,"--fade");
			add(line,call,partOverlap[i]->value());
			break;
		case 3:
			add(line,call,"--overlap");
			add(line,call,partOverlap[i]->value());
			break;
		case 4:
			add(line,call,"--parallel");
			break;


		}

		if(partTransition[i]->value()==0)
			break;
	}

	bool metaNeeded=false;

	if(std::string(targetName[1]->value())!=""
	|| std::string(targetName[2]->value())!="") // mp3 or ogg
		metaNeeded=true;

	if(metaNeeded)
	{
		for(int j=0;j<TAGS;j++)
		{
			if(std::string(metaTags[j]->value())!="")
			{
				add(line,call,tagCommand[j]);
				add(line,call,metaTags[j]->value());
			}
		}
		if(std::string(categoryTag->value())!="")
		{
			add(line,call,"--category");
			add(line,call,categoryTag->value());
		}
		if(std::string(imageTag->value())!="")
		{
			add(line,call,"--image");
			add(line,call,imageTag->value());
		}
	}

	for(int j=0;j<TARGETS;j++)
	{
		if(std::string(targetName[j]->value())!="")
		{
			add(line,call,targetCommand[j]);
			add(line,call,targetName[j]->value());
		}
	}

	commandLine->value(line.c_str());
}

void commandLine_callback(Fl_Widget* widget, void*)
{
	updateCommandLine();
}


void generalSettings_callback(Fl_Widget* widget, void*)
{
	switch(targetMode->value())
	{
	case 0:
	case 3:
	default:
		stereoLevel->hide();
		spatialDistance->hide();
		break;
	case 1:
		stereoLevel->show();
		spatialDistance->hide();
		break;
	case 2:
		stereoLevel->show();
		spatialDistance->show();
		break;
	}
	updateCommandLine();
}

void generalSettings(int x,int y,int w,int h)
{
	int nLines=3;
	int nCols=2;

	int nW=w-(nCols+1)*StdSep;
	int nH=h-(nLines+1)*StdSep;

	int labelWidth=150;
	if(labelWidth>nW/2)
		labelWidth=nW/2;
	int choiceWidth=nW-labelWidth;

	int cy=StdSep;

	cy+=nH/nLines+StdSep;

	targetMode=new Fl_Choice(2*StdSep+labelWidth,
						 	cy/*+nH/nLines+StdSep*/,
							choiceWidth,
							StdChoiceHeight
							,"Target output mode:");

	targetMode->add("Mono");
	targetMode->add("Stereo");
	targetMode->add("Spatial");
	targetMode->add("Multi");
	targetMode->value(1);
	targetMode->callback(generalSettings_callback);
	targetMode->tooltip("Set the rendering mode for the final file. 'Mono' creates a one-channel output, 'Stereo' intensity based stereo output, 'Spatial' an intensity based stereo output with inter-aural time differences, and 'Multi' takes multi-channel input to multi-channel output without any stereo rendering");

	//cy+=nH/nLines+StdSep;

	stereoLevel=new Fl_Input(2*StdSep+labelWidth+nW,
			              	 cy,
							 choiceWidth,
							 StdChoiceHeight,
							 "Stereo level factor:");

	stereoLevel->value("0.9");
	stereoLevel->callback(commandLine_callback);
	stereoLevel->tooltip("Set factor and divisor of first and last channel in a segment on one output channel- a value of 0.9 yields a maximum of about 20% difference between left and right intensity for one voice");

	cy+=nH/nLines+StdSep;

	spatialDistance=new Fl_Input(2*StdSep+labelWidth+nW,
				              	 cy,
								 choiceWidth,
								 StdChoiceHeight,
								 "Spatial distance (m):");

	spatialDistance->value("0.03");
	spatialDistance->callback(commandLine_callback);
	spatialDistance->tooltip("Maximum distance in path length from a source to the two ears- since the ears are separated by about 0.22m, this path difference should be fairly below that value");

	spatialDistance->hide(); // Standard for value 1


}

void tabs_callback(Fl_Widget* widget, void*)
{
	int i,j;

	for(i=0;i<MAXPARTS;i++)
		for(j=0;j<MAXFILES;j++)
		{
			if(widget==fileSelect[i][j])
			{
				//Fl_Native_File_Chooser fileChooser(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
				Fl_Native_File_Chooser fileChooser(Fl_Native_File_Chooser::BROWSE_MULTI_FILE);
				fileChooser.filter("*.wav");
				fileChooser.show();
				//if(fileChooser.count()==0)
				//	fileName[i][j]->value("");
				if(fileChooser.count()==1)
				{
					fileName[i][j]->value(fileChooser.filename());
					fileName[i][j]->position(std::string(fileChooser.filename()).length());
				}
				if(fileChooser.count()>1)
				{
					for(int k=0;j<MAXFILES && k<fileChooser.count();j++,k++)
					{
						fileName[i][j]->value(fileChooser.filename(k));
						fileName[i][j]->position(std::string(fileChooser.filename(k)).length());
					}
				}
			}
			if(widget==fileRemove[i][j])
			{
				fileName[i][j]->value("");
			}
		}

	for(j=0;j<TARGETS;j++)
	{
		if(widget==targetSelect[j])
		{
			Fl_Native_File_Chooser fileChooser(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
			fileChooser.filter(targetFilter[j]);
			fileChooser.show();
			if(fileChooser.count()==1)
			{
				targetName[j]->value(fileChooser.filename());
				targetName[j]->position(std::string(fileChooser.filename()).length());
			}
		}
		if(widget==targetRemove[j])
		{
			targetName[j]->value("");
		}
	}

	if(widget==imageButton)
	{
		Fl_Native_File_Chooser fileChooser;
		fileChooser.filter("*.jpg");
		fileChooser.show();
		if(fileChooser.count()==1)
		{
			imageTag->value(fileChooser.filename());
			imageTag->position(std::string(fileChooser.filename()).length());
		}
	}


	for(i=0;i<MAXPARTS;i++)
	{
		switch(partTransition[i]->value())
		{
		case 0:
		case 1:
		case 4:
		default:
			partOverlap[i]->hide();
			break;
		case 2:
		case 3:
			partOverlap[i]->show();
			break;
		}

		if(std::string(fileName[i][0]->value())=="")
			fileRemove[i][0]->hide();
		else
			fileRemove[i][0]->show();

		for(j=1;j<MAXFILES;j++)
		{
			if(std::string(fileName[i][j-1]->value())==""
			&& std::string(fileName[i][j]->value())=="")
			{
				fileName[i][j]->hide();
				fileSelect[i][j]->hide();
				fileRemove[i][j]->hide();
				fileFilter[i][j]->hide();
			} else
			{
				fileName[i][j]->show();
				fileSelect[i][j]->show();
				fileFilter[i][j]->show();
				if(std::string(fileName[i][j]->value())=="")
					fileRemove[i][j]->hide();
				else
					fileRemove[i][j]->show();
			}
		}

		//		tabs->remove(partTab[i]);

		tabs->insert(*partTab[i],i);

		if(partTransition[i]->value()==0)
		{
			i++;
			break;
		}
	}

	for(j=0;j<TARGETS;j++)
	{
		if(std::string(targetName[j]->value())=="")
			targetRemove[j]->hide();
		else
			targetRemove[j]->show();
	}

	for(;i<MAXPARTS;i++)
		tabs->remove(partTab[i]);

	updateCommandLine();

	tabs->redraw();
	window->redraw();
}

void createTabs(int x,int y,int w,int h)
{
	tabs=new Fl_Tabs(x,y,w,h);

	int ty=y+StdChoiceHeight+StdSep;
	int th=h-StdChoiceHeight-StdSep;
	int tw=w-StdSep*2;

	for(int i=0;i<MAXPARTS;i++)
	{
		partName[i]=SSTR("Segment "<<i+1);
		partTab[i]=new Fl_Group(x,ty,w,th,partName[i].c_str());
		partTab[i]->tooltip("A segment contains several audio files that are played at the same time and have the same filters applied");

		partType[i]=new Fl_Choice(tw/6+StdSep,
							 	  ty+StdSep,
								  tw/6,
								  StdChoiceHeight,
								  "Segment type:");

		partType[i]->add("Voice");
		partType[i]->add("Mix");
		partType[i]->add("Raw");
		partType[i]->value(0);
		partType[i]->callback(commandLine_callback);
		partType[i]->tooltip("The channels in a 'voice' segment are distributed in the stereo panorama, whereas a 'mix' segment typically contains stereo-mixed content- such as music intro or outros. Use raw for multi-channel input and output.");

		partTransition[i]=new Fl_Choice(3*tw/6+StdSep,
									 	ty+StdSep,
										tw/6,
										StdChoiceHeight,
										"Transition:");

		partTransition[i]->add("End");
		partTransition[i]->add("Direct");
		partTransition[i]->add("Fade");
		partTransition[i]->add("Overlap");
		partTransition[i]->add("Parallel");
		partTransition[i]->value(0);
		partTransition[i]->callback(tabs_callback);
		partTransition[i]->tooltip("You have this and the next segment 'overlap' or 'fade'd into each other. The 'direct' transition is without overlap. If this and the next segment are to be parallel, you can apply different filters on them. Use 'end' if this is the last segment.");

		partOverlap[i]=new Fl_Input(5*tw/6+StdSep,
									ty+StdSep,
									tw/6,
									StdChoiceHeight,
									"Time (s):");
		partOverlap[i]->value("2");
		partOverlap[i]->hide();
		partOverlap[i]->callback(commandLine_callback);
		partOverlap[i]->tooltip("This is the time this and the next segment overlap each other.");

		int by=ty+StdSep*2+StdChoiceHeight+StdChoiceHeight;
		int bh=th-StdSep*3-StdChoiceHeight-StdChoiceHeight;
		int bw=tw*2/3-StdSep;
		int bx=StdSep;

		Fl_Group *soundGroup=new Fl_Group(bx,by,bw,bh, "Sound files");
		soundGroup->box(FL_ENGRAVED_BOX);

		int fx=bx+StdSep;
		int fy=by+StdSep;
		int fw=bw-StdSep*2;
		int fh=bh-StdSep*2;
		int flh=fh/MAXFILES;


		for(int j=0;j<MAXFILES;j++)
		{
			fileFilter[i][j]=new Fl_Choice(fx,fy+flh*j,fw/6-StdSep,StdChoiceHeight,"");
			fileFilter[i][j]->add(" ");
			fileFilter[i][j]->add("Left");
			fileFilter[i][j]->add("Right");
			fileFilter[i][j]->add("Mono");
			fileFilter[i][j]->value(0);
			fileFilter[i][j]->callback(tabs_callback);
			fileFilter[i][j]->tooltip("Downmix stereo input files to left, right or mono channel");
			fileName[i][j]=new Fl_Input(fx+fw/6,fy+flh*j,(3*fw)/6-StdSep,StdChoiceHeight,"");
			fileName[i][j]->readonly(1);
			fileName[i][j]->tooltip("Input file in wave format");
			fileSelect[i][j]=new Fl_Button(fx+(fw*4)/6,fy+flh*j,fw/6-StdSep,StdChoiceHeight,"Select");
			fileSelect[i][j]->tooltip("Select input file, or files");
			fileSelect[i][j]->callback(tabs_callback);
			fileRemove[i][j]=new Fl_Button(fx+(fw*5)/6,fy+flh*j,fw/6,StdChoiceHeight,"Remove");
			fileRemove[i][j]->callback(tabs_callback);
			fileRemove[i][j]->tooltip("Remove this entry");
			fileRemove[i][j]->hide();
			if(j>0)
			{
				fileName[i][j]->hide();
				fileSelect[i][j]->hide();
				fileFilter[i][j]->hide();
			}

		}

		soundGroup->end();

		bx+=bw+StdSep*2;
		bw=(bw+StdSep)/2-StdSep;

		Fl_Group * filterGroup=new Fl_Group(bx,by,bw,bh,"Filter");
		filterGroup->box(FL_ENGRAVED_BOX);

		int cy=fy;
		int cx=bx+StdSep;
		int cw=(bw-StdSep*3)/2;
		int cx2=cx+cw+StdSep;

		levelerCheck[i]=new Fl_Check_Button(cx,cy,cw,StdChoiceHeight,"Leveler");
		levelerCheck[i]->callback(commandLine_callback);
		levelerCheck[i]->value(1);
		levelerCheck[i]->tooltip("The adaptive leveler will try to bring all active passages to the same loudness, whereas silent passages are muted");
		levelerChoice[i]=new Fl_Choice(cx2,cy,cw,StdChoiceHeight,"");
		levelerChoice[i]->add("Single");
		levelerChoice[i]->add("Stereo");
		levelerChoice[i]->add("Multi");
		levelerChoice[i]->callback(commandLine_callback);
		levelerChoice[i]->value(0);
		levelerChoice[i]->tooltip("In 'Single' mode each channel is treated separately, in 'Stereo' mode the loudness of two consecutive channels is leveled, in 'Multi' mode the loudness of all channels together is leveled");
		cy+=flh;
		normalizeCheck[i]=new Fl_Check_Button(cx,cy,cw,StdChoiceHeight,"Normalize");
		normalizeCheck[i]->callback(commandLine_callback);
		normalizeCheck[i]->value(1);
		normalizeCheck[i]->tooltip("Normalize current segment to maximum level");
		cy+=flh;
		xgateCheck[i]=new Fl_Check_Button(cx,cy,cw,StdChoiceHeight,"Crossgate");
		xgateCheck[i]->callback(commandLine_callback);
		xgateCheck[i]->tooltip("The robust crossgate mutes inactive channels when one channel is active to prevent crosstalk");
		cy+=flh;
		xfilterCheck[i]=new Fl_Check_Button(cx,cy,cw,StdChoiceHeight,"Crossfilter");
		xfilterCheck[i]->callback(commandLine_callback);
		xfilterCheck[i]->tooltip("The experimental crossfilter tries to mute later crosstalk copies of one channel in an other");
		cy+=flh;
		trimCheck[i]=new Fl_Check_Button(cx,cy,cw,StdChoiceHeight,"Trim silence");
		trimCheck[i]->callback(commandLine_callback);
		trimCheck[i]->tooltip("Trim silence from the start and the end of this segment");
		cy+=flh;
		skipCheck[i]=new Fl_Check_Button(cx,cy,cw,StdChoiceHeight,"Skip silence");
		skipCheck[i]->callback(commandLine_callback);
		skipCheck[i]->tooltip("Skip longer silence sections in this segment");
		cy+=flh;
		noiseCheck[i]=new Fl_Check_Button(cx,cy,cw,StdChoiceHeight,"Skip signal");
		noiseCheck[i]->callback(commandLine_callback);
		noiseCheck[i]->tooltip("Skip all signal from this segment to analyse background noise");
		cy+=flh;

		filterGroup->end();

		partTab[i]->end();

		if(i>0)
			tabs->remove(partTab[i]);
	}

	targetTab=new Fl_Group(x,ty,w,th,"Targets");
	targetTab->tooltip("Define the output files and meta information that should be added to them if possible");

	int bx=x+StdSep;
	int by=ty+StdChoiceHeight;
	int bw=w-StdSep*2;
	int bh=th/2-StdSep-StdChoiceHeight;

	Fl_Group *filesGroup=new Fl_Group(bx,by,bw,bh, "Target output files");
	filesGroup->box(FL_ENGRAVED_BOX);

	int fx=bx+StdSep;
	int fy=by+StdSep;
	int fw=bw-StdSep*2;
	int fh=bh-StdSep*2;
	int flh=fh/4;

	for(int j=0;j<TARGETS;j++)
	{
		targetName[j]=new Fl_Input(fx+fw/8,fy+flh*j,(5*fw)/8-StdSep,StdChoiceHeight,targetNames[j]);
		targetName[j]->readonly(1);
		targetSelect[j]=new Fl_Button(fx+(fw*6)/8,fy+flh*j,fw/8-StdSep,StdChoiceHeight,"Select");
		targetSelect[j]->callback(tabs_callback);
		targetRemove[j]=new Fl_Button(fx+(fw*7)/8,fy+flh*j,fw/8,StdChoiceHeight,"Remove");
		targetRemove[j]->callback(tabs_callback);
		//targetRemove[j]->hide();
	}

	filesGroup->end();

	by+=th/2;

	Fl_Group *metaGroup=new Fl_Group(bx,by,bw,bh, "Meta data tags");
	metaGroup->box(FL_ENGRAVED_BOX);

	fx=bx+StdSep;
	fy=by+StdSep;
	fw=bw-StdSep*2;
	fh=bh-StdSep*2;
	flh=fh/4;
	int fcw=fw/2;

	for(int j=0;j<TAGS;j++)
	{
		int c=j&1;
		int r=j/2;
		int x=fx+c*fcw;
		int y=fy+r*flh;
		metaTags[j]=new Fl_Input(x+fcw/4,y,(3*fcw)/4-StdSep,StdChoiceHeight,tagNames[j]);
	}

	metaTags[3]->value("Created with ospac.net");
	int c=0;
	int r=3;
	x=fx+c*fcw;
	y=fy+r*flh;

	categoryTag=new Fl_Input(x+fcw/4,y,(3*fcw)/4-StdSep,StdChoiceHeight,"Category:");
	categoryTag->value("Speech");

	c=1;
	x=fx+c*fcw;
	y=fy+r*flh;

	imageTag=new Fl_Input(x+fcw/4,y,(2*fcw)/4-StdSep*2,StdChoiceHeight,"Image:");
	imageButton=new Fl_Button(x+(3*fcw)/4,y,fcw/4,StdChoiceHeight,"Select");
	imageButton->callback(tabs_callback);

	metaGroup->end();

	targetTab->end();

	tabs->end();

	tabs->add_resizable(*targetTab);
}

class Logbuffer : public std::stringbuf
{
public:
    virtual int sync()
    {
        commandLine->value(this->str().c_str());
        commandLine->position(this->str().length());
        Fl::check();
        return 0;
    }
};

void run_callback(Fl_Widget* widget, void*)
{
	OspacMain podchain(call);

	Logbuffer logbuffer;

	std::ostream logStream(&logbuffer);

	updateCommandLine();

	logStream << "$ " << commandLine->value() << std::endl << std::endl;

	Log::setOutput(logStream);

	Log::setLoglevel(logDEBUG);
	//Log::setShowFunction(true);
	Log::setShowRuntime(true);

	renderButton->deactivate();

	int x=commandLine->x(),y=commandLine->y(),w=commandLine->w(),h=commandLine->h();
	commandLine->resize(0,0,window->w(),window->h());
	commandLine->redraw();
	window->redraw();


	try
	{
		podchain.run();
	}
	catch(std::bad_alloc& ba)
	{
		//LOG(logERROR) << "Out of memory: " << ba.what() << std::endl;
		//LOG(logINFO)  << "Try to increase your physical or virtual memory (swap)" << std::endl;
		return;
	}

	commandLine->resize(x,y,w,h);
	commandLine->redraw();
	window->redraw();

	renderButton->activate();
}

int main(int argc, char **argv)
{
	std::string title=SSTR("Ospac - Open source podcast audio chain - " << VERSION << " built " << __DATE__ << " " << __TIME__ );
	window = new Fl_Window(640,480,title.c_str());

	generalSettings(0,0,window->w()/2,window->h()/6);

	createTabs(0, window->h()/6, window->w(), window->h()*5/6-window->h()/8);

	window->add_resizable(*tabs);

	commandLine=new Fl_Multiline_Input(0,window->h()*7/8,window->w()*7/8,window->h()/8,"");
	commandLine->wrap(1);
	commandLine->readonly(1);
	commandLine->textsize(9);

	renderButton=new Fl_Button(window->w()*7/8,window->h()*7/8,window->w()/8,window->h()/8,"Run!");
	renderButton->callback(run_callback);

	updateCommandLine();

	window->end();

	window->position((Fl::w() - window->w())/2, (Fl::h() - window->h())/2);

	window->show(argc, argv);
	return Fl::run();
}

#endif
