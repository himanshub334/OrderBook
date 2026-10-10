// Optional pybind11 binding scaffold. Build with pybind11 installed and add this
// source to a CMake target; it is intentionally not part of the default build.
#include <pybind11/pybind11.h>
#include "orderbook/order_book.hpp"
namespace py = pybind11;
PYBIND11_MODULE(_orderbook, m) {
 py::enum_<ob::Side>(m,"Side").value("BUY",ob::Side::Buy).value("SELL",ob::Side::Sell);
 py::class_<ob::OrderBook>(m,"OrderBook").def(py::init<>())
  .def("best_bid",[](const ob::OrderBook& b)->py::object { auto v=b.best_bid(); return v?py::cast(*v):py::none(); })
  .def("best_ask",[](const ob::OrderBook& b)->py::object { auto v=b.best_ask(); return v?py::cast(*v):py::none(); })
  .def("cancel",&ob::OrderBook::cancel);
}
