WORKDIR = %cd%

PROJECT = vfw_generator

CC = gcc.exe
CXX = g++.exe
AR = ar.exe
LD = g++.exe
WINDRES = windres.exe

INC = -Isrc
CFLAGS = -Wall
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -O2 -s
OBJDIR = obj\\Release
DEP = 
OUT = bin\\Release\\$(PROJECT).exe

OBJ = $(OBJDIR)\\src\\main.o $(OBJDIR)\\src\\app.o $(OBJDIR)\\src\\log.o $(OBJDIR)\\src\\crc32.o $(OBJDIR)\\src\\crc16.o $(OBJDIR)\\src\\ihex.o $(OBJDIR)\\src\\xtea.o $(OBJDIR)\\src\\brates.o

all: before out after

clean:
	cmd /c del /f $(OBJ) $(OUT)
	cmd /c rd bin\\Release
	cmd /c rd $(OBJDIR)\\src
	cmd /c rd $(OBJDIR)

before: 
	cmd /c if not exist bin\\Release md bin\\Release
	cmd /c if not exist $(OBJDIR) md $(OBJDIR)
	cmd /c if not exist $(OBJDIR)\\src md $(OBJDIR)\\src

after: 

out: before $(OBJ) $(DEP)
	$(LD) $(LIBDIR) -o $(OUT) $(OBJ)  $(LDFLAGS) $(LIB)

$(OBJDIR)\\src\\%.o: src\\%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.PHONY: before after
