#----------------------------------------------------------------------------
# others : cleanup
#----------------------------------------------------------------------------
clean: ; rm -f *.o core core.* *.log $(DEPENDENCY) $(OUT) $(OUT_TOCLEAN)

dep: $(SRCS)
	$(CC) -M $(CPPFLAGS) $(SRCS) > $(DEPENDENCY)

-include $(DEPENDENCY)
