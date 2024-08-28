#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <nlohmann/json.hpp> // Include the JSON library header (header-only)

// Alias for JSON
using json = nlohmann::json;

struct GLTFNode {
    std::string name;
    std::vector<int> children;
    std::vector<double> matrix; // 4x4 matrix in column-major order
    std::vector<double> translation;
    std::vector<double> rotation;
    std::vector<double> scale;

    void from_json(const json& j) {
        if (j.contains("name")) name = j.at("name").get<std::string>();
        if (j.contains("children")) children = j.at("children").get<std::vector<int>>();
        if (j.contains("matrix")) matrix = j.at("matrix").get<std::vector<double>>();
        if (j.contains("translation")) translation = j.at("translation").get<std::vector<double>>();
        if (j.contains("rotation")) rotation = j.at("rotation").get<std::vector<double>>();
        if (j.contains("scale")) scale = j.at("scale").get<std::vector<double>>();
    }

    json to_json() const {
        json j;
        j["name"] = name;
        if (!children.empty()) j["children"] = children;
        if (!matrix.empty()) j["matrix"] = matrix;
        if (!translation.empty()) j["translation"] = translation;
        if (!rotation.empty()) j["rotation"] = rotation;
        if (!scale.empty()) j["scale"] = scale;
        return j;
    }
};

struct GLTFModel {
    std::vector<GLTFNode> nodes;

    void from_json(const json& j) {
        if (j.contains("nodes")) {
            nodes.clear();
            for (const auto& node : j.at("nodes")) {
                GLTFNode gltfNode;
                gltfNode.from_json(node);
                nodes.push_back(gltfNode);
            }
        }
    }

    json to_json() const {
        json j;
        for (const auto& node : nodes) {
            j["nodes"].push_back(node.to_json());
        }
        return j;
    }
};

class GLTFParser {
public:
    static std::optional<GLTFModel> load_from_file(const std::filesystem::path& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return std::nullopt;
        }

        json j;
        try {
            file >> j;
        } catch (const json::parse_error& e) {
            std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
            return std::nullopt;
        }

        GLTFModel model;
        model.from_json(j);
        return model;
    }

    static bool save_to_file(const GLTFModel& model, const std::filesystem::path& path) {
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << path << std::endl;
            return false;
        }

        json j = model.to_json();
        try {
            file << j.dump(4); // Pretty print with an indent of 4 spaces
        } catch (const std::exception& e) {
            std::cerr << "Failed to write JSON to file: " << e.what() << std::endl;
            return false;
        }

        return true;
    }
};
