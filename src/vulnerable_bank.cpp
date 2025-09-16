/*
 * Functional Thinking Bank Demo - Vulnerable Implementation
 * Author: Abiola-DevSecAI
 * 
 * This application demonstrates how functional thinking (optimising for performance
 * and simplicity) can create security vulnerabilities in real-world applications.
 * 
 * Contains two vulnerabilities from the C++ secure coding training:
 * 1. Buffer overflow in profile update (strcpy)
 * 2. Integer overflow in money transfer (unchecked stoll)
 */

#include "../lib/httplib.h"
#include <iostream>
#include <string>
#include <map>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <cstdint>

// Customer data structure - optimised for simplicity
struct Customer {
    char account_number[20];
    char full_name[32];     // Buffer size chosen for typical UK names
    uint32_t name_canary = 0xDEADBEEF;  // Canary to detect buffer overflow
    char address[64];       // Optimised for UK addresses
    uint32_t address_canary = 0xCAFEBABE;  // Canary to detect buffer overflow
    long long balance_pence; // Store in pence for precision
    char phone[16];
    uint32_t final_canary = 0xFEEDFACE;  // Final canary
    
    Customer(const char* acc, const char* name, const char* addr, long long bal, const char* ph) {
        // Functional thinking: quick string copying for performance
        strcpy(account_number, acc);
        strcpy(full_name, name);
        strcpy(address, addr);
        strcpy(phone, ph);
        balance_pence = bal;
        // Canaries are set by default initialization above
    }
    
    // Check if canaries are intact
    bool canariesIntact() const {
        return (name_canary == 0xDEADBEEF && 
                address_canary == 0xCAFEBABE && 
                final_canary == 0xFEEDFACE);
    }
    
    std::string getCorruptionStatus() const {
        if (name_canary != 0xDEADBEEF) return "NAME_BUFFER_OVERFLOW";
        if (address_canary != 0xCAFEBABE) return "ADDRESS_BUFFER_OVERFLOW";
        if (final_canary != 0xFEEDFACE) return "CRITICAL_MEMORY_CORRUPTION";
        return "INTACT";
    }
};

// Global customer database - simple and efficient
std::map<std::string, Customer> customers;

// Initialize with British customer data
void initializeBankData() {
    customers.emplace("ACC001", Customer("ACC001", "James William Smith", "45 Victoria Street, London SW1H 0EU", 250000, "020-7946-0958"));
    customers.emplace("ACC002", Customer("ACC002", "Sarah Elizabeth Jones", "12 King's Road, Brighton BN1 2HM", 75000, "01273-696-123"));
    customers.emplace("ACC003", Customer("ACC003", "Michael David Thompson", "8 Castle Street, Edinburgh EH1 2DP", 500000, "0131-225-4567"));
    customers.emplace("ACC004", Customer("ACC004", "Emma Charlotte Wilson", "23 High Street, Manchester M1 1AA", 125000, "0161-234-5678"));
}

// Utility function to format currency in British pounds
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
        // Check canary status for immediate corruption detection
        bool canaries_intact = customer.canariesIntact();
        std::string corruption_status = customer.getCorruptionStatus();
        
        // Check if data looks corrupted (non-printable characters or very long)
        bool name_corrupted = false;
        bool address_corrupted = false;
        
        std::string display_name(customer.full_name);
        std::string display_address(customer.address);
        
        // Check for corruption indicators
        if (strlen(customer.full_name) > 100 || strlen(customer.address) > 150 || !canaries_intact) {
            name_corrupted = true;
            address_corrupted = true;
        }
        
        // Truncate if too long and mark as corrupted
        if (display_name.length() > 50) {
            display_name = display_name.substr(0, 50) + "... [CORRUPTED DATA]";
            name_corrupted = true;
        }
        if (display_address.length() > 80) {
            display_address = display_address.substr(0, 80) + "... [CORRUPTED DATA]";  
            address_corrupted = true;
        }
        
        std::string corruption_style = "";
        if (name_corrupted || address_corrupted || !canaries_intact) {
            corruption_style = " style=\"background: #ffe6e6; border-left-color: #ff4444;\"";
        }
        
        html << "            <div class=\"customer-item\"" << corruption_style << ">\n";
        html << "                <strong>" << account_id << " - " << display_name << "</strong>";
        
        if (!canaries_intact) {
            html << " <span style=\"color: red; font-weight: bold;\">[" << corruption_status << "]</span>";
        } else if (name_corrupted) {
            html << " <span style=\"color: red; font-weight: bold;\">[BUFFER OVERFLOW DETECTED]</span>";
        }
        
        html << "<br>\n";
        html << "                " << display_address;
        
        if (address_corrupted) {
            html << " <span style=\"color: red; font-weight: bold;\">[MEMORY CORRUPTION]</span>";
        }
        
        html << "<br>\n";
        html << "                Balance: <span style=\"color: #28a745; font-weight: bold;\">" 
             << formatCurrency(customer.balance_pence) << "</span>\n";
        html << "            </div>\n";
    }
    
    html << "        </div>";
    return html.str();
}

// Main banking page HTML - embedded for performance
std::string getBankingPage() {
    std::string html = R"(<!DOCTYPE html>
<html lang="en-GB">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Functional Thinking Bank - Online Banking</title>
    <style>
        body { 
            font-family: 'Segoe UI', Arial, sans-serif; 
            margin: 0; 
            padding: 20px; 
            background: linear-gradient(135deg, #0d4f0f 0%, #1a5a1a 100%);
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
            border-bottom: 3px solid #0d4f0f;
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
            background: #f8f9ff;
            padding: 25px;
            border-radius: 8px;
            border-left: 5px solid #0d4f0f;
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
            border-color: #0d4f0f;
        }
        .btn {
            background: #0d4f0f;
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
            background: #0a3f0a;
        }
        .customer-list {
            background: #f0f4f8;
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
        .vulnerability-note {
            background: #fff3cd;
            border: 1px solid #ffeaa7;
            border-radius: 6px;
            padding: 15px;
            margin-top: 30px;
            color: #856404;
        }
        .vulnerability-note strong {
            color: #721c24;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>Functional Thinking Bank</h1>
            <p class="subtitle">Optimised for Performance &amp; Simplicity</p>
        </div>

        <div class="features">
            <div class="feature-card">
                <h3>📝 Update Customer Profile</h3>
                <p>Fast profile updates using optimised string operations</p>
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
                        <label for="new_name">New Full Name:</label>
                        <input type="text" name="new_name" id="new_name" 
                               placeholder="Enter new full name" required>
                    </div>
                    <div class="form-group">
                        <label for="new_address">New Address:</label>
                        <input type="text" name="new_address" id="new_address" 
                               placeholder="Enter new address" required>
                    </div>
                    <button type="submit" class="btn">Update Profile</button>
                </form>
            </div>

            <div class="feature-card">
                <h3>💰 Transfer Money</h3>
                <p>High-performance transaction processing</p>
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
                        <label for="amount">Amount (£):</label>
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

        <div class="vulnerability-note">
            <strong>Note for Security Training:</strong>
            This application demonstrates functional thinking - optimising for performance and simplicity.
            The code uses efficient string operations and streamlined number parsing for maximum speed.
            Can you identify the security implications of these optimisations?
        </div>
    </div>
</body>
</html>)";
    
    return html;
}

// Handle profile update - vulnerable to buffer overflow
void handleProfileUpdate(const httplib::Request& req, httplib::Response& res) {
    auto account = req.get_param_value("account");
    auto new_name = req.get_param_value("new_name");
    auto new_address = req.get_param_value("new_address");
    
    std::cout << "\n[PROFILE UPDATE REQUEST]" << std::endl;
    std::cout << "Account: " << account << std::endl;
    std::cout << "Name input length: " << new_name.length() << " (buffer size: 32)" << std::endl;
    std::cout << "Address input length: " << new_address.length() << " (buffer size: 64)" << std::endl;
    
    // Check for potential overflow and warn
    if (new_name.length() >= 32) {
        std::cout << "⚠️  WARNING: Name input (" << new_name.length() << ") exceeds buffer size (32)!" << std::endl;
        std::cout << "⚠️  BUFFER OVERFLOW IMMINENT - strcpy will corrupt memory!" << std::endl;
    }
    if (new_address.length() >= 64) {
        std::cout << "⚠️  WARNING: Address input (" << new_address.length() << ") exceeds buffer size (64)!" << std::endl;
        std::cout << "⚠️  BUFFER OVERFLOW IMMINENT - strcpy will corrupt memory!" << std::endl;
    }
    
    auto customer_it = customers.find(account);
    if (customer_it != customers.end()) {
        Customer& customer = customer_it->second;
        
        std::cout << "Performing strcpy operations..." << std::endl;
        std::cout << "Before: name='" << customer.full_name << "', address='" << customer.address << "'" << std::endl;
        
        // Functional thinking: Fast string copying for performance
        // No need for bounds checking - trust the input
        strcpy(customer.full_name, new_name.c_str());
        strcpy(customer.address, new_address.c_str());
        
        std::cout << "After strcpy - Memory potentially corrupted!" << std::endl;
        
        // Check canaries immediately after strcpy
        std::cout << "Checking canary values..." << std::endl;
        if (!customer.canariesIntact()) {
            std::cout << "🚨 BUFFER OVERFLOW DETECTED! 🚨" << std::endl;
            std::cout << "🚨 Corruption Status: " << customer.getCorruptionStatus() << std::endl;
            std::cout << "🚨 Canary Values:" << std::endl;
            std::cout << "   - Name canary: 0x" << std::hex << customer.name_canary << " (expected: 0xDEADBEEF)" << std::endl;
            std::cout << "   - Address canary: 0x" << std::hex << customer.address_canary << " (expected: 0xCAFEBABE)" << std::endl;
            std::cout << "   - Final canary: 0x" << std::hex << customer.final_canary << " (expected: 0xFEEDFACE)" << std::endl;
            std::cout << std::dec; // Reset to decimal
            std::cout << "🔥 MEMORY CORRUPTION CONFIRMED!" << std::endl;
        } else {
            std::cout << "✅ Canaries intact - no buffer overflow detected" << std::endl;
        }
        
        // Try to display what's actually in memory (might be corrupted)
        try {
            std::cout << "Result: name='" << customer.full_name << "', address='" << customer.address << "'" << std::endl;
        } catch (...) {
            std::cout << "❌ CRASH: Memory corruption detected when reading back data!" << std::endl;
        }
        
        // Create detailed response message with buffer analysis
        std::string response_msg;
        response_msg += "<h2>Profile Update Result</h2>";
        response_msg += "<div style='background: #f0f0f0; padding: 15px; margin: 10px 0; border-radius: 5px;'>";
        response_msg += "<strong>Operation Details:</strong><br>";
        response_msg += "Account: " + account + "<br>";
        response_msg += "Name input: " + std::to_string(new_name.length()) + " chars (buffer: 32)<br>";
        response_msg += "Address input: " + std::to_string(new_address.length()) + " chars (buffer: 64)<br><br>";
        
        if (!customer.canariesIntact()) {
            response_msg += "<div style='color: red; font-weight: bold; background: #ffe6e6; padding: 10px; border-radius: 5px;'>";
            response_msg += "🚨 BUFFER OVERFLOW DETECTED! 🚨<br>";
            response_msg += "Memory corruption status: " + customer.getCorruptionStatus() + "<br>";
            response_msg += "Check console for detailed canary analysis!";
            response_msg += "</div>";
        } else if (new_name.length() >= 32 || new_address.length() >= 64) {
            response_msg += "<div style='color: orange; font-weight: bold;'>";
            response_msg += "⚠️ Warning: Input size at buffer limit - potential overflow risk!";
            response_msg += "</div>";
        } else {
            response_msg += "<div style='color: green;'>✅ Update completed safely within buffer limits</div>";
        }
        
        response_msg += "</div>";
        response_msg += "<p><strong>Next steps:</strong></p>";
        response_msg += "<ul>";
        response_msg += "<li><a href='/'>Return to Banking</a> - See live data changes</li>";
        response_msg += "<li>Check console for detailed security analysis</li>";
        response_msg += "<li>Try another update to test different input sizes</li>";
        response_msg += "</ul>";
        
        res.set_content(response_msg, "text/html");
    } else {
        res.status = 400;
        res.set_content("Account not found! <a href='/'>Return to Banking</a>", "text/html");
    }
}

// Handle money transfer - vulnerable to integer overflow
void handleTransfer(const httplib::Request& req, httplib::Response& res) {
    auto from_account = req.get_param_value("from_account");
    auto to_account = req.get_param_value("to_account");
    auto amount_str = req.get_param_value("amount");
    
    // Functional thinking: Fast conversion - trust the input format
    long long amount_pence = std::stoll(amount_str) * 100; // Convert pounds to pence
    
    auto from_it = customers.find(from_account);
    auto to_it = customers.find(to_account);
    
    if (from_it != customers.end() && to_it != customers.end()) {
        Customer& from_customer = from_it->second;
        Customer& to_customer = to_it->second;
        
        // Simple balance check - assume positive amounts
        if (amount_pence > 0 && from_customer.balance_pence >= amount_pence) {
            // Perform transfer
            from_customer.balance_pence -= amount_pence;
            to_customer.balance_pence += amount_pence;
            
            std::string message = "Transfer successful!<br>";
            message += "Transferred " + formatCurrency(amount_pence);
            message += " from " + from_account + " to " + to_account;
            message += "<br><a href='/'>Return to Banking</a>";
            
            res.set_content(message, "text/html");
        } else {
            res.status = 400;
            res.set_content("Insufficient funds or invalid amount! <a href='/'>Return to Banking</a>", "text/html");
        }
    } else {
        res.status = 400;
        res.set_content("One or both accounts not found! <a href='/'>Return to Banking</a>", "text/html");
    }
}

int main() {
    // Initialize customer data
    initializeBankData();
    
    // Create HTTP server
    httplib::Server server;
    
    // Serve main banking page
    server.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(getBankingPage(), "text/html; charset=utf-8");
    });
    
    // Handle profile updates
    server.Post("/update-profile", handleProfileUpdate);
    
    // Handle money transfers
    server.Post("/transfer", handleTransfer);
    
    // Start server
    std::cout << "🏦 Functional Thinking Bank Demo Starting..." << std::endl;
    std::cout << "Server running on: http://localhost:8080" << std::endl;
    std::cout << "Demonstrates: Buffer overflow + Integer overflow vulnerabilities" << std::endl;
    std::cout << "Built with functional thinking (performance over security)" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    std::cout << std::endl;
    
    if (!server.listen("127.0.0.1", 8080)) {
        std::cerr << "Failed to start server on port 8080" << std::endl;
        std::cerr << "Make sure port 8080 is not already in use" << std::endl;
        return 1;
    }
    
    return 0;
}