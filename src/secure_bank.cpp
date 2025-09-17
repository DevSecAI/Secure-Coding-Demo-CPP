/*
 * Security Thinking Bank Demo - Secure Implementation
 * Author: Abiola-DevSecAI
 * 
 * This application demonstrates security thinking - implementing the same functionality
 * as the vulnerable version but with proper security controls and input validation.
 * 
 * Fixes applied:
 * 1. Buffer overflow protection in profile update (strncpy + bounds checking)
 * 2. Integer overflow protection in money transfer (input validation)
 * 3. Security logging and comprehensive error handling
 */

#include "../lib/httplib.h"
#include <iostream>
#include <string>
#include <map>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <optional>
#include <algorithm>
#include <cctype>
#include <chrono>

// Security logging functionality
void logSecurityEvent(const std::string& event, const std::string& details) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::cout << "[SECURITY] " << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
              << " - " << event << ": " << details << std::endl;
}

// Secure customer data structure with validation
struct Customer {
    char account_number[20];
    char full_name[32];
    char address[64];
    long long balance_pence;
    char phone[16];
    
    Customer(const char* acc, const char* name, const char* addr, long long bal, const char* ph) {
        // Security thinking: bounds-checked string copying
        strncpy(account_number, acc, sizeof(account_number) - 1);
        strncpy(full_name, name, sizeof(full_name) - 1);
        strncpy(address, addr, sizeof(address) - 1);
        strncpy(phone, ph, sizeof(phone) - 1);
        
        // Ensure null termination
        account_number[sizeof(account_number) - 1] = '\0';
        full_name[sizeof(full_name) - 1] = '\0';
        address[sizeof(address) - 1] = '\0';
        phone[sizeof(phone) - 1] = '\0';
        
        balance_pence = bal;
    }
};

// Global customer database - same structure for compatibility
std::map<std::string, Customer> customers;

// Initialize with identical British customer data
void initializeBankData() {
    customers.emplace("ACC001", Customer("ACC001", "James William Smith", "45 Victoria Street, London SW1H 0EU", 250000, "020-7946-0958"));
    customers.emplace("ACC002", Customer("ACC002", "Sarah Elizabeth Jones", "12 King's Road, Brighton BN1 2HM", 75000, "01273-696-123"));
    customers.emplace("ACC003", Customer("ACC003", "Michael David Thompson", "8 Castle Street, Edinburgh EH1 2DP", 500000, "0131-225-4567"));
    customers.emplace("ACC004", Customer("ACC004", "Emma Charlotte Wilson", "23 High Street, Manchester M1 1AA", 125000, "0161-234-5678"));
}

// Secure input validation for strings
bool validateStringInput(const std::string& input, size_t max_length, const std::string& field_name) {
    if (input.empty()) {
        logSecurityEvent("Input validation failed", field_name + " cannot be empty");
        return false;
    }
    
    if (input.length() >= max_length) {
        logSecurityEvent("Input validation failed", field_name + " exceeds maximum length (" + std::to_string(max_length) + ")");
        return false;
    }
    
    // Additional validation: check for potentially malicious characters
    for (char c : input) {
        if (c < 32 || c > 126) { // Non-printable ASCII characters
            logSecurityEvent("Input validation failed", field_name + " contains invalid characters");
            return false;
        }
    }
    
    return true;
}

// Secure amount parsing with comprehensive validation
std::optional<long long> parseTransactionAmount(const std::string& amount_str) {
    // Security thinking: validate input format first
    if (amount_str.empty() || amount_str.length() > 15) {
        logSecurityEvent("Transaction validation failed", "Invalid amount format: " + amount_str);
        return std::nullopt;
    }
    
    // Validate numeric format
    bool has_decimal = false;
    size_t decimal_pos = 0;
    
    for (size_t i = 0; i < amount_str.length(); ++i) {
        char c = amount_str[i];
        if (c == '.') {
            if (has_decimal) {
                logSecurityEvent("Transaction validation failed", "Multiple decimal points: " + amount_str);
                return std::nullopt;
            }
            has_decimal = true;
            decimal_pos = i;
        } else if (!std::isdigit(c)) {
            logSecurityEvent("Transaction validation failed", "Non-numeric characters: " + amount_str);
            return std::nullopt;
        }
    }
    
    // Check decimal places (max 2 for pence)
    if (has_decimal && (amount_str.length() - decimal_pos - 1) > 2) {
        logSecurityEvent("Transaction validation failed", "Too many decimal places: " + amount_str);
        return std::nullopt;
    }
    
    try {
        // Parse as double first to handle decimals safely
        double amount_pounds = std::stod(amount_str);
        
        // Security constraints
        constexpr double MIN_TRANSACTION = 0.01;     // 1 pence minimum
        constexpr double MAX_TRANSACTION = 1000000.0; // £1M maximum
        
        if (amount_pounds < MIN_TRANSACTION || amount_pounds > MAX_TRANSACTION) {
            logSecurityEvent("Transaction validation failed", "Amount outside valid range: " + amount_str);
            return std::nullopt;
        }
        
        // Convert to pence with overflow protection
        long long amount_pence = static_cast<long long>(amount_pounds * 100 + 0.5); // +0.5 for rounding
        
        // Final validation: ensure we haven't overflowed
        if (amount_pence <= 0 || amount_pence > 100000000) { // £1M in pence
            logSecurityEvent("Transaction validation failed", "Amount conversion overflow: " + amount_str);
            return std::nullopt;
        }
        
        return amount_pence;
        
    } catch (const std::exception& e) {
        logSecurityEvent("Transaction validation failed", "Parsing exception: " + std::string(e.what()));
        return std::nullopt;
    }
}

// Utility function - same as vulnerable version for compatibility
std::string formatCurrency(long long pence) {
    double pounds = pence / 100.0;
    std::stringstream ss;
    ss << "£" << std::fixed << std::setprecision(2) << pounds;
    return ss.str();
}

// Generate dynamic customer list HTML showing current data state
std::string generateCustomerList() {
    std::stringstream html;
    html << "<div class=\"customer-list\">\n";
    html << "            <h3>Current Customer Balances <small>(Live Data - Updates in Real Time)</small></h3>\n";
    
    for (const auto& [account_id, customer] : customers) {
        std::string display_name(customer.full_name);
        std::string display_address(customer.address);
        
        html << "            <div class=\"customer-item\">\n";
        html << "                <strong>" << account_id << " - " << display_name << "</strong><br>\n";
        html << "                " << display_address << "<br>\n";
        html << "                Balance: <span style=\"color: #28a745; font-weight: bold;\">" 
             << formatCurrency(customer.balance_pence) << "</span>\n";
        html << "            </div>\n";
    }
    
    html << "        </div>";
    return html.str();
}

// Main banking page HTML - with dynamic customer data
std::string getBankingPage() {
    std::string html = R"(<!DOCTYPE html>
<html lang="en-GB">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Security Thinking Bank - Online Banking</title>
    <style>
        body { 
            font-family: 'Segoe UI', Arial, sans-serif; 
            margin: 0; 
            padding: 20px; 
            background: linear-gradient(135deg, #28a745 0%, #20c997 100%);
            min-height: 100vh;
        }
        .container { 
            max-width: 1200px; 
            margin: 0 auto; 
            background: white; 
            border-radius: 12px; 
            padding: 30px; 
            box-shadow: 0 15px 35px rgba(0,0,0,0.1);
        }
        .header { 
            text-align: center; 
            margin-bottom: 40px; 
            padding-bottom: 20px; 
            border-bottom: 3px solid #28a745;
        }
        .header h1 { 
            color: #333; 
            margin: 0; 
            font-size: 2.5em;
        }
        .subtitle { 
            color: #666; 
            font-style: italic; 
            margin-top: 10px;
        }
        .features {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 30px;
            margin-bottom: 40px;
        }
        .feature-card {
            background: #f8fff8;
            padding: 25px;
            border-radius: 8px;
            border-left: 5px solid #28a745;
        }
        .feature-card h3 {
            color: #333;
            margin-top: 0;
        }
        .form-group {
            margin-bottom: 15px;
        }
        label {
            display: block;
            margin-bottom: 5px;
            font-weight: 600;
            color: #555;
        }
        input, select {
            width: 100%;
            padding: 12px;
            border: 2px solid #ddd;
            border-radius: 6px;
            font-size: 16px;
            transition: border-color 0.3s;
        }
        input:focus, select:focus {
            outline: none;
            border-color: #28a745;
        }
        .btn {
            background: #28a745;
            color: white;
            padding: 12px 30px;
            border: none;
            border-radius: 6px;
            cursor: pointer;
            font-size: 16px;
            font-weight: 600;
            transition: background 0.3s;
        }
        .btn:hover {
            background: #218838;
        }
        .customer-list {
            background: #f0f8f0;
            padding: 20px;
            border-radius: 8px;
            margin-top: 30px;
        }
        .customer-item {
            background: white;
            margin: 10px 0;
            padding: 15px;
            border-radius: 6px;
            border-left: 4px solid #28a745;
        }
        .security-note {
            background: #d4edda;
            border: 1px solid #c3e6cb;
            border-radius: 6px;
            padding: 15px;
            margin-top: 30px;
            color: #155724;
        }
        .security-note strong {
            color: #0f4419;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>Security Thinking Bank</h1>
            <p class="subtitle">Secure by Design &amp; Validated by Default</p>
        </div>

        <div class="features">
            <div class="feature-card">
                <h3>📝 Update Customer Profile</h3>
                <p>Secure profile updates with bounds checking and validation</p>
                <form action="/update-profile" method="POST">
                    <div class="form-group">
                        <label for="account">Account Number:</label>
                        <select name="account" id="account" required>
                            <option value="">Select Account</option>
                            <option value="ACC001">ACC001 - James Smith</option>
                            <option value="ACC002">ACC002 - Sarah Jones</option>
                            <option value="ACC003">ACC003 - Michael Thompson</option>
                            <option value="ACC004">ACC004 - Emma Wilson</option>
                        </select>
                    </div>
                    <div class="form-group">
                        <label for="new_name">New Full Name (max 31 chars):</label>
                        <input type="text" name="new_name" id="new_name" maxlength="31"
                               placeholder="Enter new full name" required>
                    </div>
                    <div class="form-group">
                        <label for="new_address">New Address (max 63 chars):</label>
                        <input type="text" name="new_address" id="new_address" maxlength="63"
                               placeholder="Enter new address" required>
                    </div>
                    <button type="submit" class="btn">Update Profile</button>
                </form>
            </div>

            <div class="feature-card">
                <h3>💰 Transfer Money</h3>
                <p>Secure transaction processing with overflow protection</p>
                <form action="/transfer" method="POST">
                    <div class="form-group">
                        <label for="from_account">From Account:</label>
                        <select name="from_account" id="from_account" required>
                            <option value="">Select From Account</option>
                            <option value="ACC001">ACC001 - James Smith</option>
                            <option value="ACC002">ACC002 - Sarah Jones</option>
                            <option value="ACC003">ACC003 - Michael Thompson</option>
                            <option value="ACC004">ACC004 - Emma Wilson</option>
                        </select>
                    </div>
                    <div class="form-group">
                        <label for="to_account">To Account:</label>
                        <select name="to_account" id="to_account" required>
                            <option value="">Select To Account</option>
                            <option value="ACC001">ACC001 - James Smith</option>
                            <option value="ACC002">ACC002 - Sarah Jones</option>
                            <option value="ACC003">ACC003 - Michael Thompson</option>
                            <option value="ACC004">ACC004 - Emma Wilson</option>
                        </select>
                    </div>
                    <div class="form-group">
                        <label for="amount">Amount:</label>
                        <input type="text" name="amount" id="amount" 
                               placeholder="Enter amount in pounds" required>
                    </div>
                    <button type="submit" class="btn">Transfer Money</button>
                </form>
            </div>
        </div>

)";
    // Insert dynamic customer list here
    html += generateCustomerList();
    html += R"(

        <div class="security-note">
            <strong>Security Features Active:</strong>
            This application demonstrates security thinking - implementing comprehensive input validation,
            bounds checking, and security logging. All operations are protected against common vulnerabilities
            whilst maintaining identical functionality to the vulnerable version.
        </div>
    </div>
</body>
</html>)";
    
    return html;
}

// Secure profile update handler - with bounds checking
void handleProfileUpdate(const httplib::Request& req, httplib::Response& res) {
    auto account = req.get_param_value("account");
    auto new_name = req.get_param_value("new_name");
    auto new_address = req.get_param_value("new_address");
    
    // Security thinking: validate all inputs before processing
    if (!validateStringInput(new_name, sizeof(Customer::full_name), "full_name") ||
        !validateStringInput(new_address, sizeof(Customer::address), "address")) {
        res.status = 400;
        res.set_content("Input validation failed! Check field lengths and characters. <a href='/'>Return to Banking</a>", "text/html");
        return;
    }
    
    auto customer_it = customers.find(account);
    if (customer_it != customers.end()) {
        Customer& customer = customer_it->second;
        
        // Security thinking: bounds-checked string copying with explicit null termination
        strncpy(customer.full_name, new_name.c_str(), sizeof(customer.full_name) - 1);
        strncpy(customer.address, new_address.c_str(), sizeof(customer.address) - 1);
        customer.full_name[sizeof(customer.full_name) - 1] = '\0';
        customer.address[sizeof(customer.address) - 1] = '\0';
        
        std::cout << "[INFO] Profile updated successfully for account: " << account << std::endl;
        res.set_content("Profile updated successfully with security validation! <a href='/'>Return to Banking</a>", "text/html");
    } else {
        logSecurityEvent("Profile update failed", "Account not found: " + account);
        res.status = 400;
        res.set_content("Account not found! <a href='/'>Return to Banking</a>", "text/html");
    }
}

// Secure money transfer handler - with overflow protection
void handleTransfer(const httplib::Request& req, httplib::Response& res) {
    auto from_account = req.get_param_value("from_account");
    auto to_account = req.get_param_value("to_account");
    auto amount_str = req.get_param_value("amount");
    
    // Security thinking: comprehensive input validation before processing
    auto amount_opt = parseTransactionAmount(amount_str);
    if (!amount_opt.has_value()) {
        res.status = 400;
        res.set_content("Invalid transaction amount! Check format and range (£0.01 - £1,000,000). <a href='/'>Return to Banking</a>", "text/html");
        return;
    }
    
    long long amount_pence = amount_opt.value();
    
    auto from_it = customers.find(from_account);
    auto to_it = customers.find(to_account);
    
    if (from_it != customers.end() && to_it != customers.end()) {
        Customer& from_customer = from_it->second;
        Customer& to_customer = to_it->second;
        
        // Security validation: explicit positive amount and sufficient balance checks
        if (from_customer.balance_pence >= amount_pence) {
            // Additional security check: prevent self-transfers
            if (from_account == to_account) {
                logSecurityEvent("Transfer blocked", "Attempted self-transfer: " + from_account);
                res.status = 400;
                res.set_content("Cannot transfer to the same account! <a href='/'>Return to Banking</a>", "text/html");
                return;
            }
            
            // Perform secure transfer with logging
            from_customer.balance_pence -= amount_pence;
            to_customer.balance_pence += amount_pence;
            
            std::cout << "[INFO] Secure transfer completed: " << formatCurrency(amount_pence) 
                      << " from " << from_account << " to " << to_account << std::endl;
            
            std::string message = "Transfer successful with security validation!<br>";
            message += "Transferred " + formatCurrency(amount_pence);
            message += " from " + from_account + " to " + to_account;
            message += "<br><a href='/'>Return to Banking</a>";
            
            res.set_content(message, "text/html");
        } else {
            logSecurityEvent("Transfer rejected", "Insufficient funds: " + from_account + " attempted " + formatCurrency(amount_pence));
            res.status = 400;
            res.set_content("Insufficient funds! <a href='/'>Return to Banking</a>", "text/html");
        }
    } else {
        logSecurityEvent("Transfer failed", "Invalid accounts: " + from_account + " -> " + to_account);
        res.status = 400;
        res.set_content("One or both accounts not found! <a href='/'>Return to Banking</a>", "text/html");
    }
}

int main() {
    // Initialize customer data (identical to vulnerable version)
    initializeBankData();
    
    // Create HTTP server
    httplib::Server server;
    
    // Serve main banking page
    server.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(getBankingPage(), "text/html; charset=utf-8");
    });
    
    // Handle secure profile updates
    server.Post("/update-profile", handleProfileUpdate);
    
    // Handle secure money transfers
    server.Post("/transfer", handleTransfer);
    
    // Start server
    std::cout << "🔒 Security Thinking Bank Demo Starting..." << std::endl;
    std::cout << "Server running on: http://localhost:8080" << std::endl;
    std::cout << "Security features: Input validation + Buffer overflow protection + Integer overflow protection" << std::endl;
    std::cout << "Built with security thinking (validation over performance)" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    std::cout << std::endl;
    
    if (!server.listen("127.0.0.1", 8080)) {
        std::cerr << "Failed to start server on port 8080" << std::endl;
        std::cerr << "Make sure port 8080 is not already in use" << std::endl;
        return 1;
    }
    
    return 0;
}