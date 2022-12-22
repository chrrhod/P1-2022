# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CC = g++
CFLAGS = -Wall -g -fsanitize=address

# ****************************************************
# Targets needed to bring the executable up to date

all: instruct convert csvMap

instruct: instructionGenerator.cpp
	$(CC) $(CFLAGS) -o instruct instructionGenerator.cpp

convert: mapConverter.cpp
	$(CC) $(CFLAGS) -o convert mapConverter.cpp

csvMap: generateCsvMap.cpp
	$(CC) $(CFLAGS) -o csvMap generateCsvMap.cpp


#$(CFLAGS)
