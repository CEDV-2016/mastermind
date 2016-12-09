EXEC := Mastermind

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/
DIRSRC_STATES := $(DIRSRC)/states/
DIRHEA_STATES := $(DIRHEA)/states/

CXX := g++

# Compiling flags --------------------------------------------------------------
CXXFLAGS := -I$(DIRHEA) -I$(DIRHEA_STATES) -Wall -I/usr/local/include/cegui-0/CEGUI -I/usr/local/include/cegui-0 `pkg-config --cflags OGRE OGRE-Overlay` -std=c++11 `pkg-config --cflags OIS`

# Flags del linker ---------------------------------------------------
LDFLAGS := `pkg-config --libs-only-L OGRE` -lOIS -lGL -lstdc++ -lboost_system  -lCEGUIBase-0 -lCEGUIOgreRenderer-0
LDLIBS := `pkg-config --libs-only-l OGRE OGRE-Overlay` -lboost_system -lOIS -lGL -lstdc++ -lCEGUIBase-0 -lCEGUIOgreRenderer-0

# Compiling mode (-mode=release -mode=debug) -----------------------------------
ifeq ($(mode), release)
	CXXFLAGS += -O2 -D_RELEASE
else
	CXXFLAGS += -g -D_DEBUG
	mode := debug
endif

OBJS := $(subst $(DIRSRC), $(DIROBJ), $(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))
OBJS += $(subst $(DIRSRC_STATES), $(DIROBJ), $(patsubst %.cpp, %.o, $(wildcard $(DIRSRC_STATES)*.cpp)))

.PHONY: all clean

all: dir $(EXEC)
	./$(EXEC)

info:
	@echo '------------------------------------------------------'
	@echo '>>> Using mode $(mode)'
	@echo '    (Call "make" with [mode=debug|release])  '
	@echo '------------------------------------------------------'

# Linking -...........----------------------------------------------------------
$(EXEC): $(OBJS)
	@echo "Linking $^"
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compiling -............-------------------------------------------------------
$(DIROBJ)%.o: $(DIRSRC)%.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC_STATES)%.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDLIBS)

# Temporals cleaning ..............---------------------------------------------
clean:
	$(RM) -r *.log $(EXEC) *~ $(DIROBJ) $(DIRSRC)*~ $(DIRHEA)*~

dir:
	mkdir -p obj/

edit:
	atom .
