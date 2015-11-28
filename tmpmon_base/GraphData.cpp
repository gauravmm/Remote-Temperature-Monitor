#include "GraphData.h"

GraphData::GraphData() {
  for(uint8_t i = 0; i < DATA_LEN; ++i) {
    data[i] = 0;
    render_data[i] = 0;
  }
}

void GraphData::push(float new_val) {
  v_val = new_val;
  // If the bounds are changed by the addition of new_val,
  if (new_val < v_min || new_val > v_max || data[idx] == v_min || data[idx] == v_max){
    // Set flags
    recompute_render_data = 1;
  } else {
    if (!recompute_render_data) {
      render_data[idx] = (uint8_t) ((new_val - v_min)*scale);
      if (include_sweep) {
        render_data[(idx + 1) % DATA_LEN] = 0;
        render_data[(idx + 2) % DATA_LEN] = DATA_HEIGHT;
        render_data[(idx + 3) % DATA_LEN] = 0;
      }
    }
  }
  data[idx] = new_val;
  idx = (idx + 1) % DATA_LEN;
}


uint8_t *GraphData::render() {
  if (recompute_render_data) {
    recompute_render_data = 0;

    // Recompute min and max;
    v_min = FLT_MAX;
    v_max = FLT_MIN;
    for(uint8_t i = 0; i < DATA_LEN; ++i) {
      if (data[i] < v_min)
        v_min = data[i];
      if (data[i] > v_max)
        v_max = data[i];
    }

    // We round min and max to the nearest multiple of align, with a minimum of
    // 0.5 aligns between it and the end of the scale.
    v_min = floor(v_min/align) * align;
    v_max = ceil((v_max + align/2.0f)/align) * align;
    
    // Recompute the scale
    scale = (DATA_HEIGHT * 1.0)/(v_max - v_min);

    // Now recompute the data:
    for(uint8_t i = 0; i < DATA_LEN; ++i) {
      render_data[i] = (uint8_t) ((data[i] - v_min)*scale);  
    }

    if (include_sweep) {
      render_data[(idx + 1) % DATA_LEN] = 0;
      render_data[(idx + 2) % DATA_LEN] = DATA_HEIGHT;
      render_data[(idx + 3) % DATA_LEN] = 0;
    }
  }

  return render_data;
}

void GraphData::alignment(float v) {
  align = v;
}

float GraphData::vmin() {
  return v_min;
}

float GraphData::vmax() {
  return v_max;
}

float GraphData::val() {
  return v_val;
}