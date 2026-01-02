CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
INCLUDES = -Irecovery_logic/include

CPP_SOURCES = \
	recovery_logic/src/main.cpp \
	recovery_logic/src/CSVreader.cpp \
	recovery_logic/src/robustStats.cpp \
	recovery_logic/src/windowStats.cpp \
	recovery_logic/src/recoveryTrend.cpp \
	recovery_logic/src/circadianShift.cpp \
	recovery_logic/src/combine.cpp \
	recovery_logic/src/miniexplanation.cpp

TARGET = sleepmaxxing

DATA = data/sample_garmin_days.csv

.PHONY: all cpp run plots clean

all: cpp run plots

cpp: $(TARGET)

$(TARGET): $(CPP_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(CPP_SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) $(DATA)

plots:
	python3 viz/graphs.py $(DATA)

clean:
	rm -f $(TARGET)
