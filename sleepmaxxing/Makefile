CXX = g++-15
CXXFLAGS = -std=c++17 \
	-Irecovery_logic/include \
	-Iml_integration \
	-Iml_logger \
	$(shell pkg-config --cflags mlpack)

LDFLAGS  = $(shell pkg-config --libs mlpack)

CPP_SOURCES = \
	recovery_logic/src/main.cpp \
	recovery_logic/src/CSVreader.cpp \
	recovery_logic/src/robustStats.cpp \
	recovery_logic/src/windowStats.cpp \
	recovery_logic/src/recoveryTrend.cpp \
	recovery_logic/src/circadianShift.cpp \
	recovery_logic/src/combine.cpp \
	recovery_logic/src/miniexplanation.cpp \
	recovery_logic/src/explain.cpp \
	ml_integration/feature_extract.cpp \
	ml_integration/label_extract.cpp \
	ml_integration/dataset_prepper.cpp \
	ml_integration/model.cpp \
	ml_logger/logger.cpp \
	ml_integration/actual_outcome.cpp \
	ml_logger/has_seen.cpp \
	ml_logger/load_training_data.cpp \
	ml_integration/learning_stats.cpp \
	ml_integration/load_errors.cpp \
	ml_integration/error_stats.cpp \
	ml_logger/load_predictions.cpp

TARGET = sleepmaxxing
DATA = data/myhealth.csv 2025-01-21
.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(CPP_SOURCES)
	$(CXX) $(CXXFLAGS) $(CPP_SOURCES) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET) $(DATA)

clean:
	rm -f $(TARGET)
