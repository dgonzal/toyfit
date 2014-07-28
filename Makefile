# Short Makefile to compile "root macros"


TARGET = fitfull

SRCDIR  = src
INCDIR  = include
OBJDIR  = obj
BINDIR  = bin

SOURCES  := $(wildcard $(SRCDIR)/*.C) 
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(patsubst $(SRCDIR)%.C,$(OBJDIR)%.o,$(SOURCES))

CXX      = g++
LINKER   = g++ -o

scramtag = $(shell cd $$CMSSW_BASE; scram tool tag $(1) $(2))

ROOTLIBS :=  $(shell root-config --evelibs)  
ROOTFLAGS := $(shell root-config --cflags)
ROOTINC :=  -I$(call scramtag, root_interface, INCLUDE)

ROOFITLIBS :=  -L$(call scramtag, roofitcore,LIBDIR ) -lRooFit -lRooFitCore -lMinuit 
ROOFITFLAGS := 
ROOFITINC :=  -I$(call scramtag, roofitcore, INCLUDE) 


USERLDFLAGS := $(ROOTLIBS) $(ROOFITLIBS)
USERCXXFLAGS := -g -Wall -O2 
USERCXXFLAGS += $(ROOTFLAGS) $(ROOFITFLAGS)
USERINCFLAGS := -I$(INCDIR) $(ROOTINC) $(ROOFITINC)

$(TARGET): $(OBJECTS)
	@$(LINKER) $@  $(OBJECTS) $(USERLDFLAGS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.C
	@mkdir -p $(OBJDIR)	
	@$(CXX) $< $(USERINCFLAGS) $(CXX_DEFINES) $(USERCXXFLAGS) -c -o $(OBJDIR)/$(notdir $@) 
	@echo "Compiled "$<" successfully!"

.PHONEY: clean
clean:
	@rm $(OBJDIR)/*
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@rm $(TARGET)
	@echo "Executable removed!"





