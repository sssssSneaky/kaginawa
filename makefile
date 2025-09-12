PRJ=kaginawa

build:
	gcc -fPIC -shared -ldl -o $(PRJ).so $(PRJ).c
	strip --strip-all $(PRJ).so

clean:
	-rm $(PRJ).so