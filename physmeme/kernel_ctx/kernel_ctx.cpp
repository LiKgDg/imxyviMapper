#include "kernel_ctx.h"

namespace physmeme {

    kernel_ctx::kernel_ctx()
        : psyscall_func(nullptr), ntoskrnl_buffer(nullptr) {
        initialize();
    }

    kernel_ctx::~kernel_ctx() {
        cleanup();
    }

    void kernel_ctx::initialize() {
        nt_rva = reinterpret_cast<std::uint32_t>(
            util::get_module_export(
                "ntoskrnl.exe",
                syscall_hook.first.data(),
                true
            ));

        nt_page_offset = nt_rva % 0x1000;
        ntoskrnl_buffer = reinterpret_cast<std::uint8_t*>(LoadLibraryA("C:\\Windows\\System32\\ntoskrnl.exe"));

        search_syscall();

#if PHYSMEME_DEBUGGING
        std::cout << "[+] psyscall_func: " << std::hex << std::showbase << psyscall_func.load() << std::endl;
#endif
    }

    void kernel_ctx::cleanup() {
        // Add cleanup logic if needed
    }

    void kernel_ctx::search_syscall() {
        std::vector<std::thread> search_threads;

        for (auto ranges : pmem_ranges) {
            search_threads.emplace_back(std::thread(
                &kernel_ctx::map_syscall,
                this,
                ranges.first,
                ranges.second
            ));
        }

        for (std::thread& search_thread : search_threads) {
            search_thread.join();
        }
    }

    void kernel_ctx::map_syscall(std::uintptr_t begin, std::uintptr_t end) const {
        // Implementation of map_syscall function
        // ...

#if PHYSMEME_DEBUGGING
        // Debugging information
#endif
    }

    void* kernel_ctx::allocate_pool(std::size_t size, POOL_TYPE pool_type) {
        static const auto ex_alloc_pool =
            util::get_module_export(
                "ntoskrnl.exe",
                "ExAllocatePool"
            );
        if (ex_alloc_pool) {
            return syscall<ExAllocatePool>(ex_alloc_pool, pool_type, size);
        }
        return nullptr;
    }

    void* kernel_ctx::allocate_pool(std::size_t size, ULONG pool_tag, POOL_TYPE pool_type) {
        static const auto ex_alloc_pool_with_tag =
            util::get_module_export(
                "ntoskrnl.exe",
                "ExAllocatePoolWithTag"
            );
        if (ex_alloc_pool_with_tag) {
            return syscall<ExAllocatePoolWithTag>(ex_alloc_pool_with_tag, pool_type, size, pool_tag);
        }
        return nullptr;
    }

    void kernel_ctx::read_kernel(std::uintptr_t addr, void* buffer, std::size_t size) {
        // Implementation of read_kernel function
        // ...
    }

    void kernel_ctx::write_kernel(std::uintptr_t addr, void* buffer, std::size_t size) {
        // Implementation of write_kernel function
        // ...
    }

}  // namespace physmeme
