all:	src/*cpp src/*h
	cd Release;make all;cd ..
	
Release/ospac:	src/*cpp src/*h
	cd Release;make all;cd ..

test:	Release/ospac
	cd test;make all;cd ..
	
install:	Release/ospac
	cd Release;make install;cd ..
	
uninstall:
	cd Release;make uninstall; cd ..

clean:
	cd Release;make clean; cd ..
	cd test;make clean;cd ..
	rm -f *~
	rm -f src/*~

	