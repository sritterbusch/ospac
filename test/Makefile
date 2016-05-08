all:	volume-changes-leveled.wav intro-normalized-skip.wav intro-normalized-noise.wav \
	../images/skip-target.png \
	intro-normalized-space.wav \
	intro-factor-2.wav intro-factor-4.wav intro-xfilter.wav intro-xgate.wav \
	stereo-test.wav spatial-test.wav spatial-strong.wav stereo-strong.wav stereo-mono-test.wav mono-test.wav multi-test.wav \
	ligo-raw.wav ligo-filter.wav ligo-event.wav \
	dynamic-noise.wav dynamic-noise-normalized.wav \
	intro-normalized-skip-05.wav intro-normalized-skip-06.wav intro-normalized-skip-07.wav \
	intro-normalized-skip-08.wav intro-normalized-skip-09.wav

clean:
	rm -f 	intro-normalized.wav volume-changes-leveled.wav intro-normalized-skip.wav intro-normalized-noise.wav \
		intro-factor-2.wav intro-factor-4.wav intro-xfilter.wav intro-xgate.wav \
		stereo-test.wav spatial-test.wav spatial-strong.wav stereo-strong.wav \
		stereo-test-mono.wav spatial-test-mono.wav spatial-strong-mono.wav stereo-strong-mono.wav \
		stereo-mono-test.wav mono-test.wav multi-test.wav \
		H-H1_LOSC_4_V1-1126259446-32.txt L-L1_LOSC_4_V1-1126259446-32.txt \
		ligo-raw.wav ligo-filter.wav ligo-event.wav *~ *ppm \
		intro-normalized-space.wav \
		dynamic-noise.wav dynamic-noise-normalized.wav \
		intro-normalized-skip-05.wav intro-normalized-skip-06.wav intro-normalized-skip-07.wav \
		intro-normalized-skip-08.wav intro-normalized-skip-09.wav

intro-normalized.wav:	intro-cooking-math-4.wav ../Release/ospac
	../Release/ospac --raw --normalize intro-cooking-math-4.wav --output intro-normalized.wav
	-../Release/ospac --raw intro-cooking-math-4.wav intro-normalized.wav --plot normalize-result.ppm
	-pnmscale .5 < normalize-result.ppm | pnmtopng  > normalize-result.png && mv normalize-result.png ../images/
	-rm normalize-result.ppm

volume-changes-leveled.wav:	volume-changes.wav ../Release/ospac
	../Release/ospac --raw --leveler volume-changes.wav --output volume-changes-leveled.wav
	-../Release/ospac --raw volume-changes.wav volume-changes-leveled.wav --plot leveler-result.ppm
	-pnmscale .5 < leveler-result.ppm | pnmtopng > leveler-result.png && mv leveler-result.png ../images/
	-rm leveler-result.ppm

intro-normalized-skip.wav:	intro-normalized.wav ../Release/ospac
	../Release/ospac --raw --skip intro-normalized.wav --output intro-normalized-skip.wav
	-../Release/ospac --raw intro-normalized.wav intro-normalized-skip.wav --plot skip-result.ppm
	-pnmscale .5 < skip-result.ppm |pnmtopng > skip-result.png && mv skip-result.png ../images/
	-rm skip-result.ppm

intro-normalized-skip-09.wav:	intro-normalized.wav ../Release/ospac
	../Release/ospac --raw --skip --skip-target 0.9 intro-normalized.wav --output intro-normalized-skip-09.wav
	-../Release/ospac --raw intro-normalized.wav intro-normalized-skip-09.wav --plot skip-result-09.ppm
	-pnmscale .5 < skip-result-09.ppm |pnmtopng > skip-result-09.png && mv skip-result-09.png ../images/
	-rm skip-result-09.ppm

intro-normalized-skip-08.wav:	intro-normalized.wav ../Release/ospac
	../Release/ospac --raw --skip --skip-target 0.8 intro-normalized.wav --output intro-normalized-skip-08.wav
	-../Release/ospac --raw intro-normalized.wav intro-normalized-skip-08.wav --plot skip-result-08.ppm
	-pnmscale .5 < skip-result-08.ppm |pnmtopng > skip-result-08.png && mv skip-result-08.png ../images/
	-rm skip-result-08.ppm

intro-normalized-skip-07.wav:	intro-normalized.wav ../Release/ospac
	../Release/ospac --raw --skip --skip-target 0.7 intro-normalized.wav --output intro-normalized-skip-07.wav
	-../Release/ospac --raw intro-normalized.wav intro-normalized-skip-07.wav --plot skip-result-07.ppm
	-pnmscale .5 < skip-result-07.ppm |pnmtopng > skip-result-07.png && mv skip-result-07.png ../images/
	-rm skip-result-07.ppm

intro-normalized-skip-06.wav:	intro-normalized.wav ../Release/ospac
	../Release/ospac --raw --skip --skip-target 0.6 intro-normalized.wav --output intro-normalized-skip-06.wav
	-../Release/ospac --raw intro-normalized.wav intro-normalized-skip-06.wav --plot skip-result-06.ppm
	-pnmscale .5 < skip-result-06.ppm |pnmtopng > skip-result-06.png && mv skip-result-06.png ../images/
	-rm skip-result-06.ppm

intro-normalized-skip-05.wav:	intro-normalized.wav ../Release/ospac
	../Release/ospac --raw --skip --skip-target 0.5 intro-normalized.wav --output intro-normalized-skip-05.wav
	-../Release/ospac --raw intro-normalized.wav intro-normalized-skip-05.wav --plot skip-result-05.ppm
	-pnmscale .5 < skip-result-05.ppm |pnmtopng > skip-result-05.png && mv skip-result-05.png ../images/
	-rm skip-result-05.ppm

../images/skip-target.png:	intro-normalized.wav intro-normalized-skip-05.wav intro-normalized-skip-06.wav intro-normalized-skip-07.wav intro-normalized-skip-08.wav intro-normalized-skip-09.wav
	-../Release/ospac --raw intro-normalized.wav \
		intro-normalized-skip-09.wav \
		intro-normalized-skip-08.wav \
		intro-normalized-skip-07.wav \
		intro-normalized-skip-06.wav \
		intro-normalized-skip-05.wav --plot skip-target.ppm
	-pnmscale .5 < skip-target.ppm |pnmtopng > skip-target.png && mv skip-target.png ../images/
	-rm skip-target.ppm

intro-normalized-noise.wav:	intro-normalized.wav ../Release/ospac
	../Release/ospac --raw --noise intro-normalized.wav --output intro-normalized-noise.wav
	-../Release/ospac --raw intro-normalized.wav intro-normalized-noise.wav --plot noise-result.ppm
	-pnmscale .5 < noise-result.ppm |pnmtopng > noise-result.png && mv noise-result.png ../images/
	-rm noise-result.ppm

intro-factor-2.wav:	volume-changes.wav ../Release/ospac
	../Release/ospac --raw --factor 2 volume-changes.wav --output intro-factor-2.wav

intro-factor-4.wav:	volume-changes.wav ../Release/ospac intro-factor-2.wav
	../Release/ospac --raw --factor 4 volume-changes.wav --output intro-factor-4.wav
	-../Release/ospac --raw volume-changes.wav intro-factor-2.wav intro-factor-4.wav --plot factor-result.ppm
	-pnmscale .5 < factor-result.ppm |pnmtopng > factor-result.png && mv factor-result.png ../images/
	-rm factor-result.ppm

intro-xfilter.wav:	intro-normalized.wav intro-reverb-only.wav ../Release/ospac
	../Release/ospac --raw --xfilter intro-normalized.wav intro-reverb-only.wav --output intro-xfilter.wav --plot xfilter-result.ppm
	-pnmscale .5 < xfilter-result.ppm |pnmtopng > xfilter-result.png && mv xfilter-result.png ../images/
	-rm xfilter-result.ppm
	../Release/ospac --raw intro-normalized.wav intro-reverb-only.wav --plot crosstalk-example.ppm
	-pnmscale .5 < crosstalk-example.ppm |pnmtopng > crosstalk-example.png && mv crosstalk-example.png ../images/
	-rm crosstalk-example.ppm

intro-xgate.wav:	intro-normalized.wav intro-reverb-only.wav ../Release/ospac
	../Release/ospac --raw --xgate intro-normalized.wav intro-reverb-only.wav --output intro-xgate.wav --plot xgate-result.ppm
	-pnmscale .5 < xgate-result.ppm |pnmtopng > xgate-result.png && mv xgate-result.png ../images/
	-rm xgate-result.ppm

stereo-test.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac 1.wav 2.wav 3.wav 4.wav 5.wav --voice 5.wav 4.wav 3.wav 2.wav 1.wav --output stereo-test.wav --plot stereo-test.ppm
	../Release/ospac --raw --mono --normalize stereo-test.wav --output stereo-test-mono.wav
	-pnmscale .5 < stereo-test.ppm|pnmtopng > stereo-test.png && mv stereo-test.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 stereo-test.wav stereo-test.mp3 && mv stereo-test.mp3 ../examples/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 stereo-test-mono.wav stereo-test-mono.mp3 && mv stereo-test-mono.mp3 ../examples/
	-rm stereo-test.ppm
	
stereo-strong.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --set-stereo-level 0.25 1.wav 2.wav 3.wav 4.wav 5.wav --voice 5.wav 4.wav 3.wav 2.wav 1.wav --output stereo-strong.wav --plot stereo-strong.ppm
	../Release/ospac --raw --mono --normalize stereo-strong.wav --output stereo-strong-mono.wav
	-pnmscale .5 < stereo-strong.ppm|pnmtopng > stereo-strong.png && mv stereo-strong.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 stereo-strong.wav stereo-strong.mp3 && mv stereo-strong.mp3 ../examples/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 stereo-strong-mono.wav stereo-strong-mono.mp3 && mv stereo-strong-mono.mp3 ../examples/
	-rm stereo-strong.ppm
	
spatial-test.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --spatial 1.wav 2.wav 3.wav 4.wav 5.wav --voice 5.wav 4.wav 3.wav 2.wav 1.wav --output spatial-test.wav --plot spatial-test.ppm
	../Release/ospac --raw --mono --normalize spatial-test.wav --output spatial-test-mono.wav
	-pnmscale .5 < spatial-test.ppm|pnmtopng > spatial-test.png && mv spatial-test.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 spatial-test.wav spatial-test.mp3 && mv spatial-test.mp3 ../examples/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 spatial-test-mono.wav spatial-test-mono.mp3 && mv spatial-test-mono.mp3 ../examples/
	-rm spatial-test.ppm

spatial-strong.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --spatial --set-stereo-spatial 0.2 1.wav 2.wav 3.wav 4.wav 5.wav --voice 5.wav 4.wav 3.wav 2.wav 1.wav --output spatial-strong.wav --plot spatial-strong.ppm
	../Release/ospac --raw --mono --normalize spatial-strong.wav --output spatial-strong-mono.wav
	-pnmscale .5 < spatial-strong.ppm|pnmtopng > spatial-strong.png && mv spatial-strong.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 spatial-strong.wav spatial-strong.mp3 && mv spatial-strong.mp3 ../examples/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 spatial-strong-mono.wav spatial-strong-mono.mp3 && mv spatial-strong-mono.mp3 ../examples/
	-rm spatial-strong.ppm

stereo-mono-test.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --set-stereo-level 1 1.wav 2.wav 3.wav 4.wav 5.wav --voice 5.wav 4.wav 3.wav 2.wav 1.wav --output stereo-mono-test.wav --plot stereo-mono-test.ppm
	-pnmscale .5 < stereo-mono-test.ppm|pnmtopng > stereo-mono-test.png && mv stereo-mono-test.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 stereo-mono-test.wav stereo-mono-test.mp3 && mv stereo-mono-test.mp3 ../examples/
	-rm stereo-mono-test.ppm
	
mono-test.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --mono 1.wav 2.wav 3.wav 4.wav 5.wav --voice 5.wav 4.wav 3.wav 2.wav 1.wav --output mono-test.wav --plot mono-test.ppm
	-pnmscale .5 < mono-test.ppm|pnmtopng > mono-test.png && mv mono-test.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 mono-test.wav mono-test.mp3 && mv mono-test.mp3 ../examples/
	-rm mono-test.ppm

multi-test.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --multi 1.wav 2.wav 3.wav 4.wav 5.wav --voice 5.wav 4.wav 3.wav 2.wav 1.wav --output multi-test.wav --plot multi-test.ppm
	-pnmscale .5 < multi-test.ppm|pnmtopng > multi-test.png && mv multi-test.png ../images/
	-rm multi-test.ppm

H-H1_LOSC_4_V1-1126259446-32.txt:
	wget https://losc.ligo.org/s/events/GW150914/H-H1_LOSC_4_V1-1126259446-32.txt.gz
	gzip -d H-H1_LOSC_4_V1-1126259446-32.txt.gz

L-L1_LOSC_4_V1-1126259446-32.txt:
	wget https://losc.ligo.org/s/events/GW150914/L-L1_LOSC_4_V1-1126259446-32.txt.gz
	gzip -d L-L1_LOSC_4_V1-1126259446-32.txt.gz

ligo-raw.wav: ../Release/ospac H-H1_LOSC_4_V1-1126259446-32.txt L-L1_LOSC_4_V1-1126259446-32.txt
	../Release/ospac --raw --ascii 4096 H-H1_LOSC_4_V1-1126259446-32.txt \
            --ascii 4096 L-L1_LOSC_4_V1-1126259446-32.txt \
            --output ligo-raw.wav
	../Release/ospac --raw ligo-raw.wav --plot ligo-raw.ppm
	-pnmscale .5 < ligo-raw.ppm|pnmtopng > ligo-raw.png && mv ligo-raw.png ../images
	-rm ligo-raw.ppm
	-lame -V1 -q0 --lowpass 22050 --resample 44100 ligo-raw.wav ligo-raw.mp3 && mv ligo-raw.mp3 ../examples/
	
ligo-filter.wav: ../Release/ospac H-H1_LOSC_4_V1-1126259446-32.txt L-L1_LOSC_4_V1-1126259446-32.txt
	../Release/ospac --raw --ascii 4096 H-H1_LOSC_4_V1-1126259446-32.txt \
            --ascii 4096 L-L1_LOSC_4_V1-1126259446-32.txt \
            --bandpass 80 300 10 --normalize \
            --output ligo-filter.wav --plot ligo-filter.ppm
	-pnmscale .5 < ligo-filter.ppm|pnmtopng > ligo-filter.png && mv ligo-filter.png ../images
	-rm ligo-filter.ppm
	-lame -V1 -q0 --lowpass 22050 --resample 44100 ligo-filter.wav ligo-filter.mp3 && mv ligo-filter.mp3 ../examples/
	
ligo-event.wav: ../Release/ospac H-H1_LOSC_4_V1-1126259446-32.txt L-L1_LOSC_4_V1-1126259446-32.txt
	../Release/ospac --raw --ascii 4096 H-H1_LOSC_4_V1-1126259446-32.txt \
            --ascii 4096 L-L1_LOSC_4_V1-1126259446-32.txt \
            --bandpass 80 300 10 \
            --skip --skip-level 0.5 --skip-order 0.95 \
            --normalize \
            --output ligo-event.wav --plot ligo-event.ppm
	-pnmscale .5 < ligo-event.ppm|pnmtopng > ligo-event.png && mv ligo-event.png ../images
	-rm ligo-event.ppm
	-lame -V1 -q0 --lowpass 22050 --resample 44100 ligo-event.wav ligo-event.mp3 && mv ligo-event.mp3 ../examples/

space-44s.wav: Makefile
	 sox -n -r44100 -c1 -t wav space-44s.wav synth whitenoise band -n 100 20 band -n 50 20 gain +30 fade h 1 44 1

intro-normalized-space.wav:	intro-cooking-math-4.wav space-44s.wav ../Release/ospac
	../Release/ospac --stereo --mix --normalize intro-cooking-math-4.wav --parallel --mix space-44s.wav --factor 0.5 --output intro-normalized-space.wav
	-../Release/ospac --raw intro-cooking-math-4.wav intro-normalized-space.wav --plot normalize-space-result.ppm
	-pnmscale .5 < normalize-space-result.ppm | pnmtopng  > normalize-space-result.png && mv normalize-space-result.png ../images/
	-rm normalize-space-result.ppm

dynamic-noise.wav:	dynamic-test.wav ../Release/ospac
	../Release/ospac --raw dynamic-test.wav --plot dynamic-test.ppm
	-pnmscale .5 < dynamic-test.ppm|pnmtopng > dynamic-test.png && mv dynamic-test.png ../images
	../Release/ospac --raw --noise dynamic-test.wav --output dynamic-noise.wav --plot dynamic-noise.ppm
	-pnmscale .5 < dynamic-noise.ppm|pnmtopng > dynamic-noise.png && mv dynamic-noise.png ../images
	
	
dynamic-noise-normalized.wav:	dynamic-test.wav ../Release/ospac
	../Release/ospac --raw --noise dynamic-test.wav --normalize --output dynamic-noise-normalized.wav --plot dynamic-noise-normalized.ppm
	-pnmscale .5 < dynamic-noise-normalized.ppm|pnmtopng > dynamic-noise-normalized.png && mv dynamic-noise-normalized.png ../images


