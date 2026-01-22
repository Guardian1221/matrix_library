CC = gcc

CFLAGS_LIB  = -std=c11 -Wall -Wextra -Werror -g
CFLAGS_TEST = $(CFLAGS_LIB) -fprofile-arcs -ftest-coverage

LDFLAGS_TEST = -lcheck -lsubunit -lm

LIB_NAME = matrix.a
TEST_BIN = test

all: $(LIB_NAME) test

$(LIB_NAME): matrix.o
	ar rcs $@ $<

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS_LIB) -c $< -o $@

test: tests.o $(LIB_NAME)
	$(CC) $(CFLAGS_TEST) tests.o -o $(TEST_BIN) $(LDFLAGS_TEST) $(LIB_NAME)

tests.o: tests.c matrix.h
	$(CC) $(CFLAGS_TEST) -c $< -o $@

run_test: test
	./$(TEST_BIN)

gcov_report: run_test
	@mkdir -p gcov_report
	gcovr --html --html-details -r . -o gcov_report/report.html
	@echo "Coverage report generated: gcov_report/report.html"

clean:
	rm -f *.o $(LIB_NAME) $(TEST_BIN)
	rm -f *.gcda *.gcno *.gcov
	rm -rf gcov_report

.PHONY: all test run_test gcov_report clean
