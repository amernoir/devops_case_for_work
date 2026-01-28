#include "json_processor.h"
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using json = nlohmann::json;

std::string JsonProcessor::create_sample_json() const {
    spdlog::info("Creating sample JSON data");
    
    json data;
    data["application"] = "JsonFormatter";
    data["version"] = "1.0.0";
    data["author"] = "Test Program";
    data["features"] = {"json_parsing", "formatting", "logging"};
    data["metadata"]["created"] = "2024-01-26";
    data["metadata"]["language"] = "C++";
    data["statistics"]["lines_of_code"] = 150;
    data["statistics"]["dependencies"] = 3;
    
    return data.dump(4); // Pretty print с отступами
}

std::string JsonProcessor::create_complex_json() const {
    json products = json::array();
    
    // Первый продукт
    json product1;
    product1["id"] = 1001;
    product1["name"] = "Laptop";
    product1["price"] = 999.99;
    product1["in_stock"] = true;
    product1["tags"] = {"electronics", "computers", "portable"};
    product1["specs"]["cpu"] = "Intel i7";
    product1["specs"]["ram"] = "16GB";
    product1["specs"]["storage"] = "512GB SSD";
    products.push_back(product1);
    
    // Второй продукт
    json product2;
    product2["id"] = 1002;
    product2["name"] = "Smartphone";
    product2["price"] = 699.99;
    product2["in_stock"] = false;
    product2["tags"] = {"electronics", "mobile", "communication"};
    product2["specs"]["screen"] = "6.1\" OLED";
    product2["specs"]["battery"] = "4000mAh";
    product2["specs"]["camera"] = "48MP";
    products.push_back(product2);
    
    json root;
    root["store"] = "ElectroWorld";
    root["products"] = products;
    root["summary"]["total_products"] = products.size();
    root["summary"]["total_value"] = 1699.98;
    
    return root.dump(2);
}

void JsonProcessor::parse_and_display(const std::string& json_str) const {
    try {
        auto logger = spdlog::get("json_logger");
        if (!logger) {
            logger = spdlog::stdout_color_mt("json_logger");
        }
        
        json data = json::parse(json_str);
        
        logger->info("Successfully parsed JSON");
        logger->info("JSON contains {} top-level keys", data.size());
        
        // Выводим информацию о структуре
        for (auto& [key, value] : data.items()) {
            std::string type_str;
            if (value.is_object()) type_str = "object";
            else if (value.is_array()) type_str = "array";
            else if (value.is_string()) type_str = "string";
            else if (value.is_number()) type_str = "number";
            else if (value.is_boolean()) type_str = "boolean";
            else type_str = "unknown";
            
            logger->info("  Key: '{}', Type: {}, Size: {}", 
                        key, type_str, 
                        value.is_structured() ? value.size() : 0);
        }
        
    } catch (const json::parse_error& e) {
        spdlog::error("JSON parse error: {}", e.what());
        throw;
    }
}

std::string JsonProcessor::format_json(const std::string& json_str) const {
    try {
        return json::parse(json_str).dump(4);
    } catch (...) {
        return json_str; // Возвращаем как есть при ошибке
    }
}
