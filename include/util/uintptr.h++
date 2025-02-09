#include <cstdint>

namespace nutsloop {

template <typename T>
uintptr_t uintptr(T* ptr) {
  return reinterpret_cast<uintptr_t>(ptr);
}

template <typename T>
uintptr_t uintptr(const std::unique_ptr<T>& smart_ptr) {
  return reinterpret_cast<uintptr_t>(smart_ptr.get());
}

}
