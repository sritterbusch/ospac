

# Completion script for OSPAC (https://github.com/sritterbusch/ospac)

_ospac(){
  local -a opts
  opts=(
    "*: :_files"
    '*--quality[Quality from 0-low, 1-standard, 2-high, 3-insane]: :(0 1 2 3)'
  '*--mp3[Write final output to the file using external lame]: :_files'
  '*--output[Write final output to the file in netbpm format]: :_files'
  '*--plot[Write final output to the file in wave format]: :_files'
  '*--ogg[Write final output to the file using external oggenc]: :_files'
  '*--highpass[<f> <t> Highpass above f Hertz, sharpness t Hertz]: :'
  '*--leveler[Enable selective leveler]'
  '*--no-factor[Disable channel multiplier]'
  '*--no-normalize[Disable final normalization]'
  '*--eqvoice[Attenuate voice frequency bands]'
  '*--band-pass[<l> <h> <t> Bandpass from l to h Hertz, sharpness t Hertz]: :'
  '*--target[Set average target L2 energy for leveler (3000)]: :'
  "*--level-mode[Shall channels be joined for leveling]: :('single' 'stereo' 'multi')"
  '*--normalize[Normalize final mix]'
  '*--no-leveler[Disable selective leveler]'
  '*--low-pass[<f> <t> Lowpass below f Hertz, sharpness t Hertz]: :'
  '*--analyze[Analyze frequency band components]'
  '*--factor[Multiply channels by the given factor with sigmoid limiter (1.25)]: :'
  '*--no-eqvoice[Do not attenuate frequency bands]'
  '*--verbosity[Set the verbosity level]: :(0 1 2 3 4 5 6)'
  '*--help[Display the help text]'
  '*--no-xfilter[Disable crosstalk filter]'
  '*--no-xgate[Disable crosstalk gate]'
  '*--xfilter[Enable experimental crosstalk filter]'
  '*--xgate[Enable robust crosstalk gate]'
  '*--parallel[Render previous and next segment in parallel]'
  '*--fade[Fading transition over the given number of seconds]: :'
  '*--overlap[Overlapping transition over the given number of seconds]: :'
  '*--right[Load right channel of wave file (if stereo)]: :_files'
  '*--ascii[<s> <file> Load ascii wave file with sample rate s]: :'
  '*--left[Load left channel of wave file (if stereo)]: :_files'
  '*--to-mono[Load mono-mixdown of wave file (if stereo)]: :_files'
  '*--album[Set the album tag if this exists in the output]: :'
  '*--category[Set the category tag if this exists in the output]: :'
  '*--title[Set the title tag if this exists in the output]: :'
  '*--comment[Set the comment tag if this exists in the output]: :'
  '*--artist[Set the artist tag if this exists in the output]: :'
  '*--year[Set the year tag if this exists in the output]: :'
  '*--episode[Set the episode tag if this exists in the output]: :'
  '*--image[Set the image tag if this exists in the output]: :_files'
  '*--trim[Trim audio from start and end]'
  '*--skip-level[Fraction of maximum level considered silence (0.01)]: :'
  '*--no-skip[Do not skip any content]'
  '*--soft[Soft skip silent passages over 0.5s length]'
  '*--noise[Skip all but silence]'
  '*--skip-target[Target length fraction for iteration]: :'
  '*--skip-order[Order of reduction (0-1, default: 0.75)]: :'
  '*--mix[Start pre-mixed channel segment]'
  '*--voice[Start of voice channel segment (default)]'
  '*--raw[Start of raw channel segment]'
  '*--set-stereo-level[Set maximum channel volume factor (0.9)]: :'
  '*--spatial[Create 3d stereo with interaural delays]'
  '*--mono[Create mono output]'
  '*--multi[Create multi channel output]'
  '*--set-stereo-spatial[Set maximum interaural delay distance (0.03)]: :'
  '*--stereo[Create intensity stereo (default)]'
  )
  _arguments $opts
}

compdef _ospac ospac=ospac

