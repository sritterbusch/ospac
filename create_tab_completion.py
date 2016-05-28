#!/usr/bin/python3

# By Johannes Bechberger, under MIT License

import sys
import os

description = "description"   # hacks that allow better editing in IDEs
zsh = "zsh"
flag = "flag"
multiple = "multiple"
section = "section"

default_completion_item = {
    multiple: True,     # allow multiple applications
    description: "",    # description of this parameter
    flag: True,         # is this argument a flag (without any arguments)?
    zsh: ""             # completion string for zsh or list of possible arguments
}

completion = {
    "Information": {
        "--help": {
            description: "Display the help text",
        },
        "--verbosity": {
            description: "Set the verbosity level",
            flag: False,
            zsh: list(range(7)),
        }
    },
    "Output modes": {
        "--spatial": {
            description: "Create 3d stereo with interaural delays"
        },
        "--stereo": {
            description: "Create intensity stereo (default)"
        },
        "--multi": {
            description: "Create multi channel output"
        },
        "--mono": {
            description: "Create mono output"
        },
        "--set-stereo-level": {
            description: "Set maximum channel volume factor (0.9)",
            flag: False
        },
        "--set-stereo-spatial": {
            description: "Set maximum interaural delay distance (0.03)",
            flag: False
        }
    },
    "Output targets": {
        "--output": {
            description: "Write final output to the file in netbpm format",
            zsh: "_files"
        },
        "--plot": {
            description: "Write final output to the file in wave format",
            zsh: "_files"
        },
        "--mp3": {
            description: "Write final output to the file using external lame",
            zsh: "_files"
        },
        "--ogg": {
            description: "Write final output to the file using external oggenc",
            zsh: "_files"
        },
        "--quality": {
            description: "Quality from 0-low, 1-standard, 2-high, 3-insane",
            zsh: [0, 1, 2, 3]
        }
    },
    "Output meta data": {
        "--title": {
            description: "Set the title tag if this exists in the output",
            flag: False
        },
        "--artist": {
            description: "Set the artist tag if this exists in the output",
            flag: False
        },
        "--album": {
            description: "Set the album tag if this exists in the output",
            flag: False
        },
        "--comment": {
            description: "Set the comment tag if this exists in the output",
            flag: False
        },
        "--category": {
            description: "Set the category tag if this exists in the output",
            flag: False
        },
        "--episode": {
            description: "Set the episode tag if this exists in the output",
            flag: False
        },
        "--year": {
            description: "Set the year tag if this exists in the output",
            flag: False
        },
        "--image": {
            description: "Set the image tag if this exists in the output",
            flag: False,
            zsh: "_files"
        }
    },
    "New segment selection": {
        "--voice": {
            description: "Start of voice channel segment (default)"
        },
        "--mix": {
            description: "Start pre-mixed channel segment"
        },
        "--raw": {
            description: "Start of raw channel segment"
        }
    },
    "Segment transition": {
        "--fade": {
            description: "Fading transition over the given number of seconds",
            flag: False
        },
        "--overlap": {
            description: "Overlapping transition over the given number of seconds",
            flag: False
        },
        "--parallel": {
            description: "Render previous and next segment in parallel"
        }
    },
    "Crosstalk filter": {
        "--xgate": {
            description: "Enable robust crosstalk gate"
        },
        "--no-xgate": {
            description: "Disable crosstalk gate"
        },
        "--xfilter": {
            description: "Enable experimental crosstalk filter"
        },
        "--no-xfilter": {
            description: "Disable crosstalk filter"
        }
    },
    "Adaptive silence skip": {
        "--soft": {
            description: "Soft skip silent passages over 0.5s length"
        },
        "--skip-target": {
            description: "Target length fraction for iteration",
            flag: False
        },
        "--skip-level": {
            description: "Fraction of maximum level considered silence (0.01)",
            flag: False
        },
        "--skip-order": {
            description: "Order of reduction (0-1, default: 0.75)",
            flag: False
        },
        "--no-skip": {
            description: "Do not skip any content"
        },
        "--trim": {
            description: "Trim audio from start and end"
        },
        "--noise": {
            description: "Skip all but silence"
        }
    },
    "Leveling, equalizer and normalization": {
        "--leveler": {
            description: "Enable selective leveler"
        },
        "--target": {
            description: "Set average target L2 energy for leveler (3000)",
            flag: False
        },
        "--level-mode": {
            description: "Shall channels be joined for leveling",
            zsh: ["single", "stereo", "multi"]
        },
        "--no-leveler": {
            description: "Disable selective leveler"
        },
        "--factor": {
            description: "Multiply channels by the given factor with sigmoid limiter (1.25)",
            flag: False
        },
        "--no-factor": {
            description: "Disable channel multiplier"
        },
        "--eqvoice": {
            description: "Attenuate voice frequency bands"
        },
        "--no-eqvoice": {
            description: "Do not attenuate frequency bands"
        },
        "--analyze": {
            description: "Analyze frequency band components"
        },
        "--normalize": {
            description: "Normalize final mix"
        },
        "--no-normalize": {
            description: "Disable final normalization"
        },
        "--band-pass": {
            description: "[l] [h] [t] Bandpass from l to h Hertz, sharpness t Hertz",
            flag: False
        },
        "--low-pass": {
            description: "[f] [t] Lowpass below f Hertz, sharpness t Hertz",
            flag: False
        },
        "--highpass": {
            description: "[f] [t] Highpass above f Hertz, sharpness t Hertz",
            flag: False
        }
    },
    "Import audio": {
        "--left": {
            description: "Load left channel of wave file (if stereo)",
            zsh: "_files"
        },
        "--right": {
            description: "Load right channel of wave file (if stereo)",
            zsh: "_files"
        },
        "--to-mono": {
            description: "Load mono-mixdown of wave file (if stereo)",
            zsh: "_files"
        },
        "--ascii": {
            description: "[s] [file] Load ascii wave file with sample rate s",
            flag: False
        }
    }
}

def zsh_completion() -> str:
    """
    Generate the tab completion for zsh
    """
    def process_item(section: str, name: str, item: dict) -> str:
        tmp = default_completion_item.copy()
        tmp.update(item)
        item = tmp
        if item[multiple]:
            name = "*" + name
        item[description] = item[description].replace("[", "<").replace("]", ">")
        ret = name + "[" + item[description] + "]"
        if not item[flag] or item[zsh]:
            ret += ": :"
        if item[zsh]:
            if isinstance(item[zsh], list):
                ret += "(" + " ".join(repr(x) for x in item[zsh]) + ")"
            elif isinstance(item[zsh], str):
                ret += item[zsh]
            else:
                print("Error on zsh completion in " + repr(item), file=sys.stderr)
                os.error(1)
        return repr(ret)
    return """

# Completion script for OSPAC (https://github.com/sritterbusch/ospac)

_ospac(){{
  local -a opts
  opts=(
    "*: :_files"
    {}
  )
  _arguments $opts
}}

compdef _ospac ospac=ospac

""".format("\n  ".join(process_item(s, n, completion[s][n]) for s in completion for n in completion[s]))

def bash_completion() -> str:
    """
    Generate the tab completion for bash
    """
    def process_item(section: str, name: str, item: dict) -> str:
        return name
    return """

# Completion script for OSPAC (https://github.com/sritterbusch/ospac)

_ospac(){{
  local opts
  opts=(
  {}
  )
  local cur=${{COMP_WORDS[COMP_CWORD]}}
  COMPREPLY=( $(compgen -W "${{opts[*]}}" -- $cur) )
}}

complete -F _ospac ospac

""".format("\n  ".join(process_item(s, n, completion[s][n]) for s in completion for n in completion[s]))


if __name__ == "__main__":
    USAGE = """
    Shell completion generator for ospac.
    It generates a tab completion file for bash or zsh.

    Usage:

        {} [zsh|bash] OUTPUT_FILE
    """.format(sys.argv[0])

    if len(sys.argv) != 3 or sys.argv[1] not in ["zsh", "bash"]:
        print(USAGE)
        exit()
    compl = ""
    if sys.argv[1] == "zsh":
        compl = zsh_completion()
    elif sys.argv[1] == "bash":
        compl = bash_completion()
    with open(sys.argv[2], "w") as f:
        f.write(compl)
