// Wait multiple timers asynchronously.
// At the same time, run the loop in two threads.

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include <asio/io_context.hpp>
#include <asio/error_code.hpp>
#include <asio/steady_timer.hpp>

std::mutex g_io_mutex;

void Print(asio::error_code ec) {
  std::lock_guard<std::mutex> lock(g_io_mutex);

  std::cout << "Hello, World!";
  std::cout << " (" << std::this_thread::get_id() << ")" << std::endl;
}

int main() {
  asio::io_context io_context;

  asio::steady_timer timer1{ io_context, std::chrono::seconds(3) };
  asio::steady_timer timer2{ io_context, std::chrono::seconds(1) };

  timer1.async_wait(&Print);
  timer2.async_wait(&Print);

  std::thread t1([&]{ asio::io_context::run, &io_context });
  std::thread t2([&]{ asio::io_context::run, &io_context });

  t1.join();
  t2.join();

  return 0;
}