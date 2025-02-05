#pragma once
#include <filesystem>
#include <sstream>

inline std::string shortened_path( const std::filesystem::path& path ) {
  using namespace std::filesystem;

  std::ostringstream shortened_path;

  // Convert to an absolute path if needed and check existence
  std::filesystem::path abs_path = path.is_absolute() ? path : current_path() / path;
  if ( !exists( abs_path ) ) {
    throw filesystem_error( "Path does not exist", abs_path, std::error_code() );
  }

  // Preserve the leading separator for absolute paths
  if ( abs_path.is_absolute() ) {
    shortened_path << '/';
  }

  // Collect segments for processing
  std::vector<std::filesystem::path> segments;
  for ( const auto& segment : abs_path ) {
    if ( !segment.empty() && segment != "/" ) {
      segments.push_back(segment);
    }
  }

  // Handle each segment based on position
  for (size_t i = 0; i < segments.size(); ++i) {
    const std::string segment_str = segments[i].string();

    if (i == segments.size() - 2 || i == segments.size() - 1) {
      // Second-to-last and last segments: include fully
      shortened_path << segment_str;
      if (i == segments.size() - 2) {
        shortened_path << "/";
      }
    }
    else {
      // Other segments: shorten
      if (!isalnum(segment_str[0])) {
        shortened_path << segment_str.substr(0, 2) << "/";
      } else {
        shortened_path << segment_str[0] << "/";
      }
    }
  }

  return shortened_path.str();
}
