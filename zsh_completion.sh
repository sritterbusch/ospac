

# Completion script for OSPAC (https://github.com/sritterbusch/ospac)

_ospac(){
  local -a opts
  opts=(
    "*: :_files"
    '*--mix[Start pre-mixed channel segment]'
  '*--raw[Start of raw channel segment]'
  '*--voice[Start of voice channel segment (default)]'
  '*--set-stereo-spatial[Set maximum interaural delay distance (0.03)]: :'
  '*--stereo[Create intensity stereo (default)]'
  '*--spatial[Create 3d stereo with interaural delays]'
  '*--mono[Create mono output]'
  '*--multi[Create multi channel output]'
  '*--set-stereo-level[Set maximum channel volume factor (0.9)]: :'
  '*--output[Write final output to the file in netbpm format]: :'
  '*--mp3[Write final output to the file using external lame]: :'
  '*--quality[Quality from 0-low, 1-standard, 2-high, 3-insane]: :(0 1 2 3)'
  '*--ogg[Write final output to the file using external oggenc]: :'
  '*--plot[Write final output to the file in wave format]: :'
  '*--ascii[<s> <file> Load ascii wave file with sample rate s]: :'
  '*--right[Load right channel of wave file (if stereo)]: :'
  '*--to-mono[Load mono-mixdown of wave file (if stereo)]: :'
  '*--left[Load left channel of wave file (if stereo)]: :'
  "*--level-mode[Shall channels be joined for leveling]: :('single' 'stereo' 'multi')"
  '*--target[Set average target L2 energy for leveler (3000)]: :'
  '*--normalize[Normalize final mix]'
  '*--eqvoice[Attenuate voice frequency bands]'
  '*--factor[Multiply channels by the given factor with sigmoid limiter (1.25)]: :'
  '*--no-normalize[Disable final normalization]'
  '*--no-eqvoice[Do not attenuate frequency bands]'
  '*--highpass[<f> <t> Highpass above f Hertz, sharpness t Hertz]: :'
  '*--leveler[Enable selective leveler]'
  '*--band-pass[<l> <h> <t> Bandpass from l to h Hertz, sharpness t Hertz]: :'
  '*--no-leveler[Disable selective leveler]'
  '*--no-factor[Disable channel multiplier]'
  '*--analyze[Analyze frequency band components]'
  '*--low-pass[<f> <t> Lowpass below f Hertz, sharpness t Hertz]: :'
  '*--no-xgate[Disable crosstalk gate]'
  '*--xfilter[Enable experimental crosstalk filter]'
  '*--no-xfilter[Disable crosstalk filter]'
  '*--xgate[Enable robust crosstalk gate]'
  '*--verbosity[Set the verbosity level]: :(0 1 2 3 4 5 6)'
  '*--help[Display the help text]'
  '*--image[Set the image tag if this exists in the output]: :'
  '*--episode[Set the episode tag if this exists in the output]: :'
  '*--artist[Set the artist tag if this exists in the output]: :'
  '*--comment[Set the comment tag if this exists in the output]: :'
  '*--album[Set the album tag if this exists in the output]: :'
  '*--category[Set the category tag if this exists in the output]: :'
  '*--title[Set the title tag if this exists in the output]: :'
  '*--year[Set the year tag if this exists in the output]: :'
  '*--parallel[Render previous and next segment in parallel]'
  '*--fade[Fading transition over the given number of seconds]: :'
  '*--overlap[Overlapping transition over the given number of seconds]: :'
  '*--skip-target[Target length fraction for iteration]: :'
  '*--soft[Soft skip silent passages over 0.5s length]'
  '*--skip-order[Order of reduction (0-1, default: 0.75)]: :'
  '*--skip-level[Fraction of maximum level considered silence (0.01)]: :'
  '*--trim[Trim audio from start and end]'
  '*--no-skip[Do not skip any content]'
  '*--noise[Skip all but silence]'
  )
  _arguments $opts
}

compdef _ospac ospac=ospac

