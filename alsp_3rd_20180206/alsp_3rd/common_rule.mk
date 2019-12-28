#----------------------------------------------------------------------------
# Suffix rules
#----------------------------------------------------------------------------
.SUFFIXES: 
.PRECIOUS: .o .c

%.c: %.awk
	$(AWK) -f $< $*.data | tee $@
%: %.o
	$(CC) $(CFLAGS) $< $(LOADLIBES) $(LDLIBS) -o $@
%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%: %.cpp
	$(CXX) $(CXXFLAGS) $< $(LOADLIBES) $(LDLIBS) -o $@
