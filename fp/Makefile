#
#    Copyright (c) 2018 The Gupta Empire - All Rights Reserved
#    Please refer to LICENCE.md in the project root directory for
#    licence information. If no Licence file is provided, please
#    contact Aryan Gupta <me@theguptaempire.net> for more info
#
#===============================================================================
#    @author          Aryan Gupta <me@theguptaempire.net>
#    @title           Makefile
#    @brief           This is a generic Makefile I use for my C++ projects
#===============================================================================
.DEFAULT_GOAL := all
#==========================  CONFIG MACROS  ====================================
CC = /usr/bin/g++
GDB = /usr/bin/gdb
EXT = out
PRGM = main

# Directories
BIN = bin
SRC = src
DEP = dep

# Boost libs
#BOOST_INC = -I/usr/local/boost/include
#BOOST_LNK = -L/usr/local/boost/lib -lboost_atomic -lboost_graph -lboost_math_tr1l -lboost_stacktrace_noop \
             -lboost_chrono -lboost_iostreams -lboost_prg_exec_monitor -lboost_system -lboost_container \
			 -lboost_locale -lboost_program_options -lboost_test_exec_monitor -lboost_context -lboost_log \
			 -lboost_random -lboost_thread -lboost_contract -lboost_log_setup -lboost_regex -lboost_timer \
			 -lboost_coroutine -lboost_math_c99 -lboost_serialization -lboost_type_erasure -lboost_date_time \
			 -lboost_math_c99f -lboost_signals -lboost_unit_test_framework -lboost_exception -lboost_math_c99l \
			 -lboost_stacktrace_addr2line -lboost_wave -lboost_fiber -lboost_math_tr1 -lboost_stacktrace_backtrace \
			 -lboost_wserialization -lboost_filesystem -lboost_math_tr1f -lboost_stacktrace_basic

# OpenCV libs
OPENCV_INC = -I/usr/include/opencv4
OPENCV_LNK = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d \
			 -lopencv_calib3d -lopencv_objdetect -lopencv_flann -lopencv_imgcodecs -lopencv_videoio

#==============================  MACROS  ======================================

# Final executable
EXE = $(BIN)/$(PRGM).$(EXT)

# Source files and dependencies
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst %, $(BIN)/%.o, $(basename $(notdir $(SRCS))))
DEPS = $(patsubst %, $(DEP)/%.d, $(basename $(notdir $(SRCS))))

# External libs
ILIBS = $(BOOST_INC) $(OPENCV_INC)
LLIBS = $(BOOST_LNK) $(OPENCV_LNK) -lpthread

# Compile/Link flags
CFLAGS = $(DEBUG) $(ILIBS) -Wall -std=c++17 -c # -fmax-errors=1
LFLAGS = $(DEBUG) $(LLIBS) -Wall -lstdc++fs

# Dependency flags
DEPFLAGS = -MT $@ -MD -MP -MF $(patsubst %,$(DEP)/%.Td, $(basename $(notdir $<)))

#============================= PHONY RECEPIES =================================

# Build for debugging
.PHONY: debug
debug: DEBUG = -O0 -ggdb -g3
debug: $(EXE)

# Build for production
.PHONY: build
build: DEBUG = -O3 -DNDEBUG -s
build: $(EXE)

# Run program
.PHONY: run
run:
	@$(EXE)

# Clean and recompile for production
.PHONY: all
all: clean build

# Clean directories
.PHONY: clean
clean:
	-rm -f $(BIN)/*
	-rm -f $(DEP)/*

#============================ RECEPIES ========================================

# Object files
$(BIN)/%.o: $(SRC)/%.cpp
	@echo Building $@
	@$(CC) $(DEPFLAGS) $< $(CFLAGS) -o $@
	@mv -f $(DEP)/$*.Td $(DEP)/$*.d

# Link
$(EXE): $(OBJS)
	@echo Linking $@
	@$(CC) $^ -o $@ $(LFLAGS)

# Dependencies
.PRECIOUS = $(DEP)/%.d
$(DEP)/%.d: ;

# Include Dependencies
-include $(DEPS)