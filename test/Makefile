all:	volume-changes-leveled.wav intro-normalized-skip.wav intro-normalized-noise.wav \
	intro-factor-2.wav intro-factor-4.wav intro-xfilter.wav intro-xgate.wav \
	stereo-test.wav spatial-test.wav spatial-strong.wav stereo-strong.wav stereo-mono-test.wav mono-test.wav multi-test.wav \
	ligo-raw.wav ligo-filter.wav ligo-event.wav

clean:
	rm -f 	intro-normalized.wav volume-changes-leveled.wav intro-normalized-skip.wav intro-normalized-noise.wav \
		intro-factor-2.wav intro-factor-4.wav intro-xfilter.wav intro-xgate.wav \
		stereo-test.wav spatial-test.wav spatial-strong.wav stereo-strong.wav \
		stereo-test-mono.wav spatial-test-mono.wav spatial-strong-mono.wav stereo-strong-mono.wav \
		stereo-mono-test.wav mono-test.wav multi-test.wav \
		H-H1_LOSC_4_V1-1126259446-32.txt L-L1_LOSC_4_V1-1126259446-32.txt \
		ligo-raw.wav ligo-filter.wav ligo-event.wav *~ *ppm 

intro-normalized.wav:	intro-cooking-math-4.wav ../Release/ospac
	../Release/ospac --raw --normalize intro-cooking-math-4.wav --output intro-normalized.wav
	-../Release/ospac --raw intro-cooking-math-4.wav intro-normalized.wav --plot normalize-result.ppm
	-pnmscale .5 < normalize-result.ppm | pnmtopng  > normalize-result.png && mv normalize-result.png ../images/
	-rm normalize-result.ppm

volume-changes-leveled.wav:	volume-changes.wav ../Release/ospac
	../Release/ospac --raw --leveler volume-changes.wav --target 8000 --output volume-changes-leveled.wav
	-../Release/ospac --raw volume-changes.wav volume-changes-leveled.wav --plot leveler-result.ppm
	-pnmscale .5 < leveler-result.ppm | pnmtopng > leveler-result.png && mv leveler-result.png ../images/
	-rm leveler-result.ppm

intro-normalized-skip.wav:	intro-normalized.wav ../Release/ospac
	../Release/ospac --raw --skip intro-normalized.wav --output intro-normalized-skip.wav
	-../Release/ospac --raw intro-normalized.wav intro-normalized-skip.wav --plot skip-result.ppm
	-pnmscale .5 < skip-result.ppm |pnmtopng > skip-result.png && mv skip-result.png ../images/
	-rm skip-result.ppm

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
	../Release/ospac {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav --output stereo-test.wav
	../Release/ospac --raw stereo-test.wav --plot stereo-test.ppm
	../Release/ospac --raw --mono --normalize stereo-test.wav --output stereo-test-mono.wav
	-pnmscale .5 < stereo-test.ppm|pnmtopng > stereo-test.png && mv stereo-test.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 stereo-test.wav stereo-test.mp3 && mv stereo-test.mp3 ../examples/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 stereo-test-mono.wav stereo-test-mono.mp3 && mv stereo-test-mono.mp3 ../examples/
	-rm stereo-test.ppm
	
stereo-strong.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --set-stereo-level 0.25 {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav --output stereo-strong.wav --plot stereo-strong.ppm
	../Release/ospac --raw stereo-strong.wav --plot stereo-strong.ppm
	../Release/ospac --raw --mono --normalize stereo-strong.wav --output stereo-strong-mono.wav
	-pnmscale .5 < stereo-strong.ppm|pnmtopng > stereo-strong.png && mv stereo-strong.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 stereo-strong.wav stereo-strong.mp3 && mv stereo-strong.mp3 ../examples/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 stereo-strong-mono.wav stereo-strong-mono.mp3 && mv stereo-strong-mono.mp3 ../examples/
	-rm stereo-strong.ppm
	
spatial-test.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --spatial {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav --output spatial-test.wav
	../Release/ospac --raw spatial-test.wav --plot spatial-test.ppm
	../Release/ospac --raw --mono --normalize spatial-test.wav --output spatial-test-mono.wav
	-pnmscale .5 < spatial-test.ppm|pnmtopng > spatial-test.png && mv spatial-test.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 spatial-test.wav spatial-test.mp3 && mv spatial-test.mp3 ../examples/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 spatial-test-mono.wav spatial-test-mono.mp3 && mv spatial-test-mono.mp3 ../examples/
	-rm spatial-test.ppm

spatial-strong.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --spatial --set-stereo-spatial 0.2 {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav --output spatial-strong.wav
	../Release/ospac --raw spatial-strong.wav --plot spatial-strong.ppm
	../Release/ospac --raw --mono --normalize spatial-strong.wav --output spatial-strong-mono.wav
	-pnmscale .5 < spatial-strong.ppm|pnmtopng > spatial-strong.png && mv spatial-strong.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 spatial-strong.wav spatial-strong.mp3 && mv spatial-strong.mp3 ../examples/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 spatial-strong-mono.wav spatial-strong-mono.mp3 && mv spatial-strong-mono.mp3 ../examples/
	-rm spatial-strong.ppm

stereo-mono-test.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --set-stereo-level 1 {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav --output stereo-mono-test.wav
	../Release/ospac --raw stereo-mono-test.wav --plot stereo-mono-test.ppm
	-pnmscale .5 < stereo-mono-test.ppm|pnmtopng > stereo-mono-test.png && mv stereo-mono-test.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 stereo-mono-test.wav stereo-mono-test.mp3 && mv stereo-mono-test.mp3 ../examples/
	-rm stereo-mono-test.ppm
	
mono-test.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --mono {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav --output mono-test.wav
	../Release/ospac --raw mono-test.wav --plot mono-test.ppm
	-pnmscale .5 < mono-test.ppm|pnmtopng > mono-test.png && mv mono-test.png ../images/
	-lame -V8.5 -q0 --lowpass 22050 --resample 44100 mono-test.wav mono-test.mp3 && mv mono-test.mp3 ../examples/
	-rm mono-test.ppm

multi-test.wav:	1.wav 2.wav 3.wav 4.wav 5.wav ../Release/ospac
	../Release/ospac --multi {1,2,3,4,5}.wav --voice {5,4,3,2,1}.wav --output multi-test.wav
	../Release/ospac --raw multi-test.wav --plot multi-test.ppm
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
            --output ligo-filter.wav
	../Release/ospac --raw ligo-filter.wav --plot ligo-filter.ppm
	-pnmscale .5 < ligo-filter.ppm|pnmtopng > ligo-filter.png && mv ligo-filter.png ../images
	-rm ligo-filter.ppm
	-lame -V1 -q0 --lowpass 22050 --resample 44100 ligo-filter.wav ligo-filter.mp3 && mv ligo-filter.mp3 ../examples/
	
ligo-event.wav: ../Release/ospac H-H1_LOSC_4_V1-1126259446-32.txt L-L1_LOSC_4_V1-1126259446-32.txt
	../Release/ospac --raw --ascii 4096 H-H1_LOSC_4_V1-1126259446-32.txt \
            --ascii 4096 L-L1_LOSC_4_V1-1126259446-32.txt \
            --bandpass 80 300 10 \
            --skip --skip-level 0.5 --skip-order 0.95 \
            --normalize \
            --output ligo-event.wav
	../Release/ospac --raw ligo-event.wav --plot ligo-event.ppm
	-pnmscale .5 < ligo-event.ppm|pnmtopng > ligo-event.png && mv ligo-event.png ../images
	-rm ligo-event.ppm
	-lame -V1 -q0 --lowpass 22050 --resample 44100 ligo-event.wav ligo-event.mp3 && mv ligo-event.mp3 ../examples/
	