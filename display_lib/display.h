#ifndef _DISPLAY_LIB_
#define _DISPLAY_LIB_


//*** This library is part of oscilloscope application based on stm32f746 embedded in STM32 Discovery board ***//
//*** These functions implement graphical features necessary to basic oscilloscope features ***//
//*** This library is dedicated to 272px x 480px display ***//

//*** Display Layout***//
/// _____________________________
///|          Top Bar            |
///|-----------------------------|
///|                     |       |
///|         Chart       |Buttons|
///|                     |       |
///|-----------------------------|
///|          Bottom Bar         |
///|_____________________________|


#include "main.h"
#include "lvgl.h"
#include "core_cm7.h"
#include "lvgl.h"
#include "ui.h"
#include "stm32746g_discovery_sdram.h"
#include "adc.h"
#include "ltdc.h"
#include <string.h>
#include <stdbool.h>
#include "arm_math.h"

#define RESOLUTION_HORIZONTAL 480
#define RESOLUTION_VERTICAL 272
#define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565))

#define DISPLAY_BUTTONS_WIDTH 	76
#define DISPLAY_BUTTONS_HEIGHT 	48
#define DISPLAY_CHART_WIDTH 	404
#define DISPLAY_CHART_HEIGHT 	248

#define BUTTON_PRESSED			1
#define BUTTON_RELEASED			0

typedef struct button_state_t{
	bool BUTTON_STOP;
	bool BUTTON_CURSOR;
	bool BUTTON_DC_AC_COUPLING;
	bool BUTTONMATH;
	bool BUTTON_OPTIONS;
}button_state_t;


/// display initialization
void display_init(void);

/// my flush call back
void my_flush_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map);

///  Widget description: Plot current and past input signal sample's measured by ADC3.
///
///  Displayed features:
/// 					- Chart
///			  			- Grid
void display_chart_window(void);


///  Widget description: right side buttons which allows to choose oscilloscope options
///
///  Displayed features:
/// 					- "Stop/Run"                Button
///						- "Place X/Y Cursor 1 | 2"  button
/// 					- "DC/AC Coupling"          button
/// 					- "Math functions"          button
///						- "More options"            button
void display_buttons_window(void);


///  Widget description: bottom bar measure
///
///  Displayed features:
/// 					- "VOLTS/DIV"                 label
///						- "TIME/DIV"                  label
/// 					- "X_Coordinate Cursor_1 | 2" label
/// 					- "Y_Coordinate Cursor_1 | 2" label
void display_bottom_bar_window(void);


///  Widget description: top bar additional informations
///
///  Displayed features:
/// 					- Time     label
///						- "Status" label
///	Due to small screen size topBarWindow is not used. Some data from Top bar were moved to bottom bar
///void display_topBarWindow(void);


///  Widget description: chart update callback called by lv_timer_create(display_chartWindow())
///
///  Displayed features:
/// 					- update chart
///						- STOP/RUN button feature
void update_chart(lv_timer_t *timer);


///  Widget description: display chart axes: OX and OY, additionally display scale and chart grid
///
///  Displayed features:
/// 					- display scale
///						- display chart grid
///						-
void display_set_axis(void);


void display_create_math_window_box(void);
void display_update_math_window_box(void);
void display_toggle_window_box(void *obj);

#endif
