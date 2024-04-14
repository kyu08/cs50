.PHONY: run
run:
	gcc inheritance.c -o inheritance && ./inheritance

.PHONY: lint
lint:
	cppcheck --quiet .

.PHONY: clean
clean:
	rm -fr ./$(TARGET)/* 
