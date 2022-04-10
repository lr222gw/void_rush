#Implicit Rules
CXX = clang++
#CXX = g++
CXXFLAGS = -glldb -std=c++17
#CXXFLAGS = -g -std=c++17
#Explicit rules
SRC := $(wildcard src/*.cpp)
SRC += $(wildcard src/*/*.cpp)
OBJ := $(SRC:.cpp=.o)


# COMPILE_SRC_DEPENDENCIES := $(patsubst src/%.cc,%.d,$(SRC))
# INTERPRETER_SRC_DEPENDENCIES := $(patsubst interpreter_src/%.cc,%.d,$(INTERPRETER_SRC))

# -include $(COMPILE_SRC_DEPENDENCIES)
# -include $(INTERPRETER_SRC_DEPENDENCIES)

GEN_UNMODIFIED_DB := temp.json
_clang 	:=  clang++ -glldb 	
_gpp 	:=  g++ 	-g 	
COMPILER := ${_clang}
#COMPILER := ${_gpp}
src_dir := src/


#This is the debug build
void_rush_debug.exe: $(OBJ) #Faster, Uses the compiled files
	$(COMPILER) -w $(OBJ) -ovoid_rush_debug.exe

#This is the release build
void_rush.exe: $(OBJ) #Faster, Uses the compiled files
	clang++ -w -std=c++17 $(OBJ) -ovoid_rush.exe	

run_void_rush: void_rush.exe
	void_rush.exe	

run_void_rush_debug: void_rush_debug.exe
	void_rush_debug.exe	

#https://clangd.llvm.org/guides/system-headers	
		
gen_cdb_noWarn: $(OBJ) 
	clang++ -MJ $(GEN_UNMODIFIED_DB) -std=c++17 $(SRC) -o_void_rush.o	
	.\.scripts\gen_compiler_database.bat

gen_cdb_someWarn: $(OBJ) 
	clang++ -MJ $(GEN_UNMODIFIED_DB) -Wall -std=c++17 $(SRC) -o_void_rush.o
	.\.scripts\gen_compiler_database.bat

gen_cdb_extraWarn: $(OBJ) 
	clang++ -MJ $(GEN_UNMODIFIED_DB) -Wall -Wextra -std=c++17 $(SRC)  -o_void_rush.o
	.\.scripts\gen_compiler_database.bat

gen_cdb_pedanticWarn: $(OBJ) 
	clang++ -MJ $(GEN_UNMODIFIED_DB) -Wall -Wextra -Wpedantic -std=c++17 $(SRC) -o_void_rush.o
	.\.scripts\gen_compiler_database.bat			

gen_cdb_everyWarn: $(OBJ) 
	clang++ -MJ $(GEN_UNMODIFIED_DB) -Weverything -std=c++17 $(SRC) -o_void_rush.o 	
	.\.scripts\gen_compiler_database.bat
#sed -e "1s/^/[\n/" -e "$s/,$/\n]/" -e "s/\\xE5/å/g" -e "s/\\xE4/ä/g" -e "s/\\xF6/ö/g" temp.json > compile_commands.json	

gen_cdb: $(OBJ) gen_cdb_extraWarn

fixplz: clean gen_cdb void_rush_debug.exe


clean:		
	del void_rush.exe
	del void_rush_debug.exe
	del _void_rush.o
	del *.ilk
	del *.pdb	
	del src\*.o
	del .cache\clangd\index\*.idx	
	

# %.o: %.cc Makefile
#         $(CXX) $(WARNING) $(CXXFLAGS) -MMD -MP -c $< -o $@