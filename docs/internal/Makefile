# test makefile for specific builds, place into project's root directory

MKDIR   := md
CC      := g++
OBJ     := ./
INCLUDE := ./inc
SRC     := ./src
SRCS    := $(wildcard $(SRC)/*.cpp)
OBJS    := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))
EXE     := scripal
CFLAGS  := -I$(INCLUDE) -fPIC -Wno-attributes -finline-functions -std=gnu++17 -o3
LDLIBS  := -lPocoFoundation -lPocoUtil -lPocoJSON -lPocoEncodings -lPocoZip
LDFLAGS := -o3

.PHONY: all run clean

all: $(EXE)

$(EXE): $(OBJS) 
		$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ)/%.o: $(SRC)/%.cpp | $(OBJ)
		$(CC) $(CFLAGS) -c $< -o $@

$(BIN) $(OBJ):
		$(MKDIR) $@

run: $(EXE)
    $<

clean:
		rm -f *.o; rm scripal
