LIB_OBJS = error.o readn.o writen.o
LIB_NAME = libtool.a
RANLIB = ranlib
CLEANFILES = core core.* *.core *.o temp.* *.out typescript* \
		*.lc *.lh *.bsdi *.sparc *.uw
all:	${LIB_OBJS}
		ar rv ${LIB_NAME} $?
		${RANLIB} ${LIB_NAME}

%.o : %.cpp
	g++  -I../include -c -o $@ $^ 


clean:
		rm -f ${PROGS} ${CLEANFILES}
