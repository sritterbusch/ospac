

# Completion script for OSPAC (https://github.com/sritterbusch/ospac)

_ospac(){
  local opts
  opts=(
  --image
  --artist
  --category
  --episode
  --year
  --album
  --comment
  --title
  --mix
  --voice
  --raw
  --verbosity
  --help
  --xgate
  --no-xfilter
  --no-xgate
  --xfilter
  --mono
  --spatial
  --set-stereo-spatial
  --stereo
  --multi
  --set-stereo-level
  --output
  --ogg
  --plot
  --quality
  --mp3
  --low-pass
  --level-mode
  --factor
  --no-leveler
  --no-factor
  --analyze
  --no-eqvoice
  --target
  --highpass
  --leveler
  --normalize
  --band-pass
  --eqvoice
  --no-normalize
  --left
  --to-mono
  --right
  --ascii
  --overlap
  --fade
  --parallel
  --skip-target
  --skip-level
  --no-skip
  --noise
  --soft
  --trim
  --skip-order
  )
  local cur=${COMP_WORDS[COMP_CWORD]}
  COMPREPLY=( $(compgen -W "${opts[*]}" -- $cur) )
}

complete -F _ospac ospac

