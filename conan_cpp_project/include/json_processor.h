#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

class JsonProcessor {
public:
    JsonProcessor() = default;
    ~JsonProcessor() = default;
    
    std::string create_sample_json() const;
    
    void parse_and_display(const std::string& json_str) const;
    
    std::string create_complex_json() const;
    
private:
    std::string format_json(const std::string& json_str) const;
};
