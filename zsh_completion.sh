

# Completion script for OSPAC (https://github.com/sritterbusch/ospac)

_ospac(){
  local -a opts
  opts=(
    "*: :_files"
    '*--soft[Soft skip silent passages over 0.5s length]'
  '*--no-skip[Do not skip any content]'
  '*--skip-level[Fraction of maximum level considered silence (0.01)]: :'
  '*--noise[Skip all but silence]'
  '*--skip-target[Target length fraction for iteration]: :'
  '*--skip-order[Order of reduction (0-1, default: 0.75)]: :'
  '*--trim[Trim audio from start and end]'
  '*--fade[Fading transition over the given number of seconds]: :'
  '*--parallel[Render previous and next segment in parallel]'
  '*--overlap[Overlapping transition over the given number of seconds]: :'
  '*--quality[Quality from 0-low, 1-standard, 2-high, 3-insane]: :(0 1 2 3)'
  '*--output[Write final output to the file in netbpm format]: :'
  '*--plot[Write final output to the file in wave format]: :'
  '*--mp3[Write final output to the file using external lame]: :'
  '*--ogg[Write final output to the file using external oggenc]: :'
  '*--ascii[<s> <file> Load ascii wave file with sample rate s]: :'
  '*--left[Load left channel of wave file (if stereo)]: :'
  '*--right[Load right channel of wave file (if stereo)]: :'
  '*--to-mono[Load mono-mixdown of wave file (if stereo)]: :'
  '*--episode[Set the episode tag if this exists in the output]: :'
  '*--album[Set the album tag if this exists in the output]: :'
  '*--title[Set the title tag if this exists in the output]: :'
  '*--year[Set the year tag if this exists in the output]: :'
  '*--category[Set the category tag if this exists in the output]: :'
  '*--comment[Set the comment tag if this exists in the output]: :'
  '*--artist[Set the artist tag if this exists in the output]: :'
  '*--image[Set the image tag if this exists in the output]: :'
  '*--raw[Start of raw channel segment]'
  '*--mix[Start pre-mixed channel segment]'
  '*--voice[Start of voice channel segment (default)]'
  '*--set-stereo-spatial[Set maximum interaural delay distance (0.03)]: :'
  '*--spatial[Create 3d stereo with interaural delays]'
  '*--set-stereo-level[Set maximum channel volume factor (0.9)]: :'
  '*--multi[Create multi channel output]'
  '*--stereo[Create intensity stereo (default)]'
  '*--mono[Create mono output]'
  '*--verbosity[Set the verbosity level]: :(0 1 2 3 4 5 6)'
  '*--help[Display the help text]'
  '*--xgate[Enable robust crosstalk gate]'
  '*--no-xgate[Disable crosstalk gate]'
  '*--xfilter[Enable experimental crosstalk filter]'
  '*--no-xfilter[Disable crosstalk filter]'
  '*--target[Set average target L2 energy for leveler (3000)]: :'
  '*--no-leveler[Disable selective leveler]'
  '*--no-factor[Disable channel multiplier]'
  '*--eqvoice[Attenuate voice frequency bands]'
  "*--level-mode[Shall channels be joined for leveling]: :('single' 'stereo' 'multi')"
  '*--low-pass[<f> <t> Lowpass below f Hertz, sharpness t Hertz]: :'
  '*--normalize[Normalize final mix]'
  '*--analyze[Analyze frequency band components]'
  '*--no-eqvoice[Do not attenuate frequency bands]'
  '*--factor[Multiply channels by the given factor with sigmoid limiter (1.25)]: :'
  '*--no-normalize[Disable final normalization]'
  '*--highpass[<f> <t> Highpass above f Hertz, sharpness t Hertz]: :'
  '*--band-pass[<l> <h> <t> Bandpass from l to h Hertz, sharpness t Hertz]: :'
  '*--leveler[Enable selective leveler]'
  )
  _arguments $opts
}

compdef _ospac ospac=ospac

