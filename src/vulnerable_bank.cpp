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

// Customer data structure - optimised for simplicity
struct Customer {
    char account_number[20];
    char full_name[32];     // Buffer size chosen for typical UK names
    char address[64];       // Optimised for UK addresses
    long long balance_pence; // Store in pence for precision
    char phone[16];
    
    Customer(const char* acc, const char* name, const char* addr, long long bal, const char* ph) {
        // Functional thinking: quick string copying for performance
        strcpy(account_number, acc);
        strcpy(full_name, name);
        strcpy(address, addr);
        strcpy(phone, ph);
        balance_pence = bal;
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

// Main banking page HTML - embedded for performance
std::string getBankingPage() {
    return R"(<!DOCTYPE html>
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
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
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
            border-bottom: 3px solid #667eea;
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
            border-left: 5px solid #667eea;
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
            border-color: #667eea;
        }
        .btn {
            background: #667eea;
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
            background: #5a6fd8;
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

        <div class="customer-list">
            <h3>Current Customer Balances</h3>
            <div class="customer-item">
                <strong>ACC001 - James William Smith</strong><br>
                45 Victoria Street, London SW1H 0EU<br>
                Balance: <span style="color: #28a745; font-weight: bold;">£2,500.00</span>
            </div>
            <div class="customer-item">
                <strong>ACC002 - Sarah Elizabeth Jones</strong><br>
                12 King's Road, Brighton BN1 2HM<br>
                Balance: <span style="color: #28a745; font-weight: bold;">£750.00</span>
            </div>
            <div class="customer-item">
                <strong>ACC003 - Michael David Thompson</strong><br>
                8 Castle Street, Edinburgh EH1 2DP<br>
                Balance: <span style="color: #28a745; font-weight: bold;">£5,000.00</span>
            </div>
            <div class="customer-item">
                <strong>ACC004 - Emma Charlotte Wilson</strong><br>
                23 High Street, Manchester M1 1AA<br>
                Balance: <span style="color: #28a745; font-weight: bold;">£1,250.00</span>
            </div>
        </div>

        <div class="vulnerability-note">
            <strong>Note for Security Training:</strong>
            This application demonstrates functional thinking - optimising for performance and simplicity.
            The code uses efficient string operations and streamlined number parsing for maximum speed.
            Can you identify the security implications of these optimisations?
        </div>
    </div>
</body>
</html>)";
}

// Handle profile update - vulnerable to buffer overflow
void handleProfileUpdate(const httplib::Request& req, httplib::Response& res) {
    auto account = req.get_param_value("account");
    auto new_name = req.get_param_value("new_name");
    auto new_address = req.get_param_value("new_address");
    
    auto customer_it = customers.find(account);
    if (customer_it != customers.end()) {
        Customer& customer = customer_it->second;
        
        // Functional thinking: Fast string copying for performance
        // No need for bounds checking - trust the input
        strcpy(customer.full_name, new_name.c_str());
        strcpy(customer.address, new_address.c_str());
        
        res.set_content("Profile updated successfully! <a href='/'>Return to Banking</a>", "text/html");
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