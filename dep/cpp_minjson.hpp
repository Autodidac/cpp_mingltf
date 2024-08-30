#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <string_view>
#include <charconv>  // For std::from_chars
#include <stdexcept>
#include <sstream>
#include <cassert>

namespace json {

// JSON value can be an object, array, string, number, boolean, or null
using JsonValue = std::variant<std::nullptr_t, bool, double, std::string, 
                               std::vector<JsonValue>, 
                               std::unordered_map<std::string, JsonValue>>;

enum class JsonType {
    Null, Bool, Number, String, Array, Object
};

// Utility function to identify type
inline JsonType get_type(const JsonValue& value) {
    return static_cast<JsonType>(value.index());
}

// JSON Parsing Error
struct JsonParseException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

// Trim whitespace characters
inline std::string_view trim_whitespace(std::string_view sv) {
    sv.remove_prefix(std::min(sv.find_first_not_of(" \t\n\r"), sv.size()));
    sv.remove_suffix(sv.size() - sv.find_last_not_of(" \t\n\r") - 1);
    return sv;
}

// Parse a JSON null value
inline JsonValue parse_null(std::string_view& sv) {
    if (sv.substr(0, 4) == "null") {
        sv.remove_prefix(4);
        return nullptr;
    }
    throw JsonParseException("Invalid JSON: Expected 'null'");
}

// Parse a JSON boolean value
inline JsonValue parse_bool(std::string_view& sv) {
    if (sv.substr(0, 4) == "true") {
        sv.remove_prefix(4);
        return true;
    } else if (sv.substr(0, 5) == "false") {
        sv.remove_prefix(5);
        return false;
    }
    throw JsonParseException("Invalid JSON: Expected 'true' or 'false'");
}

// Parse a JSON number
inline JsonValue parse_number(std::string_view& sv) {
    double result;
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), result);
    if (ec == std::errc()) {
        sv.remove_prefix(ptr - sv.data());
        return result;
    }
    throw JsonParseException("Invalid JSON: Unable to parse number");
}

// Parse a JSON string
inline JsonValue parse_string(std::string_view& sv) {
    if (sv.front() != '"') throw JsonParseException("Invalid JSON: Expected '\"'");
    sv.remove_prefix(1); // Remove leading "
    
    std::string result;
    while (!sv.empty() && sv.front() != '"') {
        if (sv.front() == '\\') {
            sv.remove_prefix(1);
            if (sv.empty()) throw JsonParseException("Invalid JSON: Unexpected end of input in string");
            switch (sv.front()) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                default: throw JsonParseException("Invalid JSON: Unsupported escape character");
            }
        } else {
            result += sv.front();
        }
        sv.remove_prefix(1);
    }
    
    if (sv.empty() || sv.front() != '"') throw JsonParseException("Invalid JSON: Unterminated string");
    sv.remove_prefix(1); // Remove trailing "
    
    return result;
}

// Parse a JSON array
inline JsonValue parse_array(std::string_view& sv) {
    if (sv.front() != '[') throw JsonParseException("Invalid JSON: Expected '['");
    sv.remove_prefix(1); // Remove leading '['
    
    std::vector<JsonValue> result;
    while (!sv.empty()) {
        sv = trim_whitespace(sv);
        if (sv.front() == ']') {
            sv.remove_prefix(1); // Remove trailing ']'
            return result;
        }
        
        result.push_back(parse_value(sv));
        
        sv = trim_whitespace(sv);
        if (sv.front() == ',') {
            sv.remove_prefix(1); // Remove ','
        } else if (sv.front() != ']') {
            throw JsonParseException("Invalid JSON: Expected ',' or ']'");
        }
    }
    
    throw JsonParseException("Invalid JSON: Unterminated array");
}

// Parse a JSON object
inline JsonValue parse_object(std::string_view& sv) {
    if (sv.front() != '{') throw JsonParseException("Invalid JSON: Expected '{'");
    sv.remove_prefix(1); // Remove leading '{'
    
    std::unordered_map<std::string, JsonValue> result;
    while (!sv.empty()) {
        sv = trim_whitespace(sv);
        if (sv.front() == '}') {
            sv.remove_prefix(1); // Remove trailing '}'
            return result;
        }
        
        auto key = std::get<std::string>(parse_string(sv));
        sv = trim_whitespace(sv);
        if (sv.front() != ':') throw JsonParseException("Invalid JSON: Expected ':' after key");
        sv.remove_prefix(1); // Remove ':'
        
        result[key] = parse_value(sv);
        
        sv = trim_whitespace(sv);
        if (sv.front() == ',') {
            sv.remove_prefix(1); // Remove ','
        } else if (sv.front() != '}') {
            throw JsonParseException("Invalid JSON: Expected ',' or '}'");
        }
    }
    
    throw JsonParseException("Invalid JSON: Unterminated object");
}

// General JSON value parser
inline JsonValue parse_value(std::string_view& sv) {
    sv = trim_whitespace(sv);
    if (sv.empty()) throw JsonParseException("Invalid JSON: Empty input");

    switch (sv.front()) {
        case 'n': return parse_null(sv);
        case 't': case 'f': return parse_bool(sv);
        case '"': return parse_string(sv);
        case '[': return parse_array(sv);
        case '{': return parse_object(sv);
        default:  return parse_number(sv);
    }
}

// Serialize JSON value to string
inline std::string serialize(const JsonValue& value) {
    switch (get_type(value)) {
        case JsonType::Null: return "null";
        case JsonType::Bool: return std::get<bool>(value) ? "true" : "false";
        case JsonType::Number: {
            std::ostringstream oss;
            oss.precision(15);
            oss << std::get<double>(value);
            return oss.str();
        }
        case JsonType::String: return "\"" + std::get<std::string>(value) + "\"";
        case JsonType::Array: {
            std::string result = "[";
            const auto& array = std::get<std::vector<JsonValue>>(value);
            for (size_t i = 0; i < array.size(); ++i) {
                if (i > 0) result += ",";
                result += serialize(array[i]);
            }
            return result + "]";
        }
        case JsonType::Object: {
            std::string result = "{";
            const auto& obj = std::get<std::unordered_map<std::string, JsonValue>>(value);
            size_t i = 0;
            for (const auto& [key, val] : obj) {
                if (i++ > 0) result += ",";
                result += "\"" + key + "\":" + serialize(val);
            }
            return result + "}";
        }
        default: throw JsonParseException("Unknown JSON value type");
    }
}

} // namespace json
