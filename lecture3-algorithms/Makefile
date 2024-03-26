TARGET = target
# need to install cs50 library(see: https://github.com/cs50/libcs50?tab=readme-ov-file#from-source-linux-and-mac)
# https://github.com/cs50/help50-deprecated/issues/24#issuecomment-422015970
.PHONY: run
run:
	clang $(ARG).c -o ./$(TARGET)/$(ARG) -lcs50 \
	&& ./$(TARGET)/$(ARG)

.PHONY: clean
clean:
	rm -fr ./$(TARGET)/* 
