all:	ospac

ospac:	src/*cpp src/*h
	cd Release;make all;cd ..

ospac-ffmpeg:	src/*cpp src/*h
	cd Release-ffmpeg;make all;cd ..
	
gui:	ospac-gui
	
ospac-gui:	src/*cpp src/*h
	cd GUI-Release;make all;cd ..

ospac-gui-linux:	src/*cpp src/*h
	cd GUI-Release;make ospac-gui-linux;cd ..
	
GUI-Release/ospac-gui:	src/*cpp src/*h
	cd GUI-Release;make all;cd ..

GUI-Release/ospac-gui-linux:	src/*cpp src/*h
	cd GUI-Release;make ospac-gui-linux;cd ..

Release/ospac:	src/*cpp src/*h
	cd Release;make all;cd ..

tab-completion: create_tab_completion.py
	python3 create_tab_completion.py zsh zsh_completion.sh
	python3 create_tab_completion.py bash bash_completion.sh

test:	Release/ospac
	cd test;make all;cd ..
	
install-ospac:	Release/ospac
	cd Release;make install;cd ..
	
install-ospac-ffmpeg:	Release-ffmpeg/ospac
	cd Release-ffmpeg;make install;cd ..

install-gui:	GUI-Release/ospac-gui
	cd GUI-Release;make install-gui;cd ..
	
install: install-ospac

uninstall:
	cd Release;make uninstall; cd ..
	cd Release-ffmpeg;make uninstall; cd ..
	cd GUI-Release;make uninstall; cd ..

uninstall-gui:
	cd Release;make uninstall; cd ..
	cd Release-ffmpeg;make uninstall; cd ..
	cd GUI-Release;make uninstall; cd ..

clean:
	cd Release;make clean; cd ..
	cd Release-ffmpeg;make clean; cd ..
	cd GUI-Release;make clean; cd ..
	cd test;make clean;cd ..
	rm -f *~
	rm -f src/*~
	rm zsh_completion.sh bash_completion.sh
