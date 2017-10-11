
PROJECT=xelite.so
OBJ=$(HOME)/obj/x64/Debug/$(PROJECT)
LIBOUT=$(HOME)/deploy/lib

CC=g++
INCLUDE=-I$(HOME)/include/Xelite -I$(HOME)/include/cardac
sobjects=$(patsubst %.cpp,$(OBJ)/%.o,$(wildcard *.cpp))

vpath %.cpp .
vpath %.o $(OBJ)

$(OBJ)/%.o: %.cpp
	-@mkdir -p $(OBJ)
	    $(CC) $(INCLUDE) -fPIC -std=c++0x -c $< -o $(OBJ)/$*.o


$(LIBOUT)/$(PROJECT): $(sobjects)
	-@mkdir -p $(LIBOUT)
	$(CC) -shared $(sobjects) -o $(LIBOUT)/$(PROJECT)

cleanall:
	-rm $(OBJ)/*.o
	-rm $(LIBOUT)/$(PROJECT)

all: $(LIBOUT)/$(PROJECT)
	
