CC = clang

.PHONY: install-tests
install-tests: clean-tests
	$(CC) framework.c tests.c -o tests
	$(CC) framework.c  itests.c -o itests

.PHONY: clean-tests
clean-tests:
	rm -f tests itests
