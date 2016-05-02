all:	ospac

ospac:	src/*cpp src/*h
	cd Release;make all;cd ..
	
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

test:	Release/ospac
	cd test;make all;cd ..
	
install-ospac:	Release/ospac
	cd Release;make install;cd ..
	
install-gui:	GUI-Release/ospac-gui
	cd GUI-Release;make install-gui;cd ..
	
install: install-ospac

uninstall:
	cd Release;make uninstall; cd ..
	cd GUI-Release;make uninstall; cd ..

uninstall-gui:
	cd Release;make uninstall; cd ..
	cd GUI-Release;make uninstall; cd ..

clean:
	cd Release;make clean; cd ..
	cd GUI-Release;make clean; cd ..
	cd test;make clean;cd ..
	rm -f *~
	rm -f src/*~

	