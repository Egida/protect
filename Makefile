# Network Next Makefile

.PHONY: build
build: build/Makefile
	cd build && make -j

.PHONY: clean
clean: build/Makefile
	cd build && make clean
	rm -rf dist/*

build/Makefile: build/CMakeLists.txt
	cd build && cmake -DCMAKE_BUILD_TYPE=Release .
