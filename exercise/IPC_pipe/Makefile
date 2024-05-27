include ../Make.defines

.PHONY : all  objs clean  
SOUREC := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOUREC))
EXES := $(patsubst %.o,%,$(OBJS))
all : $(EXES)
# 所有可执行文件都带_WQL后缀
% : %.o
	$(CXX) $< -o $@ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)
objs : $(OBJS)

$(OBJS) :%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< 

clean:
	$(RM) $(CLEANFILES)
# 	清理所有，包括可执行文件。	
veryclean:
	$(RM) $(CLEANFILES) $(EXES)