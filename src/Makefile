CFLAGS = -Wall -Wextra -Werror
INSTALL_FILES = brick_game/tetris/funcs.c gui/cli/gui.c s21_tetris.c -lncurses
TEST_FILES = tests/tests.c brick_game/tetris/funcs.c gui/cli/gui.c -lcheck -lncurses 
GCOV_FILES = tests/tests.c brick_game/tetris/funcs.c -lcheck -lncurses 
GCOVFLAGS = --coverage -lgcov
DIST_DIR = s21_Tetris

.PHONY: test

UNAME_S = $(shell uname)

ifeq ($(UNAME_S),Darwin)
OPEN_CMD = open
endif

ifeq ($(UNAME_S),Linux)
OPEN_CMD = xdg-open
ADD_LDFLAGS = -lm -lsubunit
endif

all: install

install:
	mkdir -p build
	gcc $(CFLAGS) $(INSTALL_FILES) $(ADD_LDFLAGS) -o build/Tetris
	echo 0 > build/score.txt

game: install
	./build/Tetris

uninstall:
	rm -rf build
	rm -f score.txt
test:
	echo 0 > score.txt
	gcc $(CFLAGS) $(TEST_FILES) $(ADD_LDFLAGS) -o s21_tests.out
	cd test && ./test
	./s21_tests.out || true

gcov_report: clean
	echo 0 > score.txt
	mkdir -p gcov
	gcc $(CFLAGS) $(GCOVFLAGS) $(GCOV_FILES) $(ADD_LDFLAGS) -o ./gcov/gcov_test
	./gcov/gcov_test || true
	gcovr --html-details gcov/gcov_report.html 2>/dev/null || true
	ln -s gcov/gcov_report.html ./gcov_report.html || true
	clean_gcovr >/dev/null

dvi:
	$(OPEN_CMD) Documentation.html

dist:
	mkdir -p $(DIST_DIR)
	cp -a brick_game $(DIST_DIR)
	cp -a gui $(DIST_DIR)
	cp -a test $(DIST_DIR)
	cp -a *.c $(DIST_DIR)
	cp -a *.h $(DIST_DIR)
	cp -a Documentation.html $(DIST_DIR)
	tar -czf s21_Tetris.tar.gz $(DIST_DIR)
	rm -rf $(DIST_DIR)
	
clang:
	cp ../materials/linters/.clang-format .
	clang-format -i *.c *.h */*.c */*.h */*/*.c */*/*.h
	rm .clang-format

check:
	cp ../../../materials/linters/.clang-format ../../brick_game/tetris/.clang-format
	clang-format -n *.c *.h
	clang-format -n ../../gui/cli/*.c ../../gui/cli/*.h
	clang-format -n ../../test/*.c ../../test/*.h
	rm -f .clang-format

leaks_game: install
	cd build && leaks -atExit -- ./Tetris

leaks_test: test
	cd ../../test && leaks -atExit -- ./test

clean:
	cd tests && rm -f test && rm -f *.txt
	rm -f *.txt && rm -f *.out
	rm -rf gcov and rm -f gcov_report.html
	rm -f s21_Tetris.tar.gz