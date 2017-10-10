testapp: testapp.o I18N.o
	g++ -o testapp.exe testapp.o I18N.o

testapp.o: testapp.cpp ../../I18N.h
	g++ -c -DUNICODE testapp.cpp

I18N.o: ../../I18N.cpp
	g++ -c -DUNICODE -I. ../../I18N.cpp

clean:
	rm testapp.exe testapp.o I18N.o


