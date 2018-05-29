#include "cart_test_multi_result_sheets.h"

namespace tf0x_common {
CartTestMultiResultSheets::CartTestMultiResultSheets() : current_index_(-1)
{

}

CartTestMultiResultSheets::~CartTestMultiResultSheets() {

}

std::shared_ptr<CartTestResultSheet> CartTestMultiResultSheets::CurrentSheet() {
  if (current_index_ >= sheets_.size()) {
    for (auto& sheet : sheets_) {
      sheet->Clear();
    }
    if (sheets_.size()) {
      current_index_ = 0;
    } else {
      current_index_ = -1;
    }
  }
  if (current_index_ < 0) {
    return nullptr;
  }
  return sheets_[current_index_];
}

void CartTestMultiResultSheets::AddSheet(
    std::shared_ptr<CartTestResultSheet> sheet) {
  if (current_index_ < 0) {
    current_index_ = 0;
  }
  for (auto& sh : sheets_) {
    if (sh == sheet) {
      return;
    }
  }
  sheets_.push_back(sheet);
}

void CartTestMultiResultSheets::SheetDone() {
  ++current_index_;
}
}
