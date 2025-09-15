# Functional Thinking Bank Demo Makefile
# Author: Abiola-DevSecAI
# 
# Cross-platform makefile for building the functional thinking banking demonstration
# Supports Windows (MinGW), macOS, and Linux

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./lib
LDFLAGS = -pthread

# Platform detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LDFLAGS += -ldl
endif
ifeq ($(UNAME_S),Darwin)
    LDFLAGS += -framework CoreFoundation -framework Security
endif

# Source files
VULNERABLE_SRC = src/vulnerable_bank.cpp
SECURE_SRC = src/secure_bank.cpp

# Binary names
VULNERABLE_BIN = vulnerable_bank
SECURE_BIN = secure_bank

# Default target
.PHONY: all vulnerable secure run clean help

all: vulnerable

# Build functional thinking version (optimised for demonstration)
vulnerable: $(VULNERABLE_BIN)

$(VULNERABLE_BIN): $(VULNERABLE_SRC)
	@echo "Building functional thinking banking application..."
	$(CXX) $(CXXFLAGS) -O2 $(VULNERABLE_SRC) -o $(VULNERABLE_BIN) $(LDFLAGS)
	@echo "✓ Functional thinking version built successfully"

# Build security thinking version (with proper security measures)
secure: $(SECURE_BIN)

$(SECURE_BIN): $(SECURE_SRC)
	@echo "Building security thinking banking application..."
	$(CXX) $(CXXFLAGS) -O2 -D_FORTIFY_SOURCE=2 -fstack-protector-strong $(SECURE_SRC) -o $(SECURE_BIN) $(LDFLAGS)
	@echo "✓ Security thinking version built successfully"

# Quick start - build and run functional thinking version
run: vulnerable
	@echo "Starting functional thinking banking demo on http://localhost:8080"
	@echo "Press Ctrl+C to stop the server"
	@echo ""
	./$(VULNERABLE_BIN)

# Run security thinking version
run-secure: secure
	@echo "Starting security thinking banking demo on http://localhost:8080"
	@echo "Press Ctrl+C to stop the server"
	@echo ""
	./$(SECURE_BIN)

# Clean up build artifacts
clean:
	@echo "Cleaning up build files..."
	-rm -f $(VULNERABLE_BIN) $(SECURE_BIN)
	@echo "✓ Clean complete"

# Display help information
help:
	@echo "Functional Thinking Bank Demo - Build System"
	@echo "============================================="
	@echo ""
	@echo "Available targets:"
	@echo "  vulnerable    - Build the functional thinking version (default)"
	@echo "  secure        - Build the security thinking version"
	@echo "  run           - Build and run functional thinking version"
	@echo "  run-secure    - Build and run security thinking version"
	@echo "  clean         - Remove build artifacts"
	@echo "  help          - Display this help"
	@echo ""
	@echo "Quick start:"
	@echo "  make run      - Build and start the demo"
	@echo ""
	@echo "For exploitation instructions, see exploits/instructions.md"