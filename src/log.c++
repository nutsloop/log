#include "log.h++"

namespace nutsloop {

#if DEBUG_LOG
std::atomic<bool> log::tmp_debug_file_already_set_{ false };
std::atomic<std::string*> log::tmp_debug_file_path_{};
std::unique_ptr<std::ofstream> log::debug_tmp_file_stream_{ nullptr };
#endif

std::shared_mutex log::mtx_;
std::atomic<bool> log::set_has_been_called_{ false };
std::atomic<bool> log::activated_{ false };

std::unique_ptr<log_registry_t> log::log_registry_{ nullptr };
std::unique_ptr<log_t> log::log_{ nullptr };

std::unique_ptr<log::stream_redirect_> log::stream_redirect_pointer_ = nullptr;
std::atomic<bool> log::stream_redirect_active_{ false };
log::null_logger_ log::null_log_;

}
