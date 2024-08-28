#include "minimalgltf.hpp"

int main() {
    const std::filesystem::path filePath = "../models/Cube.gltf";

    // Load the glTF file
    auto modelOpt = GLTFParser::load_from_file(filePath);
    if (!modelOpt) {
        std::cerr << "Failed to load glTF model." << std::endl;
        return -1;
    }

    GLTFModel& model = *modelOpt;
    std::cout << "Loaded glTF model with " << model.nodes.size() << " nodes." << std::endl;

    // Example of accessing nodes
    for (const auto& node : model.nodes) {
        std::cout << "Node name: " << node.name << std::endl;
        if (!node.translation.empty()) {
            std::cout << "  Translation: (" << node.translation[0] << ", " << node.translation[1] << ", " << node.translation[2] << ")" << std::endl;
        }
    }

    // Save the glTF model to a new file
    if (!GLTFParser::save_to_file(model, "path/to/your/model_output.gltf")) {
        std::cerr << "Failed to save glTF model." << std::endl;
        return -1;
    }

    return 0;
}
