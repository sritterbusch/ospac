

# Completion script for OSPAC (https://github.com/sritterbusch/ospac)

_ospac(){
  local opts
  opts=(
  --mix
  --raw
  --voice
  --overlap
  --fade
  --parallel
  --year
  --comment
  --image
  --album
  --title
  --episode
  --category
  --artist
  --skip-order
  --skip-target
  --skip-level
  --no-skip
  --soft
  --noise
  --trim
  --help
  --verbosity
  --normalize
  --no-eqvoice
  --eqvoice
  --level-mode
  --leveler
  --no-factor
  --analyze
  --low-pass
  --band-pass
  --no-leveler
  --highpass
  --no-normalize
  --factor
  --target
  --set-stereo-level
  --spatial
  --stereo
  --multi
  --mono
  --set-stereo-spatial
  --xfilter
  --no-xfilter
  --xgate
  --no-xgate
  --ascii
  --left
  --to-mono
  --right
  --output
  --mp3
  --quality
  --plot
  --ogg
  )
  local cur=${COMP_WORDS[COMP_CWORD]}
  COMPREPLY=( $(compgen -W "${opts[*]}" -- $cur) )
}

complete -F _ospac ospac

