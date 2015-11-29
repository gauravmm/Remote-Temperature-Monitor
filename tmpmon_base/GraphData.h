#ifndef GRAPH_DATA_H
#define GRAPH_DATA_H

#include <Arduino.h>

#define DATA_LEN 96
#define DATA_HEIGHT 48

#ifndef FLT_MIN
#define FLT_MIN -999.9f
#endif

#ifndef FLT_MAX
#define FLT_MAX 999.9f
#endif

class GraphData
{
  public:
    GraphData();
    void push(float new_val);
	uint8_t * render();
	void alignment(float v);
	float vmin();
	float vmax();
    float val();
  private:
    float data[DATA_LEN];
    uint8_t render_data[DATA_LEN];
    uint8_t idx = 0;
    uint8_t recompute_render_data = 1;
    uint8_t include_sweep = 1;
    float v_min = FLT_MAX;
    float v_max = FLT_MIN;
    float v_val = 0.0f;
    float scale = 1.0f;
    float align = 1.0f;
};

#endif