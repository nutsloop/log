#include "internal_debug.h++"

#include <iostream>

namespace nutsloop::nlog {

#if DEBUG_LOG == true

bool internal_debug::tmp_file_reassign_or_cleanup_() {

  const std::string directory_path = "/var/tmp";
  std::vector<std::filesystem::path> debug_files_found;

  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(directory_path)) {
    if (entry.is_regular_file()) {
      if (const auto &file_name = entry.path().filename().string();
          file_name.find("nutsloop_log") != std::string::npos) {
        debug_files_found.push_back(entry.path());
      }
    }
  }

  if (debug_files_found.empty()) {
    std::cout << "no debug file found.\n";

    return false;
  }

  std::cout << "found (" << debug_files_found.size()
            << ") log class debug files." << '\n';
  for (const auto &file_path : debug_files_found) {
    std::cout << "  " << file_path << "\n";
  }

  if (debug_files_found.size() > 1) {
    std::cout << "multiple log class debug files found." << '\n'
              << "Do you want to delete all of them? (y/n): ";
    char response;
    std::cin >> response;

    if (response == 'y' || response == 'Y') {
      for (const auto &file_path : debug_files_found) {
        try {
          std::filesystem::remove(file_path);
          std::cout << "Deleted: " << file_path << "\n";
        } catch (const std::filesystem::filesystem_error &e) {
          std::cerr << "Error deleting file: " << e.what() << "\n";
        }
      }
      return false;
    }
  }

  for (const auto &file_path : debug_files_found) {
    std::cout << std::format(
        "Do you want to reassign the stream to {}? (y/n): ",
        file_path.string());

    char response;
    std::cin >> response;

    if (response == 'y' || response == 'Y') {
      tmp_file_path_.store(
          new std::string(file_path.string())); // Atomic update
      tmp_file_already_set_.store(true);
      std::cout << "Reassigned: " << file_path << "\n";

      return true;
    }

    std::cout << "Do you want to delete this file? (y/n): ";
    std::cin >> response;

    if (response == 'y' || response == 'Y') {
      try {
        std::filesystem::remove(file_path);
        std::cout << "Deleted: " << file_path << "\n";
      } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Error deleting file: " << e.what() << "\n";
      }
    }
  }

  return false;
}

#endif
} // namespace nutsloop::nlog
