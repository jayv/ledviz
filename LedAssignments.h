#ifndef BURNINGMAN_LEDASSIGNMENTS_H
#define BURNINGMAN_LEDASSIGNMENTS_H

#include <cstdint>
typedef uint8_t byte;

const uint8_t COL_HEIGHTS[] = {27, 27, 26, 26};
const bool COL_DIR[] = {true, false, true, false}; // true = up, false = down
uint8_t CIRCLE_DEADZONE = 10;

uint8_t GetMaxColHeight() {
    uint8_t max_col_height = 0;
    for (uint8_t i = 0; i < 4; i++) {
        if (COL_HEIGHTS[i] > max_col_height) {
            max_col_height = COL_HEIGHTS[i];
        }
    }
    return max_col_height;
}

void GetRowAndColIdx(const uint8_t pxl_idx, uint8_t* row_idx, uint8_t* col_idx) {
    uint8_t col_height_sum = 0;
    for (uint8_t i = 0; i < sizeof(COL_HEIGHTS); i++) {
        col_height_sum += COL_HEIGHTS[i];

        if (pxl_idx < col_height_sum) {
            // Found column!
            *col_idx = i;
            if (COL_DIR[i]) {
                *row_idx = pxl_idx - (col_height_sum - COL_HEIGHTS[i]);
            } else {
                *row_idx = col_height_sum - pxl_idx - 1;
            }
            return;
        }
    }

    // ERROR: pxl_idx > n_pixels
    *row_idx = 0;
    *col_idx = 0;
}


uint8_t GetFullCircleIndex(const uint8_t pxl_idx) {
    uint8_t row_idx, col_idx;
    GetRowAndColIdx(pxl_idx, &row_idx, &col_idx);

    uint8_t circle_idx;
    if (col_idx < 2) { // left half of circle
        circle_idx = row_idx;
    } else { // right half of circle
        circle_idx = COL_HEIGHTS[0] + CIRCLE_DEADZONE + COL_HEIGHTS[2] - row_idx;
    }

    return circle_idx;
}

uint8_t GetFullCircleTotal() {
    return COL_HEIGHTS[0] + COL_HEIGHTS[2] + 2 * CIRCLE_DEADZONE;
}

#endif //BURNINGMAN_LEDASSIGNMENTS_H
