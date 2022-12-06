GPP=g++
PROJ_FILES_PATH=.
TESTS_DIR=./WorldCupTests
O_FILES_DIR=$(TESTS_DIR)/OFiles
EXEC=WorldCupUnitTester
TESTS_INCLUDED_FILE=worldcup23a1.h $(TESTS_DIR)/catch.hpp
OBJS=$(O_FILES_DIR)/UnitTests.o $(O_FILES_DIR)/team.o $(O_FILES_DIR)/player.o $(O_FILES_DIR)/worldcup23a1.o # UPDATE HERE ALL THE O FILES YOU CREATED BELOW
DEBUG_FLAG= -g # can add -g
COMP_FLAG=--std=c++11 -Wall -Werror -pedantic-errors $(DEBUG_FLAG)

$(EXEC) : $(OBJS)
	$(GPP) $(COMP_FLAG) $(OBJS) -o $@

$(O_FILES_DIR)/UnitTests.o : $(TESTS_DIR)/WorldCupTests.cpp
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) $(TESTS_DIR)/WorldCupTests.cpp -o $@

 # UPDATE FROM HERE

$(O_FILES_DIR)/AVLTree.o : AVLTree.cpp AVLTree.h
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) AVLTree.cpp -o $@

$(O_FILES_DIR)/team.o : team.cpp team.h player.h
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) team.cpp -o $@

$(O_FILES_DIR)/player.o : player.cpp player.h AVLTree.h team.h
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) player.cpp -o $@

$(O_FILES_DIR)/worldcup23a1.o : worldcup23a1.cpp worldcup23a1.h wet1util.h AVLTree.h team.h player.h
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) worldcup23a1.cpp -o $@
	
 # UNTIL HERE
	

.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC)
