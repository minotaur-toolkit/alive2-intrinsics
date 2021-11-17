CC=clang++
WARN=-Wall -Werror 
CFLAGS=-I -march=native -mavx512f -mavx512bw -mavx512vl -mmmx -O3 -std=c++20
SFLAGS=-Wno-unused-command-line-argument -Wno-override-module #can't figure out how to handle the warning
DEPS=randomizer.hpp vectorutil.hpp x86Intrin.hpp
TARGETLLFILE=sse2_pavg_w-2inputs


all: driver

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) $(WARN) -c -o $@ $< 

%.S: %.ll
	$(CC) $(CFLAGS) $(SFLAGS) $(WARN) $< -S -o $@

%.o: %.S
	$(CC) $(CFLAGS) $(WARN) $< -c -o $@

driver: driver.o randomizer.o x86Intrin.o testfiles/$(TARGETLLFILE).src.o
	$(CC) $(CFLAGS) $(WARN) x86Intrin.o randomizer.o testfiles/$(TARGETLLFILE).src.o driver.o -o rundriver.out
