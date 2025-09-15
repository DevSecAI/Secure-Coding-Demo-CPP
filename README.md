# Functional Thinking Bank Demo

**Educational Security Training: C++ Buffer Overflow & Integer Overflow Vulnerabilities**

*Author: Abiola-DevSecAI*

## Overview

This repository demonstrates how **functional thinking** (optimising for performance and simplicity) can create serious security vulnerabilities in real-world applications. The demo contrasts two identical banking applications:

- **Functional Thinking Version**: Fast, efficient code with security vulnerabilities
- **Security Thinking Version**: Same functionality with proper security controls

Based on the C++ Secure Coding Training document, this hands-on demonstration shows:
1. **Buffer Overflow** vulnerability (strcpy without bounds checking)
2. **Integer Overflow** vulnerability (unchecked numeric conversion)

## Quick Start (3 Commands)

```bash
git clone <repository-url>
cd vulnerable-bank-demo
make run
```

Then open http://localhost:8080 in your browser.

## What You'll Learn

### Core Concepts
- How **functional thinking** creates security gaps
- Real-world buffer overflow exploitation
- Integer overflow attacks in financial systems
- The difference between functional and security thinking approaches

### Practical Skills
- Building and running C++ applications with embedded web servers
- Identifying vulnerable code patterns
- Exploiting memory safety vulnerabilities
- Implementing security fixes without breaking functionality

## Prerequisites

### Required Software
- **C++ Compiler**: g++ with C++17 support
- **Web Browser**: Any modern browser
- **Terminal/Command Line**: Basic familiarity

### Platform Support
- **Linux**: Should work on Ubuntu 20.04+
- **macOS**: Should work on macOS 12+
- **Windows**: MinGW or WSL required

### Installation Verification
```bash
g++ --version  # Should show C++17 support
make --version # Should be available
```

## Project Structure

```
vulnerable-bank-demo/
├── README.md                              # This file
├── Makefile                               # Build system
├── src/
│   ├── vulnerable_bank.cpp               # Functional thinking implementation
│   └── secure_bank.cpp                   # Security thinking implementation
├── lib/
│   └── httplib.h                         # Embedded HTTP server library
└── exploits/
    ├── buffer_overflow_attack.html       # Buffer overflow exploitation tool
    ├── integer_overflow_attack.html      # Integer overflow exploitation tool
    └── instructions.md                   # Detailed exploitation guide
```

## Demo Walkthrough

### Phase 1: Normal Operations (5 minutes)

1. **Start the application:**
   ```bash
   make run
   ```

2. **Open your browser to:** http://localhost:8080

3. **Try legitimate operations:**
   - Update a customer profile with normal data
   - Transfer money between accounts (e.g., £100)
   - Observe everything works perfectly

### Phase 2: Exploit Buffer Overflow (10 minutes)

1. **Open the exploitation tool:**
   ```bash
   # In a new terminal or browser tab
   open exploits/buffer_overflow_attack.html
   # or visit file:///path/to/exploits/buffer_overflow_attack.html
   ```

2. **Generate malicious payload:**
   - Select "Name Buffer - Large Overflow (100 chars)"
   - Copy the generated payload

3. **Execute the attack:**
   - Return to the banking application
   - Select any account for profile update
   - Paste the malicious payload into the "Full Name" field
   - Submit the form

4. **Observe the results:**
   - Application crash (segmentation fault)
   - Memory corruption
   - Buffer overflow exploitation successful

### Phase 3: Exploit Integer Overflow (10 minutes)

1. **Restart the application** (if it crashed):
   ```bash
   make run
   ```

2. **Open the integer overflow tool:**
   ```bash
   open exploits/integer_overflow_attack.html
   ```

3. **Execute negative number attack:**
   - Generate a "Negative Transfer - Large Amount (-£10,000)" payload
   - Copy the attack amount
   - Go to the banking application's transfer section
   - Select accounts: From ACC002 (Sarah - £750) To ACC001 (James)
   - Paste the negative amount (-10000)
   - Submit the transfer

4. **Observe the results:**
   - Transfer succeeds despite negative amount
   - Money is stolen from the target account
   - Balance validation bypassed

### Phase 4: Security Fixes Demonstration (5 minutes)

1. **Build and run the secure version:**
   ```bash
   make run-secure
   ```

2. **Attempt the same exploits:**
   - Try the buffer overflow payload → **Blocked with validation error**
   - Try the negative transfer → **Rejected with security logging**
   - All attacks now fail safely

3. **Observe security features:**
   - Input validation messages
   - Security event logging in terminal
   - Identical functionality but protected

## Build Commands

| Command | Description |
|---------|-------------|
| `make` | Build functional thinking version (default) |
| `make vulnerable` | Build functional thinking version |
| `make secure` | Build security thinking version |
| `make run` | Build and run functional thinking version |
| `make run-secure` | Build and run security thinking version |
| `make clean` | Remove build files |
| `make help` | Show available commands |

## Troubleshooting

### Port 8080 Already in Use
```bash
# Find what's using port 8080
lsof -i :8080  # macOS/Linux
netstat -ano | findstr :8080  # Windows

# Kill the process or use a different port
# Edit the source files to change port if needed
```

### Compilation Errors
```bash
# Check C++ compiler version
g++ --version

# Ensure C++17 support
g++ -std=c++17 --version

# Manual compilation (if make fails)
g++ -std=c++17 -I./lib src/vulnerable_bank.cpp -o vulnerable_bank -pthread
```

### Application Crashes Immediately
```bash
# Check for missing dependencies
ldd vulnerable_bank  # Linux
otool -L vulnerable_bank  # macOS

# Run with debugging
gdb ./vulnerable_bank  # Linux
lldb ./vulnerable_bank  # macOS
```

### Browser Cannot Connect
- Ensure the application started successfully (check terminal output)
- Try http://127.0.0.1:8080 instead of localhost
- Check firewall settings
- Ensure no other service is using port 8080

## Vulnerability Details

### Buffer Overflow (CWE-120)

**Location:** `src/vulnerable_bank.cpp:handleProfileUpdate()`

**Vulnerable Code:**
```cpp
// Functional thinking: Fast string copying for performance
strcpy(customer.full_name, new_name.c_str());    // 32-byte buffer
strcpy(customer.address, new_address.c_str());   // 64-byte buffer
```

**Exploitation:** Input longer than buffer size overwrites adjacent memory

**Fix Applied in Secure Version:**
```cpp
// Security thinking: Bounds-checked copying with validation
strncpy(customer.full_name, new_name.c_str(), sizeof(customer.full_name) - 1);
customer.full_name[sizeof(customer.full_name) - 1] = '\0';
```

### Integer Overflow (CWE-190)

**Location:** `src/vulnerable_bank.cpp:handleTransfer()`

**Vulnerable Code:**
```cpp
// Functional thinking: Fast conversion - trust the input format
long long amount_pence = std::stoll(amount_str) * 100;

// Simple validation - assume positive amounts  
if (amount_pence > 0 && from_customer.balance_pence >= amount_pence) {
```

**Exploitation:** Negative numbers bypass validation, large numbers cause overflow

**Fix Applied in Secure Version:**
```cpp
// Security thinking: Comprehensive input validation
std::optional<long long> parseTransactionAmount(const std::string& amount_str) {
    // Validate format, range, and business logic constraints
    // Handle edge cases and potential overflow conditions
}
```

## Learning Objectives

After completing this demonstration, participants will understand:

1. **Root Cause Analysis**: How performance optimisations create security vulnerabilities
2. **Exploitation Techniques**: Practical buffer overflow and integer overflow attacks
3. **Secure Coding Practices**: Input validation, bounds checking, and defensive programming
4. **Security vs Performance Trade-offs**: When and how to prioritise security

## Additional Resources

### Related Training Materials
- C++ Secure Coding Training Document (source material)
- OWASP C++ Security Guidelines
- CERT SEI C++ Coding Standards

### Real-World Examples
- **CitrixBleed CVE-2023-4966**: Buffer over-read vulnerability
- **Heartbleed**: Buffer overflow in OpenSSL
- **Integer overflow attacks** in cryptocurrency applications

### Tools for Further Learning
- **Static Analysis**: Clang Static Analyzer, PVS-Studio
- **Dynamic Analysis**: Valgrind, AddressSanitizer
- **Fuzzing**: libFuzzer, AFL++

## Contributing

This is an educational project. If you find issues or have suggestions:

1. **Bug Reports**: Include OS, compiler version, and error messages
2. **Enhancement Requests**: Focus on educational value
3. **Pull Requests**: Maintain the learning objectives and British localisation

## Legal & Ethical Use

**Important:** This software is for educational purposes only.

- ✅ **Authorised Use**: Security training, education, authorised penetration testing
- ❌ **Prohibited Use**: Attacking systems without permission, malicious activities

Users are responsible for compliance with applicable laws and regulations.


---

*For questions about this educational demonstration, refer to the documentation or contact the training organiser.*