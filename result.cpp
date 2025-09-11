
#include "result.hpp"

bool HResult::failed() const {
    return FAILED(result);
}

bool HResult::success() const {
    return !failed();
}

void HResult::clear_result() {
    result = 0;
}

HRESULT HResult::get_result() const {
    return result;
}
