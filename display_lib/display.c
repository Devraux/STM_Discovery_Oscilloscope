#include "display.h"


/// Private variables ///
uint8_t buf1[RESOLUTION_HORIZONTAL * RESOLUTION_VERTICAL / 2 * BYTES_PER_PIXEL];
uint16_t *framebuffer;

static button_state button_state_t = {BUTTON_RELEASED};

////////////////////// Right side buttons local objects/////////////////
static lv_obj_t *label_stopRun;
static lv_obj_t *label_Cursor;
static lv_obj_t *label_DC_ACCoupling;
static lv_obj_t *label_mathFun;
static lv_obj_t *label_options;
////////////////////////////////////////////////////////////////////////

////////////////////// Bottom bar local objects ////////////////////////
static lv_obj_t *voltsDiv;
static lv_obj_t *timeDiv;
static lv_obj_t *time;
////////////////////////////////////////////////////////////////////////

////////////////////// Data display local objects //////////////////////
static lv_obj_t *chart;
static lv_chart_series_t *ADC_dataSeries;

////////////////////////////////////////////////////////////////////////

////////////////////// Measure window box local objects/////////////////
static lv_obj_t *math_windowBox;
static lv_obj_t *label_minVal;
static lv_obj_t *label_maxVal;
////////////////////////////////////////////////////////////////////////

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
/// STOP BUTTON CASE
//  if (GPIO_Pin == USER_BUTTON_Pin) //temporary stop button
//  {
//	  button_state_t.BUTTON_STOP = !button_state_t.BUTTON_STOP;
//  }

/// MATH BUTTON CASE
	if (GPIO_Pin == USER_BUTTON_Pin) //temporary stop button
	{
		//display_toggleWindowBox(math_box);
		lv_async_call(display_toggleWindowBox, math_windowBox);
	}

}

void my_flush_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
    uint16_t *buf16 = (uint16_t *)px_map;
    uint16_t *framebuffer = (uint16_t *)(SDRAM_BANK_ADDR);

    int32_t width = area->x2 - area->x1 + 1;
    int32_t height = area->y2 - area->y1 + 1;

    for (int32_t y = 0; y < height; y++)
        memcpy(&framebuffer[(area->y1 + y) * 480 + area->x1], &buf16[y * width], width * sizeof(uint16_t));

    display_Simple_Update(SDRAM_BANK_ADDR);
    lv_display_flush_ready(display);
}

void display_init(void)
{
	lv_init();
	//lv_tick_set_cb(HAL_GetTick);
	lv_display_t *display1 = lv_display_create(RESOLUTION_HORIZONTAL, RESOLUTION_VERTICAL);
	lv_display_set_buffers(display1, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
	lv_display_set_flush_cb(display1, my_flush_cb);
	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x43444d), LV_STATE_DEFAULT);
	HAL_NVIC_SetPriority(SysTick_IRQn, 1, 0);
	//ui_init();


	//Display tool bars and chart template initialization
	display_chartWindow();
	display_bottomBarWindow();
	display_buttonsWindow();
	display_createMathWindowBox();


}

void display_buttonsWindow(void)
{

	//////////////////////////// Stop/Run /////////////////////////////////////
	lv_obj_t *btn_stopRun = lv_button_create(lv_screen_active());
	lv_obj_set_size(btn_stopRun, DISPLAY_BUTTONS_WIDTH, DISPLAY_BUTTONS_HEIGHT);
	lv_obj_align(btn_stopRun, LV_ALIGN_TOP_RIGHT, 0, 0);
	lv_obj_set_style_bg_color(btn_stopRun, lv_color_hex(0x1b39c6), LV_PART_MAIN);
	lv_obj_set_style_radius(btn_stopRun, 2, LV_PART_MAIN);


	label_stopRun = lv_label_create(btn_stopRun);
	lv_label_set_text(label_stopRun, "Stop");
	lv_obj_center(label_stopRun);
	lv_obj_set_style_text_color(label_stopRun, lv_color_hex(0xd6e32b), LV_PART_MAIN);
	///////////////////////////////////////////////////////////////////////////


	//////////////////////////// Place X/Y Cursor 1 | 2 ///////////////////////
	lv_obj_t *btn_Cursor = lv_button_create(lv_screen_active());
	lv_obj_set_size(btn_Cursor, DISPLAY_BUTTONS_WIDTH, DISPLAY_BUTTONS_HEIGHT);
	lv_obj_align(btn_Cursor, LV_ALIGN_TOP_RIGHT,0 ,50);
	lv_obj_set_style_bg_color(btn_Cursor, lv_color_hex(0x1b39c6), LV_PART_MAIN);
	lv_obj_set_style_radius(btn_Cursor, 2, LV_PART_MAIN);

	label_Cursor = lv_label_create(btn_Cursor);
	lv_label_set_text(label_Cursor, "Cursor");
	lv_obj_center(label_Cursor);
	lv_obj_set_style_text_color(label_Cursor, lv_color_hex(0xd6e32b), LV_PART_MAIN);
	///////////////////////////////////////////////////////////////////////////

	//////////////////////////// DC/AC Coupling ///////////////////////////////
	lv_obj_t *btn_DC_ACCoupling = lv_button_create(lv_screen_active());
	lv_obj_set_size(btn_DC_ACCoupling, DISPLAY_BUTTONS_WIDTH, DISPLAY_BUTTONS_HEIGHT);
	lv_obj_align(btn_DC_ACCoupling, LV_ALIGN_TOP_RIGHT, 0, 100);
	lv_obj_set_style_bg_color(btn_DC_ACCoupling, lv_color_hex(0x1b39c6), LV_PART_MAIN);
	lv_obj_set_style_radius(btn_DC_ACCoupling, 2, LV_PART_MAIN);

	label_DC_ACCoupling = lv_label_create(btn_DC_ACCoupling);
	lv_label_set_text(label_DC_ACCoupling, "DC/AC");
	lv_obj_center(label_DC_ACCoupling);
	lv_obj_set_style_text_color(label_DC_ACCoupling, lv_color_hex(0xd6e32b), LV_PART_MAIN);
	///////////////////////////////////////////////////////////////////////////

	//////////////////////////// Math functions ///////////////////////////////
	lv_obj_t *btn_mathFun = lv_button_create(lv_screen_active());
	lv_obj_set_size(btn_mathFun, DISPLAY_BUTTONS_WIDTH, DISPLAY_BUTTONS_HEIGHT);
	lv_obj_align(btn_mathFun, LV_ALIGN_TOP_RIGHT, 0, 150);
	lv_obj_set_style_bg_color(btn_mathFun, lv_color_hex(0x1b39c6), LV_PART_MAIN);
	lv_obj_set_style_radius(btn_mathFun, 2, LV_PART_MAIN);

	label_mathFun = lv_label_create(btn_mathFun);
	lv_label_set_text(label_mathFun, "Math");
	lv_obj_center(label_mathFun);
	lv_obj_set_style_text_color(label_mathFun, lv_color_hex(0xd6e32b), LV_PART_MAIN);
	///////////////////////////////////////////////////////////////////////////

	//////////////////////////// More options /////////////////////////////////
	lv_obj_t *btn_options = lv_button_create(lv_screen_active());
	lv_obj_set_size(btn_options, DISPLAY_BUTTONS_WIDTH, DISPLAY_BUTTONS_HEIGHT);
	lv_obj_align(btn_options, LV_ALIGN_TOP_RIGHT, 0, 200);
	lv_obj_set_style_bg_color(btn_options, lv_color_hex(0x1b39c6), LV_PART_MAIN);
	lv_obj_set_style_radius(btn_options, 2, LV_PART_MAIN);

	label_options = lv_label_create(btn_options);
	lv_label_set_text(label_options, "Options");
	lv_obj_center(label_options);
	lv_obj_set_style_text_color(label_options, lv_color_hex(0xd6e32b), LV_PART_MAIN);
	///////////////////////////////////////////////////////////////////////////
}

void display_bottomBarWindow(void)
{
	lv_obj_t *bottom_bar = lv_obj_create(lv_screen_active());
	lv_obj_set_size(bottom_bar, 480, 24);
	lv_obj_align(bottom_bar, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_obj_set_style_bg_color(bottom_bar, lv_color_hex(0x1b39c6), LV_PART_MAIN);
	lv_obj_clear_flag(bottom_bar, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_style_border_width(bottom_bar, 0, LV_PART_MAIN);
	lv_obj_set_style_radius(bottom_bar, 0, LV_PART_MAIN);


	voltsDiv = lv_label_create(bottom_bar);
	lv_label_set_text(voltsDiv, "V/Div:");
	lv_obj_align(voltsDiv, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_style_text_color(voltsDiv, lv_color_hex(0xd6e32b), LV_PART_MAIN);

	timeDiv = lv_label_create(bottom_bar);
	lv_label_set_text(timeDiv, "s/Div:");
	lv_obj_align(timeDiv, LV_ALIGN_LEFT_MID, 80, 0);
	lv_obj_set_style_text_color(timeDiv, lv_color_hex(0xd6e32b), LV_PART_MAIN);

	time = lv_label_create(bottom_bar);
	lv_label_set_text(time, "Time: 12:15");
	lv_obj_align(time, LV_ALIGN_LEFT_MID, 370, 0);
	lv_obj_set_style_text_color(time, lv_color_hex(0xd6e32b), LV_PART_MAIN);
}

void display_chartWindow(void)
{
////////////////////// chart basic properties ////////////////////////////
	chart = lv_chart_create(lv_screen_active());
	lv_obj_set_size(chart, DISPLAY_CHART_WIDTH, DISPLAY_CHART_HEIGHT);
	lv_obj_align(chart, LV_ALIGN_CENTER, -38, -12);
	lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
	lv_obj_set_style_radius(chart, 2, LV_PART_MAIN); // set chart corner style(radius)
	lv_obj_set_style_border_color(chart, lv_color_hex(0x1b39c6), LV_PART_MAIN);
	lv_obj_set_style_border_width(chart, 2, LV_PART_MAIN);
	lv_obj_set_style_pad_all(chart, 0, LV_PART_MAIN);
	lv_obj_set_style_bg_color(chart, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_chart_set_div_line_count(chart, 11, 15);
	lv_obj_set_style_width(chart, 0, LV_PART_SCROLLBAR);
	//lv_obj_set_style_bg_opa(chart, LV_OPA_COVER, LV_PART_MAIN);
////////////////////////////////////////////////////////////////////////


////////////////////// chart OX & OY PLOT AXIS ////////////////////////
	display_setAxis();
////////////////////////////////////////////////////////////////////////


////////////////////// chart ADC Data plot ////////////////////////////
	ADC_dataSeries = lv_chart_add_series(chart, lv_color_hex(0xff0000), LV_CHART_AXIS_PRIMARY_Y);
	lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -4200, 4200);
    lv_chart_set_point_count(chart, 400);
    lv_chart_set_ext_y_array(chart, ADC_dataSeries, (int32_t*)adc_get_current_buffer());

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 3);
    lv_obj_add_style(chart, &style_line, LV_PART_ITEMS);

    lv_timer_create(update_chart, 100, NULL);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
	lv_chart_refresh(chart);
////////////////////////////////////////////////// ////////////////////


}

void display_setAxis(void)
{
	////////////////////// chart OX & OY Axis /////////////////////
	static lv_style_t style_axis;
	lv_style_init(&style_axis);
	lv_style_set_line_width(&style_axis, 1);
	lv_style_set_line_color(&style_axis, lv_color_hex(0x000000));

	////////////////////// chart AXES STYLE ///////////////////////
	lv_obj_add_style(chart, &style_axis, LV_PART_MAIN);

	////////////////////// chart MIDDLE OX ////////////////////////
	lv_obj_t *OX_scale_1 = lv_scale_create(chart);
	lv_scale_set_mode(OX_scale_1, LV_SCALE_MODE_HORIZONTAL_TOP);
	lv_obj_set_size(OX_scale_1, DISPLAY_CHART_WIDTH - 4, 0);
	lv_obj_set_style_pad_hor(OX_scale_1, lv_chart_get_first_point_center_offset(chart), 0);
	lv_obj_set_style_text_font(OX_scale_1, NULL, 0);
	lv_obj_align(OX_scale_1, LV_ALIGN_LEFT_MID, 0, 1);
	lv_scale_set_total_tick_count(OX_scale_1, 71);
	lv_scale_set_major_tick_every(OX_scale_1, 5);
	lv_obj_set_style_length(OX_scale_1, 3, LV_PART_ITEMS);
	lv_obj_set_style_length(OX_scale_1, 5, LV_PART_INDICATOR);
	lv_obj_set_style_line_width(OX_scale_1, 1, LV_PART_ITEMS);
	lv_obj_set_style_line_width(OX_scale_1, 1, LV_PART_INDICATOR);
	//lv_obj_set_style_line_color(OX_scale_1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_ITEMS);
	//lv_obj_set_style_line_color(OX_scale_1, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
	lv_obj_add_style(OX_scale_1, &style_axis, LV_PART_MAIN);

	lv_obj_t *OX_scale_2 = lv_scale_create(chart);
	lv_scale_set_mode(OX_scale_2, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
	lv_obj_set_size(OX_scale_2, DISPLAY_CHART_WIDTH - 4, 0);
	lv_obj_set_style_pad_hor(OX_scale_2, lv_chart_get_first_point_center_offset(chart), 0);
	lv_obj_set_style_text_font(OX_scale_2, NULL, 0);
	lv_obj_align(OX_scale_2, LV_ALIGN_LEFT_MID, 0, 1);
	lv_scale_set_total_tick_count(OX_scale_2, 71);
	lv_scale_set_major_tick_every(OX_scale_2, 5);
	lv_obj_set_style_length(OX_scale_2, 3, LV_PART_ITEMS);
	lv_obj_set_style_length(OX_scale_2, 5, LV_PART_INDICATOR);
	lv_obj_set_style_line_width(OX_scale_2, 1, LV_PART_ITEMS);
	lv_obj_set_style_line_width(OX_scale_2, 1, LV_PART_INDICATOR);
	//lv_obj_set_style_line_color(OX_scale_2, lv_palette_main(LV_PALETTE_BLUE), LV_PART_ITEMS);
	//lv_obj_set_style_line_color(OX_scale_2, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
	lv_obj_add_style(OX_scale_2, &style_axis, LV_PART_MAIN);


	////////////////////// chart BOTTOM SIDE OX ////////////////////////
	lv_obj_t *OX_scaleBottom = lv_scale_create(chart);
	lv_scale_set_mode(OX_scaleBottom, LV_SCALE_MODE_HORIZONTAL_TOP);
	lv_obj_set_size(OX_scaleBottom, DISPLAY_CHART_WIDTH - 4, 0);
	lv_obj_set_style_pad_hor(OX_scaleBottom, lv_chart_get_first_point_center_offset(chart), 0);
	lv_obj_set_style_text_font(OX_scaleBottom, NULL, 0);
	lv_obj_align(OX_scaleBottom, LV_ALIGN_BOTTOM_MID, 0, 1);
	lv_scale_set_total_tick_count(OX_scaleBottom, 71);
	lv_scale_set_major_tick_every(OX_scaleBottom, 5);
	lv_obj_set_style_length(OX_scaleBottom, 4, LV_PART_ITEMS);
	lv_obj_set_style_length(OX_scaleBottom, 6, LV_PART_INDICATOR);
	lv_obj_set_style_line_width(OX_scaleBottom, 1, LV_PART_ITEMS);
	lv_obj_set_style_line_width(OX_scaleBottom, 1, LV_PART_INDICATOR);
	//lv_obj_set_style_line_color(OX_scale_1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_ITEMS);
	//lv_obj_set_style_line_color(OX_scale_1, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
	lv_obj_add_style(OX_scaleBottom, &style_axis, LV_PART_MAIN);


	////////////////////// chart TOP SIDE OX ////////////////////////
	lv_obj_t *OX_scaleTop = lv_scale_create(chart);
	lv_scale_set_mode(OX_scaleTop, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
	lv_obj_set_size(OX_scaleTop, DISPLAY_CHART_WIDTH - 4, 0);
	lv_obj_set_style_pad_hor(OX_scaleTop, lv_chart_get_first_point_center_offset(chart), 0);
	lv_obj_set_style_text_font(OX_scaleTop, NULL, 0);
	lv_obj_align(OX_scaleTop, LV_ALIGN_TOP_MID, 0, 0);
	lv_scale_set_total_tick_count(OX_scaleTop, 71);
	lv_scale_set_major_tick_every(OX_scaleTop, 5);
	lv_obj_set_style_length(OX_scaleTop, 4, LV_PART_ITEMS);
	lv_obj_set_style_length(OX_scaleTop, 6, LV_PART_INDICATOR);
	lv_obj_set_style_line_width(OX_scaleTop, 1, LV_PART_ITEMS);
	lv_obj_set_style_line_width(OX_scaleTop, 1, LV_PART_INDICATOR);
	//lv_obj_set_style_line_color(OX_scale_2, lv_palette_main(LV_PALETTE_BLUE), LV_PART_ITEMS);
	//lv_obj_set_style_line_color(OX_scale_2, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
	lv_obj_add_style(OX_scaleTop, &style_axis, LV_PART_MAIN);


	////////////////////// chart MIDDLE OY ////////////////////////
	lv_obj_t * OY_scale_1 = lv_scale_create(chart);
	lv_scale_set_mode(OY_scale_1, LV_SCALE_MODE_VERTICAL_RIGHT);
	lv_obj_set_size(OY_scale_1, 0, DISPLAY_CHART_HEIGHT - 4);
	lv_obj_set_style_pad_hor(OY_scale_1, lv_chart_get_first_point_center_offset(chart), 0);
	lv_obj_set_style_text_font(OY_scale_1, NULL, 0);
	lv_obj_align(OY_scale_1, LV_ALIGN_CENTER, 2, 0);
	lv_scale_set_total_tick_count(OY_scale_1, 51);
	lv_scale_set_major_tick_every(OY_scale_1, 5);
	lv_obj_set_style_length(OY_scale_1, 4, LV_PART_ITEMS);
	lv_obj_set_style_length(OY_scale_1, 6, LV_PART_INDICATOR);
	lv_obj_set_style_line_width(OY_scale_1, 1, LV_PART_ITEMS);
	lv_obj_set_style_line_width(OY_scale_1, 1, LV_PART_INDICATOR);
	//lv_obj_set_style_line_color(OY_scale_1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_ITEMS);
	//lv_obj_set_style_line_color(OY_scale_1, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
	lv_obj_add_style(OY_scale_1, &style_axis, LV_PART_MAIN);

	lv_obj_t * OY_scale_2 = lv_scale_create(chart);
	lv_scale_set_mode(OY_scale_2, LV_SCALE_MODE_VERTICAL_LEFT);
	lv_obj_set_size(OY_scale_2, 0, DISPLAY_CHART_HEIGHT - 4);
	lv_obj_set_style_pad_hor(OY_scale_2, lv_chart_get_first_point_center_offset(chart), 0);
	lv_obj_set_style_text_font(OY_scale_2, NULL, 0);
	lv_obj_align(OY_scale_2, LV_ALIGN_CENTER, 2, 0);
	lv_scale_set_total_tick_count(OY_scale_2, 51);
	lv_scale_set_major_tick_every(OY_scale_2, 5);
	lv_obj_set_style_length(OY_scale_2, 4, LV_PART_ITEMS);
	lv_obj_set_style_length(OY_scale_2, 6, LV_PART_INDICATOR);
	lv_obj_set_style_line_width(OY_scale_2, 1, LV_PART_ITEMS);
	lv_obj_set_style_line_width(OY_scale_2, 1, LV_PART_INDICATOR);
	//lv_obj_set_style_line_color(OY_scale_2, lv_palette_main(LV_PALETTE_BLUE), LV_PART_ITEMS);
	//lv_obj_set_style_line_color(OY_scale_2, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
	lv_obj_add_style(OY_scale_2, &style_axis, LV_PART_MAIN);


	////////////////////// chart LEFT SIDE OY ////////////////////////
	lv_obj_t * OY_scaleLeft = lv_scale_create(chart);
	lv_scale_set_mode(OY_scaleLeft, LV_SCALE_MODE_VERTICAL_RIGHT);
	lv_obj_set_size(OY_scaleLeft, 0, DISPLAY_CHART_HEIGHT - 4);
	lv_obj_set_style_pad_hor(OY_scaleLeft, lv_chart_get_first_point_center_offset(chart), 0);
	lv_obj_set_style_text_font(OY_scaleLeft, NULL, 0);
	lv_obj_align(OY_scaleLeft, LV_ALIGN_LEFT_MID, 0, 0);
	lv_scale_set_total_tick_count(OY_scaleLeft, 51);
	lv_scale_set_major_tick_every(OY_scaleLeft, 5);
	lv_obj_set_style_length(OY_scaleLeft, 4, LV_PART_ITEMS);
	lv_obj_set_style_length(OY_scaleLeft, 6, LV_PART_INDICATOR);
	lv_obj_set_style_line_width(OY_scaleLeft, 1, LV_PART_ITEMS);
	lv_obj_set_style_line_width(OY_scaleLeft, 1, LV_PART_INDICATOR);
	//lv_obj_set_style_line_color(OY_scale_1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_ITEMS);
	//lv_obj_set_style_line_color(OY_scale_1, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
	lv_obj_add_style(OY_scale_1, &style_axis, LV_PART_MAIN);

	////////////////////// chart RIGHT SIDE OY ////////////////////////
	lv_obj_t *OY_scaleRight = lv_scale_create(chart);
	lv_scale_set_mode(OY_scaleRight, LV_SCALE_MODE_VERTICAL_LEFT);
	lv_obj_set_size(OY_scaleRight, 0, DISPLAY_CHART_HEIGHT - 4);
	lv_obj_set_style_pad_hor(OY_scaleRight, lv_chart_get_first_point_center_offset(chart), 0);
	lv_obj_set_style_text_font(OY_scaleRight, NULL, 0);
	lv_obj_align(OY_scaleRight, LV_ALIGN_RIGHT_MID, 2, 0);
	lv_scale_set_total_tick_count(OY_scaleRight, 51);
	lv_scale_set_major_tick_every(OY_scaleRight, 5);
	lv_obj_set_style_length(OY_scaleRight, 4, LV_PART_ITEMS);
	lv_obj_set_style_length(OY_scaleRight, 6, LV_PART_INDICATOR);
	lv_obj_set_style_line_width(OY_scaleRight, 1, LV_PART_ITEMS);
	lv_obj_set_style_line_width(OY_scaleRight, 1, LV_PART_INDICATOR);
	//lv_obj_set_style_line_color(OY_scale_1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_ITEMS);
	//lv_obj_set_style_line_color(OY_scale_1, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
	lv_obj_add_style(OY_scale_1, &style_axis, LV_PART_MAIN);


	lv_obj_set_style_bg_opa(OX_scale_1, LV_OPA_TRANSP, LV_PART_MAIN);
	lv_obj_set_style_line_opa(OX_scale_1, LV_OPA_TRANSP, LV_PART_MAIN);

	lv_obj_set_style_bg_opa(OX_scale_2, LV_OPA_TRANSP, LV_PART_MAIN);
	lv_obj_set_style_line_opa(OX_scale_2, LV_OPA_TRANSP, LV_PART_MAIN);

	lv_obj_set_style_bg_opa(OY_scale_1, LV_OPA_TRANSP, LV_PART_MAIN);
	lv_obj_set_style_line_opa(OY_scale_1, LV_OPA_TRANSP, LV_PART_MAIN);

	lv_obj_set_style_bg_opa(OY_scale_2, LV_OPA_TRANSP, LV_PART_MAIN);
	lv_obj_set_style_line_opa(OY_scale_2, LV_OPA_TRANSP, LV_PART_MAIN);

	lv_obj_set_style_bg_opa(OX_scaleTop, LV_OPA_TRANSP, LV_PART_MAIN);
	lv_obj_set_style_line_opa(OX_scaleTop, LV_OPA_TRANSP, LV_PART_MAIN);

	lv_obj_set_style_bg_opa(OX_scaleBottom, LV_OPA_TRANSP, LV_PART_MAIN);
	lv_obj_set_style_line_opa(OX_scaleBottom, LV_OPA_TRANSP, LV_PART_MAIN);

	lv_obj_set_style_bg_opa(OY_scaleLeft, LV_OPA_TRANSP, LV_PART_MAIN);
	lv_obj_set_style_line_opa(OY_scaleLeft, LV_OPA_TRANSP, LV_PART_MAIN);

	lv_obj_set_style_bg_opa(OY_scaleRight, LV_OPA_TRANSP, LV_PART_MAIN);
	lv_obj_set_style_line_opa(OY_scaleRight, LV_OPA_TRANSP, LV_PART_MAIN);
}

void update_chart(lv_timer_t *timer)
{
	//@important -> DMA IRQ after copied one buffer is placed in stm32f746xx_it.c

	if(button_state_t.BUTTON_STOP == BUTTON_PRESSED) // RUN/STOP button pressed
		return;

	HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);

	if(!lv_obj_has_flag(math_windowBox, LV_OBJ_FLAG_HIDDEN))
		display_updateMathWindowBox();

	uint32_t *ADC_dataPtr = adc_get_current_buffer();
	lv_chart_set_ext_y_array(chart, ADC_dataSeries, (int32_t*)ADC_dataPtr);
    lv_chart_refresh(chart);
}

void display_createMathWindowBox(void)
{
	math_windowBox = lv_win_create(lv_screen_active());
	lv_obj_set_size(math_windowBox, DISPLAY_CHART_WIDTH - 16, 40);
	lv_obj_align(math_windowBox, LV_ALIGN_BOTTOM_MID, -38, -30);
	lv_win_add_title(math_windowBox, "Math tool");

	lv_obj_t *math_toolHeader = lv_win_get_header(math_windowBox);
	lv_obj_set_style_text_color(math_toolHeader, lv_color_hex(0xd6e32b), LV_PART_MAIN);
	lv_obj_set_style_bg_color(math_toolHeader, lv_color_hex(0x3e6ed6), LV_PART_MAIN);
	lv_obj_set_style_height(math_toolHeader, 15, LV_PART_MAIN);

	lv_obj_t *math_toolContent = lv_win_get_content(math_windowBox);
	lv_obj_set_style_text_color(math_toolContent, lv_color_hex(0xd6e32b), LV_PART_MAIN);
	lv_obj_set_style_bg_color(math_toolContent, lv_color_hex(0x5c6e96), LV_PART_MAIN);
	lv_obj_clear_flag(math_toolContent, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_style_pad_top(math_toolContent, 15, LV_PART_MAIN);
	lv_obj_set_style_pad_left(math_toolContent, 5, LV_PART_MAIN);

	label_maxVal = lv_label_create(math_toolContent);
	lv_obj_align(label_maxVal, LV_ALIGN_CENTER, -140, 0);

	label_minVal = lv_label_create(math_toolContent);
	lv_obj_align(label_minVal, LV_ALIGN_CENTER, -40, 0);

	lv_obj_add_flag(math_windowBox, LV_OBJ_FLAG_HIDDEN);
}

void display_updateMathWindowBox(void)
{
	uint32_t *ADC_dataPtr = adc_get_current_buffer();

	lv_label_set_text_fmt(label_maxVal, "Max[V]: %.3f", (float)array_get_max(ADC_dataPtr) * (float)ADC_RESOLUTION);
    lv_label_set_text_fmt(label_minVal, "Min[V]: %.3f", (float)array_get_min(ADC_dataPtr) * (float)ADC_RESOLUTION);
}

void display_toggleWindowBox(void *obj)
{
	 if(obj == NULL)
		 return;

	lv_obj_t *obj_t = (lv_obj_t *)obj;

	if (lv_obj_has_flag(obj_t, LV_OBJ_FLAG_HIDDEN))
		lv_obj_remove_flag(obj_t, LV_OBJ_FLAG_HIDDEN);

	else
		lv_obj_add_flag(obj_t, LV_OBJ_FLAG_HIDDEN);
}
