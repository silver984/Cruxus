#include "parsed_path.hpp"
#include <filesystem>

namespace crx {

parsed_path::parsed_path(
    std::string_view stitched_val,
    std::string_view directory_val,
    std::string_view file_name_val,
    std::string_view extension_val
) :
    stitched(stitched_val),
    directory(directory_val),
    file_name(file_name_val),
    extension(extension_val)
{}

parsed_path parsed_path::parse(std::string_view file) {
    std::filesystem::path abs = std::filesystem::absolute(file);
    std::string ext = abs.extension().string();

    if (!ext.empty() && ext[0] == '.') {
        // remove the dot from the extension
        ext.erase(0, 1);
    }

    // make extension lowercase
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    return parsed_path(
        abs.string(),
        abs.parent_path().string(),
        abs.stem().string(),
        ext
    );
}

}