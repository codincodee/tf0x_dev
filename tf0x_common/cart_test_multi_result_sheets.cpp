#include "cart_test_multi_result_sheets.h"

namespace tf0x_common {
CartTestMultiResultSheets::CartTestMultiResultSheets() : current_index_(-1)
{

}

CartTestMultiResultSheets::~CartTestMultiResultSheets() {

}

std::shared_ptr<CartTestResultSheet> CartTestMultiResultSheets::CurrentSheet() {
  return nullptr;
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
}
