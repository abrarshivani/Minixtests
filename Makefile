CC = clang

.PHONY: install-tests
install-tests: clean-tests
	$(CC) framework.c tests.c -o tests

.PHONY: clean-tests
clean-tests:
	rm -f proc_topic_test topic_test
