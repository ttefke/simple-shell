objects = sish.o exec.o cd.o util.o
executable = sish

sish : $(objects)
	cc -o sish $(objects)

sish.o : sish.c include/sish.h
	cc -c -g -std=c11 sish.c

exec.o : exec.c include/exec.h
	cc -c -g -std=c11 exec.c

cd.o : cd.c include/cd.h
	cc -c -g -std=c11 cd.c

util.o: util.c include/util.h
	cc -c -g -std=c11 util.c

clean :
	-rm sish $(objects)
