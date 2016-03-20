## Ospac Rendering Modes: Mono, Stereo, Spatial, Multi

It can be highly beneficial to a podcast to render conversation in stereo,
to help the listener to differentiate speakers even with similar voices. 
But there are quite some issues that need to be taken into consideration:
For example, listening experience, one-channel and mono reproduction,
compression and audio quality.

## Mono mode

In mono mode, ospac renders all signals into a one channel signal.

    ospac --mono {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav \
          --output mono-test.wav
    ospac --raw mono-test.wav --plot mono-test.ppm

![Mono rendering](https://github.com/sritterbusch/ospac/raw/master/images/mono-test.png)

[Listen to the mono rendering](https://github.com/sritterbusch/ospac/raw/master/examples/mono-test.mp3)

Please be aware, that ospac does the mono mix-down without scaling, thus you
must use --factor or --normalize when including stereo signals using --mix
or --raw.

## Stereo mode

The standard stereo mode renders the audio signals with different intensity
settings on the two stereo channels. In the default setting (0.9 or 1.1), the
left-most channel has a intensity boost of 10% on the left and 10% damping
on the right channel. The right-most channel is treated accordingly and all
other channels are evenly distributed in between.

    ospac {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav \
          --output stereo-test.wav
    ospac --raw stereo-test.wav --plot stereo-test.ppm

![Standard stereo rendering](https://github.com/sritterbusch/ospac/raw/master/images/stereo-test.png)

[Listen to the standard stereo rendering](https://github.com/sritterbusch/ospac/raw/master/examples/stereo-test.mp3)

You can increase the stereo effect by changing the --set-stereo-level setting:

    ospac --set-stereo-level 0.25 \
          {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav \
          --output stereo-strong.wav
    ospac --raw stereo-strong.wav --plot stereo-strong.ppm

![Strong stereo rendering](https://github.com/sritterbusch/ospac/raw/master/images/stereo-strong.png)

[Listen to the strong stereo rendering](https://github.com/sritterbusch/ospac/raw/master/examples/stereo-strong.mp3)

You may also disable the stereo effect by setting the stereo level to 1 and
generate a two-channel mono signal:

    ospac --set-stereo-level 1 \
          {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav \
          --output stereo-mono-test.wav
    ospac --raw stereo-mono-test.wav --plot stereo-mono-test.ppm

![Mono stereo rendering](https://github.com/sritterbusch/ospac/raw/master/images/stereo-mono-test.png)

[Listen to the mono stereo rendering](https://github.com/sritterbusch/ospac/raw/master/examples/stereo-mono-test.mp3)

This mode can be used to have a full stereo intro while the actual
conversation should be in mono.

## Spatial mode

Inter-aural differences in intensity are only one hint for spatial
localization of speakers. Another hint are inter-aural time delays that
occur since sound travels with about 330m/s in air and the two ears are
separated by about 0.22m. The standard setting for the spatial mode are
0.09m sound distance in addition the the standard stereo intensity setting
of 0.9:

    ospac --spatial {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav \
          --output spatial-test.wav
    ospac --raw spatial-test.wav --plot spatial-test.ppm

![Standard spatial rendering](https://github.com/sritterbusch/ospac/raw/master/images/spatial-test.png)

[Listen to the standard spatial rendering](https://github.com/sritterbusch/ospac/raw/master/examples/spatial-test.mp3)

You can also increase the interaural time delay to the maximum delay
representing 0.2m for sounds coming directly from the sides:

    ospac --spatial --set-stereo-spatial 0.2 \
          {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav \
          --output spatial-strong.wav
    ospac --raw spatial-strong.wav --plot spatial-strong.ppm

![Standard spatial rendering](https://github.com/sritterbusch/ospac/raw/master/images/spatial-strong.png)

[Listen to the standard spatial rendering](https://github.com/sritterbusch/ospac/raw/master/examples/spatial-strong.mp3)

## Multi mode

Ospac also supports a multi-channel mode where no rendering takes place and
results in as many channels as they were provided. This mode is useful for
just using the crosstalk gate or filter and then rely on a another audio
tool for the actual stereo or mono rendering.

    ospac --multi --set-stereo-spatial 0.2 \
          {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav \
          --output multi-test.wav
    ospac --raw multi-test.wav --plot multi-test.ppm

![Standard spatial rendering](https://github.com/sritterbusch/ospac/raw/master/images/multi-test.png)

## Discussion

While the stereo rendering can be very beneficial for the listener, there
are also some down sides that need to be taken into consideration: First of
all, strong stereo settings can be very annoying. If someone talks to you
from the right, you would simply turn towards this person- the listener
cannot do this, and is right to be annoyed. Secondly, you cannot be sure
that there are always two speakers available. Many handhelds only have one
speaker and either play a mono mono mixdown or just one channel. Also, there
might be listeners that can only listen with one ear. Thirdly, the use of
stereo rendering decreases the compressibility of the signal, which will
lead to a decrease of quality with constant compression rate. This gets even
worse if a mono-mixdown occurs of a compressed audio file.

Especially the --spatial mode has problems with mono-mixdowns as
you can experience in this
[mono mixdown of the strong spatial rendering](https://github.com/sritterbusch/ospac/raw/master/examples/spatial-strong-mono.mp3).
This is the same effect you experience when listening to the radio which
switches from mono reception to stereo reception. Suddenly you hear more
high frequencies. They were there before, but were canceled in the mono
mixdown.

The following table shows the discussion of the modes:

Rendering      | Mono | Stereo mono | Stereo | Strong stereo | Spatial | Strong spatial 
-------------- | ---- | ----------- | ------ | ------------- | ------- | --------------
Stereo effect  |  -   |      -      |   +    |      ++       |    ++   |    +++
Not annoying   |  +   |      +      |   +    |      --       |    +    |    --        
One channel    |  ++  |      ++     |   +    |      --       |    +    |    +
Mono rendering |  ++  |      ++     |   ++   |      ++       |    -    |    --
Compression    |  43  |      47     |   54   |      60       |    59   |    63

The compression are kBit/s with fixed quality setting in lame for above
examples.
