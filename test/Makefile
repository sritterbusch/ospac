all:	volume-changes-leveled.wav intro-normalized-skip.wav intro-factor-2.wav intro-factor-4.wav intro-xfilter.wav intro-xgate.wav

clean:
	rm -f intro-normalized.wav volume-changes-leveled.wav intro-normalized-skip.wav intro-factor-2.wav intro-factor-4.wav intro-xfilter.wav intro-xgate.wav *~

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
	