#pragma once
#include <array>
#include <atomic>
#include <cstddef>
#include <new>
#include <type_traits>
#include <utility>
namespace ob {
template<class T, std::size_t Capacity> class SPSCQueue {
 static_assert(Capacity>=2 && (Capacity&(Capacity-1))==0,"Capacity must be power of two");
 static_assert(std::is_copy_assignable_v<T>,"T must be copy assignable");
 alignas(64) std::atomic<std::size_t> head_{0};
 alignas(64) std::atomic<std::size_t> tail_{0};
 std::array<T,Capacity> data_{};
public:
 bool push(const T& v) { auto h=head_.load(std::memory_order_relaxed); auto next=(h+1)&(Capacity-1); if(next==tail_.load(std::memory_order_acquire)) return false; data_[h]=v; head_.store(next,std::memory_order_release); return true; }
 bool pop(T& out) { auto t=tail_.load(std::memory_order_relaxed); if(t==head_.load(std::memory_order_acquire)) return false; out=data_[t]; tail_.store((t+1)&(Capacity-1),std::memory_order_release); return true; }
};
}
