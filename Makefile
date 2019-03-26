OBJFOLDER = obj
BUILDFOLDER = build
RESOURCEFOLDER = res
DATAFOLDER = gameData
SOURCEFOLDER = src

ifeq ($(RELEASE), 1)
	CXXFLAGS = -O3 -frename-registers -flto -s
	LDFLGAGS = -s
	EXECUTABLENAME_DEF = DoukutsuRelease
else
	CXXFLAGS = -Og -ggdb
	EXECUTABLENAME_DEF = DoukutsuDebug
	ifneq ($(CONSOLE), 0)
		CONSOLE = 1
endif
endif

ifeq ($(JAPANESE), 1)
	CXXFLAGS += -DJAPANESE

	ifeq ($(RELEASE), 1)
		EXECUTABLENAME_DEF = DoukutsuReleaseJp
	else
		EXECUTABLENAME_DEF = DoukutsuDebugJp
	endif
endif

EXECUTABLENAME ?= $(EXECUTABLENAME_DEF)

ifeq ($(FIX_BUGS), 1)
	CXXFLAGS += -DFIX_BUGS
endif

ifeq ($(WINDOWS), 1)
	ifeq ($(CONSOLE), 1)
		CXXFLAGS += -mconsole
	endif

	CXXFLAGS += -DWINDOWS
	LIBS += -lkernel32
endif

ifeq ($(RASPBERRY_PI), 1)
	CXXFLAGS += -DRASPBERRY_PI
endif

ifneq ($(WARNINGS), 1)
	CXXFLAGS += -Wall -Wextra
endif

ifeq ($(ALL_WARNINGS),1)
	ifneq ($(findstring clang,$(CXX)),)
		# Use clang-specific flag -Weverything
		CXXFLAGS += -Weverything
	else
		# Assume g++-compatible compiler
		CXXFLAGS += -pedantic -Wall -Wextra -Walloc-zero -Walloca -Wbool-compare -Wcast-align -Wcast-qual -Wchar-subscripts -Wchkp -Wctor-dtor-privacy -Wdangling-else -Wdisabled-optimization -Wdouble-promotion -Wduplicated-branches -Wduplicated-cond -Wfloat-conversion -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat-y2k -Wimport -Winit-self -Winvalid-pch -Wlogical-not-parentheses -Wlogical-op -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wnoexcept -Wnoexcept-type -Wnon-virtual-dtor -Wnormalized=nfc -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wregister -Wreorder -Wrestrict -Wshadow -Wsizeof-array-argument -Wstack-protector -Wstrict-aliasing=3 -Wstrict-null-sentinel -Wsuggest-attribute=const -Wsuggest-attribute=format -Wsuggest-attribute=noreturn -Wsuggest-override -Wstrict-null-sentinel -Wswitch-bool -Wundef -Wunreachable-code -Wunused -Wunused-local-typedefs -Wunused-macros -Wunused-parameter -Wvariadic-macros -Wwrite-strings -Wzero-as-null-pointer-constant
	endif
endif

# Base command-line when calling the compiler
# `sdl2-config --cflags` to get the flags needed to compile with SDL2
# -MMD -MP -MF $@.d to make the compiler generate dependency files
CXXFLAGS += `sdl2-config --cflags` `pkg-config freetype2 --cflags` -MMD -MP -MF $@.d -DLODEPNG_NO_COMPILE_ENCODER -DLODEPNG_NO_COMPILE_ERROR_TEXT -DLODEPNG_NO_COMPILE_CPP
LIBS += `sdl2-config --static-libs` `pkg-config freetype2 --libs`

ifeq ($(STATIC), 1)
	LDFLAGS += -static
	LIBS += `pkg-config sdl2 --libs --static` `pkg-config freetype2 --libs --static` -lfreetype
else
	LIBS += `pkg-config sdl2 --libs` `pkg-config freetype2 --libs`
endif

# For an accurate result to the original's code, compile in alphabetical order
SOURCES = \
	lodepng/lodepng \
	ArmsItem \
	Back \
	Boss \
	BossAlmo1 \
	BossAlmo2 \
	BossBallos \
	BossFrog \
	BossIronH \
	BossLife \
	BossOhm \
	BossPress \
	BossTwinD \
	BossX \
	BulHit \
	Bullet \
	Caret \
	Config \
	Draw \
	Ending \
	Escape \
	Fade \
	File \
	Flags \
	Flash \
	Font \
	Frame \
	Game \
	Generic \
	GenericLoad \
	Input \
	KeyControl \
	Main \
	Map \
	MapName \
	MiniMap \
	MyChar \
	MycHit \
	MycParam \
	NpcAct000 \
	NpcAct020 \
	NpcAct040 \
	NpcAct060 \
	NpcAct080 \
	NpcAct100 \
	NpcAct120 \
	NpcAct140 \
	NpcAct160 \
	NpcAct180 \
	NpcAct200 \
	NpcAct220 \
	NpcAct240 \
	NpcAct260 \
	NpcAct280 \
	NpcAct300 \
	NpcAct320 \
	NpcAct340 \
	NpChar \
	NpcHit \
	NpcTbl \
	Organya \
	PixTone \
	Profile \
	Resource \
	SelStage \
	Shoot \
	Sound \
	Stage \
	Star \
	TextScr \
	Triangle \
	ValueView

RESOURCES =

OBJECTS = $(addprefix $(OBJFOLDER)/$(EXECUTABLENAME)/, $(addsuffix .o, $(SOURCES)))
DEPENDENCIES := $(addsuffix .d, $(OBJS))
DEPENDENCIES = $(addprefix obj/$(FILENAME)/, $(addsuffix .o.d, $(SOURCES)))

ifeq ($(WINDOWS), 1)
	OBJECTS += $(OBJFOLDER)/$(EXECUTABLENAME)/win_icon.o
endif

all: $(BUILDFOLDER)/$(EXECUTABLENAME) dummyDataTarget
	@echo Build finished without errors !

$(BUILDFOLDER)/$(EXECUTABLENAME): $(OBJECTS)
	@mkdir -p $(@D)
	@echo Linking to $@...
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)
	@echo Finished compiling $@

# Do this unless we find a way to use rsync to check if there is any change to be made in some way
dummyDataTarget:
	@echo Syncing data files...
	@rsync -a $(DATAFOLDER)/ $(BUILDFOLDER)/
	@echo Synced data files

$(OBJFOLDER)/$(EXECUTABLENAME)/%.o: $(SOURCEFOLDER)/%.cpp
	@mkdir -p $(@D)
	@echo Compiling $<...
	@$(CXX) $(CXXFLAGS) $< -o $@ -c
	@echo Finished compiling $<

$(OBJFOLDER)/$(EXECUTABLENAME)/Resource.o: $(SOURCEFOLDER)/Resource.cpp $(addprefix $(SOURCEFOLDER)/Resource/, $(addsuffix .h, $(RESOURCES)))
	@mkdir -p $(@D)
	@echo Compiling $<...
	@$(CXX) $(CXXFLAGS) $< -o $@ -c
	@echo Finished compiling $<

$(SOURCEFOLDER)/Resource/%.h: $(RESOURCEFOLDER)/% $(OBJFOLDER)/bin2h
	@mkdir -p $(@D)
	@echo Converting $<...
	@$(OBJFOLDER)/bin2h $< $@
	@echo Finished converting $<

$(OBJFOLDER)/bin2h: $(RESOURCEFOLDER)/bin2h.c
	@mkdir -p $(@D)
	@echo Compiling $^...
	@$(CC) -O3 -s -static $^ -o $@
	@echo Finished compiling $^

$(OBJFOLDER)/$(EXECUTABLENAME)/win_icon.o: $(RESOURCEFOLDER)/ICON/ICON.rc $(RESOURCEFOLDER)/ICON/0.ico $(RESOURCEFOLDER)/ICON/ICON_MINI.ico
	@mkdir -p $(@D)
	@echo Making $^...
	@windres $< $@
	@echo Finished making $^

# Include dependencies
include $(wildcard $(DEPENDENCIES))
	
clean:
	@rm -rf $(BUILDFOLDER) $(OBJFOLDER)
