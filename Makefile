TARGETS   = Expr fturtle
OBJECTS   = ftlex.o ftparse.o ftparse.hpp ftparse.cpp
LDLIBS    = -lfl
CPPFLAGS += -g -std=c++0x

.PHONY: all clean

%.cpp: %.lpp
	$(LEX) $(LFLAGS) $^
	mv -f lex.yy.c $@

%.cpp: %.ypp
	$(YACC) $(YFLAGS) $^
	mv -f y.tab.c $@

%.hpp: %.ypp
	$(YACC) $(YFLAGS) -d $^
	$(RM) y.tab.c
	mv -f y.tab.h $@

fturtle: ftlex.o ftparse.o
	c++ -o fturtle ftlex.o ftparse.o -ll
	rm ftlex.o ftparse.o
all: $(TARGETS)

clean:
	$(RM) $(TARGETS) $(OBJECTS)
ftlex.o: ftparse.hpp
Expr: Expr.hpp
fturtle: Expr.hpp
