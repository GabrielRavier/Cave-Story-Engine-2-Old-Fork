ifeq ($(RELEASE), 1)
	CXXFLAGS = -O3 -frename-registers -flto -s
	FILENAME_DEF = DoukutsuRelease
else
	CXXFLAGS = -Og -ggdb
	FILENAME_DEF = DoukutsuDebug
	ifneq ($(CONSOLE), 0)
		CONSOLE = 1
	endif
endif

ifeq ($(JAPANESE), 1)
	CXXFLAGS += -DJAPANESE
	LIBS += -liconv
	ifeq ($(RELEASE), 1)
		FILENAME_DEF = DoukutsuReleasejp
	else
		FILENAME_DEF = DoukutsuDebugjp
	endif
endif

FILENAME ?= $(FILENAME_DEF)

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

ifeq ($(WARNINGS), 1)
	CXXFLAGS += -Wall -Wextra
endif

ifeq ($(ALL_WARNINGS), 1)
	CXXFLAGS += -pedantic -Wall -Wextra -Walloc-zero -Walloca -Wbool-compare -Wcast-align -Wcast-qual -Wchar-subscripts -Wchkp -Wctor-dtor-privacy -Wdangling-else -Wdisabled-optimization -Wdouble-promotion -Wduplicated-branches -Wduplicated-cond -Wfloat-conversion -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat-y2k -Wimport -Winit-self -Winvalid-pch -Wlogical-not-parentheses -Wlogical-op -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wnoexcept -Wnoexcept-type -Wnon-virtual-dtor -Wnormalized=nfc -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wregister -Wreorder -Wrestrict -Wshadow -Wsizeof-array-argument -Wstack-protector -Wstrict-aliasing=3 -Wstrict-null-sentinel -Wsuggest-attribute=const -Wsuggest-attribute=format -Wsuggest-attribute=noreturn -Wsuggest-override -Wstrict-null-sentinel -Wswitch-bool -Wundef -Wunreachable-code -Wunused -Wunused-local-typedefs -Wunused-macros -Wunused-parameter -Wvariadic-macros -Wwrite-strings -Wzero-as-null-pointer-constant
endif

# Base command-line when calling the compiler
# `sdl2-config --cflags` to get the flags needed to compile with SDL2
# -MMD -MP -MF $@.d to make the compiler generate dependency files
CXXFLAGS += `sdl2-config --cflags` `pkg-config freetype2 --cflags` -MMD -MP -MF $@.d
LIBS += `sdl2-config --static-libs` `pkg-config freetype2 --libs`

ifeq ($(STATIC), 1)
CXXFLAGS += -static
LIBS += -lharfbuzz -lfreetype -lbz2 -lpng -lz -lgraphite2 -lRpcrt4 -lDwrite -lusp10
endif

# For an accurate result to the original's code, compile in alphabetical order
SOURCES = \
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
	Shoot \
	Sound \
	Stage \
	Star \
	SelStage \
	TextScr \
	Triangle \
	ValueView

RESOURCES = \
	BITMAP/CREDIT01.bmp \
	BITMAP/CREDIT02.bmp \
	BITMAP/CREDIT03.bmp \
	BITMAP/CREDIT04.bmp \
	BITMAP/CREDIT05.bmp \
	BITMAP/CREDIT06.bmp \
	BITMAP/CREDIT07.bmp \
	BITMAP/CREDIT08.bmp \
	BITMAP/CREDIT09.bmp \
	BITMAP/CREDIT10.bmp \
	BITMAP/CREDIT11.bmp \
	BITMAP/CREDIT12.bmp \
	BITMAP/CREDIT14.bmp \
	BITMAP/CREDIT15.bmp \
	BITMAP/CREDIT16.bmp \
	BITMAP/CREDIT17.bmp \
	BITMAP/CREDIT18.bmp \
	CURSOR/CURSOR_IKA.bmp \
	CURSOR/CURSOR_NORMAL.bmp \
	ORG/ACCESS \
	ORG/ANZEN \
	ORG/BALCONY \
	ORG/BALLOS \
	ORG/BDOWN \
	ORG/CEMETERY \
	ORG/CURLY \
	ORG/DR \
	ORG/ENDING \
	ORG/ESCAPE \
	ORG/FANFALE1 \
	ORG/FANFALE2 \
	ORG/FANFALE3 \
	ORG/FIREEYE \
	ORG/GAMEOVER \
	ORG/GINSUKE \
	ORG/GRAND \
	ORG/GRAVITY \
	ORG/HELL \
	ORG/IRONH \
	ORG/JENKA \
	ORG/JENKA2 \
	ORG/KODOU \
	ORG/LASTBT3 \
	ORG/LASTBTL \
	ORG/LASTCAVE \
	ORG/MARINE \
	ORG/MAZE \
	ORG/MDOWN2 \
	ORG/MURA \
	ORG/OSIDE \
	ORG/PLANT \
	ORG/QUIET \
	ORG/REQUIEM \
	ORG/TOROKO \
	ORG/VIVI \
	ORG/WANPAK2 \
	ORG/WANPAKU \
	ORG/WEED \
	ORG/WHITE \
	ORG/XXXX \
	ORG/ZONBIE \
	WAVE/WAVE100

ifeq ($(JAPANESE), 1)
	RESOURCES += BITMAP/PIXEL_JP.bmp
else
	RESOURCES += BITMAP/PIXEL.bmp
endif

ifneq ($(WINDOWS), 1)
	RESOURCES += ICON/ICON_MINI.bmp
endif

OBJECTS = $(addprefix obj/$(FILENAME)/, $(addsuffix .o, $(SOURCES)))

ifeq ($(WINDOWS), 1)
	OBJECTS += obj/$(FILENAME)/win_icon.o
endif

all: build/$(FILENAME)

build/$(FILENAME): $(OBJECTS)
	@mkdir -p $(@D)
	@echo Linking to $@...
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
	@echo Finished compiling $@
	@echo Copying data files...
	@rsync -a gameData/ build/
	@echo Copied data files

obj/$(FILENAME)/%.o: src/%.cpp
	@mkdir -p $(@D)
	@echo Compiling $^...
	@$(CXX) $(CXXFLAGS) $^ -o $@ -c
	@echo Finished compiling $^

obj/$(FILENAME)/Resource.o: src/Resource.cpp $(addprefix src/Resource/, $(addsuffix .h, $(RESOURCES)))
	@mkdir -p $(@D)
	@echo Compiling $<...
	@$(CXX) $(CXXFLAGS) $< -o $@ -c
	@echo Finished compiling $<

src/Resource/%.h: res/% obj/bin2h
	@mkdir -p $(@D)
	@echo Converting $<...
	@obj/bin2h $< $@
	@echo Finished converting $<

obj/bin2h: res/bin2h.c
	@mkdir -p $(@D)
	@echo Compiling $^...
	@$(CC) -O3 -s -static $^ -o $@
	@echo Finished compiling $^

obj/$(FILENAME)/win_icon.o: res/ICON/ICON.rc res/ICON/0.ico res/ICON/ICON_MINI.ico
	@mkdir -p $(@D)
	@echo Making $^...
	@windres $< $@
	@echo Finished making $^

clean:
	@rm -rf build obj
