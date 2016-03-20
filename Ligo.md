## Bandpass and LIGO

Ospac has a very accurate band pass filter that can be used to reduce rumble
or high frequency hissing. It is so accurate that it can be used to remove
noise from the gravitational wave detection experiment 
[LIGO](https://en.wikipedia.org/wiki/LIGO).

## Fetch data and generate raw sound

The [LIGO experiment detected a special event](https://losc.ligo.org/events/GW150914/)
of which the raw data can be downloaded:

    wget https://losc.ligo.org/s/events/GW150914/H-H1_LOSC_4_V1-1126259446-32.txt.gz
    wget https://losc.ligo.org/s/events/GW150914/L-L1_LOSC_4_V1-1126259446-32.txt.gz
    gzip -d H-H1_LOSC_4_V1-1126259446-32.txt.gz
    gzip -d L-L1_LOSC_4_V1-1126259446-32.txt.gz

This data represents the signals from two "laser microphones" located in
Hanford and Livingston, USA, which are separated from each other by about
3000km. The microphone have shown to be able to detect planes flying over
them as well as seismic waves. But apart from that, they should also be able
to detect gravitational waves as they were predicted by Einstein. Using
ospac you can listen to the recording with 4096Hz sample frequence (they
also provide files with 16384Hz sample frequency):

    ospac --raw --ascii 4096 H-H1_LOSC_4_V1-1126259446-32.txt \
                --ascii 4096 L-L1_LOSC_4_V1-1126259446-32.txt \
                --output ligo-raw.wav
    ospac --raw ligo-raw.wav --plot ligo-raw.ppm

![Raw ligo data](https://github.com/sritterbusch/ospac/raw/master/images/ligo-raw.png)

[Listen to the raw ligo data](https://github.com/sritterbusch/ospac/raw/master/examples/ligo-raw.mp3)

## Band pass filter

This already sounds fascinating, but the actual signal is covered by noise.
Using Audacity we ca have a look at the spectrum:

![Raw ligo data spectrum](https://github.com/sritterbusch/ospac/raw/master/images/ligo-analysis.png)

Within the interval from 80Hz to 300Hz there seems to be a valley of less
noise, as [this was hinted by Ligo](https://losc.ligo.org/s/events/GW150914/GW150914_tutorial.html)). 
Using the --bandfilter of ospac with a very thin transition interval of just
10Hz, we can have a closer look at this less noisy segment: 

    ospac --raw --ascii 4096 H-H1_LOSC_4_V1-1126259446-32.txt \
                --ascii 4096 L-L1_LOSC_4_V1-1126259446-32.txt \
                --bandpass 80 300 10 --normalize \
                --output ligo-filter.wav
    ospac --raw ligo-filter.wav --plot ligo-filter.ppm

![Raw data band filtered 80-300Hz](https://github.com/sritterbusch/ospac/raw/master/images/ligo-filter.png)

[Listen to the filtered ligo data](https://github.com/sritterbusch/ospac/raw/master/examples/ligo-filter.mp3)

## Isolate the final result

What's was at 16s? We can isolate this event using the skip filter:

    ospac --raw --ascii 4096 H-H1_LOSC_4_V1-1126259446-32.txt \
                --ascii 4096 L-L1_LOSC_4_V1-1126259446-32.txt \
                --bandpass 80 300 10 \
                --skip --skip-level 0.5 --skip-order 0.95 \
                --normalize \
                --output ligo-event.wav
    ospac --raw ligo-event.wav --plot ligo-event.ppm

![Isolated event in the raw data band filtered 80-300Hz](https://github.com/sritterbusch/ospac/raw/master/images/ligo-event.png)

[Listen to the isolated event in the filtered ligo data](https://github.com/sritterbusch/ospac/raw/master/examples/ligo-event.mp3)

These are two massive black holes well beyond our sight that fell into each 
other.

Apart from this, you can very well use the --bandpass filter to reduce low
frequency rumbling or high frequency hissing.  For this, you can most
probably using wider transition intervals for faster processing.
