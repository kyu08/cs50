.PHONY: run
run: clean recover
	./recover card.raw

clean:
	rm -f ./recover \
	&& rm -f ./*.jpg

.PHONY: lint
lint:
	cppcheck --quiet .
