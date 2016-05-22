

# Completion script for OSPAC (https://github.com/sritterbusch/ospac)

_ospac(){
  local opts
  opts=(
  --parallel
  --fade
  --overlap
  --verbosity
  --help
  --mix
  --raw
  --voice
  --quality
  --mp3
  --plot
  --ogg
  --output
  --spatial
  --set-stereo-spatial
  --stereo
  --set-stereo-level
  --mono
  --multi
  --title
  --artist
  --category
  --image
  --year
  --episode
  --album
  --comment
  --no-factor
  --factor
  --low-pass
  --no-normalize
  --no-eqvoice
  --level-mode
  --highpass
  --normalize
  --leveler
  --eqvoice
  --band-pass
  --no-leveler
  --analyze
  --target
  --left
  --to-mono
  --right
  --ascii
  --xfilter
  --no-xgate
  --no-xfilter
  --xgate
  --noise
  --skip-level
  --trim
  --soft
  --no-skip
  --skip-order
  --skip-target
  )
  local cur=${COMP_WORDS[COMP_CWORD]}
  COMPREPLY=( $(compgen -W "${opts[*]}" -- $cur) )
}

complete -F _ospac ospac

