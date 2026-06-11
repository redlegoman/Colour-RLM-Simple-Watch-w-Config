#include <pebble.h>

#define KEY_COLOR_RED 0
#define KEY_COLOR_GREEN 1
#define KEY_COLOR_BLUE 2
#define KEY_HIGH_CONTRAST 3
#define T_KEY_COLOR_RED 4
#define T_KEY_COLOR_GREEN 5
#define T_KEY_COLOR_BLUE 6
#define D_KEY_COLOR_RED 7
#define D_KEY_COLOR_GREEN 8
#define D_KEY_COLOR_BLUE 9
#define S_KEY_COLOR_RED 10
#define S_KEY_COLOR_GREEN 11
#define S_KEY_COLOR_BLUE 12
#define KEY_CONFIG_SET 13
#define KEY_SHOW_SECS 14

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_day_layer;
static TextLayer *s_date_layer;
static TextLayer *s_left_layer;
static TextLayer *s_right_layer;

static GFont s_time_font;
static GFont s_day_font;

static bool s_show_seconds = true;

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  BatteryChargeState chargeState = battery_state_service_peek();

  static char buffer[] = "00:00";
  static char day[] = "Wednesday";
  static char date[] = "00 MTH";
  static char secs[] = "00";
  static char percent_show[] = "100%";

  snprintf(percent_show, sizeof(percent_show), "%i%%",
           chargeState.charge_percent);

  if (clock_is_24h_style()) {
    strftime(buffer, sizeof(buffer), "%H:%M", tick_time);
  } else {
    strftime(buffer, sizeof(buffer), "%I:%M", tick_time);
  }

  strftime(day, sizeof(day), "%A", tick_time);
  strftime(date, sizeof(date), "%e %b", tick_time);

  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_day_layer, day);
  text_layer_set_text(s_date_layer, date);
  text_layer_set_text(s_left_layer, percent_show);

  if (s_show_seconds) {
    strftime(secs, sizeof(secs), "%S", tick_time);
    text_layer_set_text(s_right_layer, secs);
  } else {
    text_layer_set_text(s_right_layer, "");
  }

  text_layer_set_text(s_right_layer, "");
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void toggle_seconds_service() {
  tick_timer_service_unsubscribe();
  if (s_show_seconds) {
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  } else {
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  }
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *high_contrast_t = dict_find(iter, KEY_HIGH_CONTRAST);
  if (high_contrast_t && high_contrast_t->value->int8 > 0) {
    window_set_background_color(s_main_window, GColorBlack);
    persist_write_bool(KEY_HIGH_CONTRAST, true);
  } else {
    persist_write_bool(KEY_HIGH_CONTRAST, false);
  }

  Tuple *show_seconds_t = dict_find(iter, KEY_SHOW_SECS);
  if (show_seconds_t) {
    s_show_seconds = show_seconds_t->value->int32 == 1;
    persist_write_bool(KEY_SHOW_SECS, s_show_seconds);
    toggle_seconds_service();
  }

  Tuple *color_red_t = dict_find(iter, KEY_COLOR_RED);
  Tuple *color_green_t = dict_find(iter, KEY_COLOR_GREEN);
  Tuple *color_blue_t = dict_find(iter, KEY_COLOR_BLUE);

  Tuple *t_color_red_t = dict_find(iter, T_KEY_COLOR_RED);
  Tuple *t_color_green_t = dict_find(iter, T_KEY_COLOR_GREEN);
  Tuple *t_color_blue_t = dict_find(iter, T_KEY_COLOR_BLUE);

  Tuple *d_color_red_t = dict_find(iter, D_KEY_COLOR_RED);
  Tuple *d_color_green_t = dict_find(iter, D_KEY_COLOR_GREEN);
  Tuple *d_color_blue_t = dict_find(iter, D_KEY_COLOR_BLUE);

  Tuple *s_color_red_t = dict_find(iter, S_KEY_COLOR_RED);
  Tuple *s_color_green_t = dict_find(iter, S_KEY_COLOR_GREEN);
  Tuple *s_color_blue_t = dict_find(iter, S_KEY_COLOR_BLUE);

  Tuple *config_set_t = dict_find(iter, KEY_CONFIG_SET);

  if (color_red_t && color_green_t && color_blue_t) {
#if defined(PBL_COLOR)
    int red = color_red_t->value->int32;
    int green = color_green_t->value->int32;
    int blue = color_blue_t->value->int32;
    int t_red = t_color_red_t->value->int32;
    int t_green = t_color_green_t->value->int32;
    int t_blue = t_color_blue_t->value->int32;
    int d_red = d_color_red_t->value->int32;
    int d_green = d_color_green_t->value->int32;
    int d_blue = d_color_blue_t->value->int32;
    int s_red = s_color_red_t ? s_color_red_t->value->int32 : 0;
    int s_green = s_color_green_t ? s_color_green_t->value->int32 : 0;
    int s_blue = s_color_blue_t ? s_color_blue_t->value->int32 : 0;
    int config_set = config_set_t->value->int32;

    persist_write_int(KEY_COLOR_RED, red);
    persist_write_int(KEY_COLOR_GREEN, green);
    persist_write_int(KEY_COLOR_BLUE, blue);
    persist_write_int(T_KEY_COLOR_RED, t_red);
    persist_write_int(T_KEY_COLOR_GREEN, t_green);
    persist_write_int(T_KEY_COLOR_BLUE, t_blue);
    persist_write_int(D_KEY_COLOR_RED, d_red);
    persist_write_int(D_KEY_COLOR_GREEN, d_green);
    persist_write_int(D_KEY_COLOR_BLUE, d_blue);
    persist_write_int(S_KEY_COLOR_RED, s_red);
    persist_write_int(S_KEY_COLOR_GREEN, s_green);
    persist_write_int(S_KEY_COLOR_BLUE, s_blue);
    persist_write_int(KEY_CONFIG_SET, config_set);

    if (config_set == 1) {
      window_set_background_color(s_main_window,
                                  GColorFromRGB(red, green, blue));
      text_layer_set_text_color(s_time_layer,
                                GColorFromRGB(t_red, t_green, t_blue));
      text_layer_set_text_color(s_day_layer,
                                GColorFromRGB(d_red, d_green, d_blue));
      text_layer_set_text_color(s_date_layer,
                                GColorFromRGB(d_red, d_green, d_blue));
      text_layer_set_text_color(s_right_layer,
                                GColorFromRGB(s_red, s_green, s_blue));
    }
#elif defined(PBL_BW)
    window_set_background_color(s_main_window, GColorBlack);
#endif
  }
  update_time();
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  if (persist_exists(KEY_SHOW_SECS)) {
    s_show_seconds = persist_read_bool(KEY_SHOW_SECS);
  }

  s_day_font =
      fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SARA_27));
  s_time_font =
      fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_80));

  // Time Layer - Center target anchor point
  // Adjust time_y slightly higher if needed to keep it centered
  int time_y = (bounds.size.h / 2) - 45;
  // Increase the height from 69 to accommodate the larger font (e.g., 85)
  s_time_layer = text_layer_create(GRect(0, time_y, bounds.size.w, 85));

  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  // Day Layer - Positioned ABOVE the time, aligned LEFT
  int day_y = time_y - 28;
  s_day_layer = text_layer_create(GRect(4, day_y, bounds.size.w - 8, 35));
  text_layer_set_background_color(s_day_layer, GColorClear);
  text_layer_set_font(s_day_layer, s_day_font);
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(s_day_layer));

  // Date Layer - Positioned BELOW the time, aligned RIGHT
  int date_y = time_y + 64;
  s_date_layer = text_layer_create(GRect(4, date_y, bounds.size.w - 8, 35));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_font(s_date_layer, s_day_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

  // Status metrics pinned to the bottom hardware boundary
  int status_y = bounds.size.h - 22;
  s_left_layer = text_layer_create(GRect(4, status_y, 60, 20));
  text_layer_set_background_color(s_left_layer, GColorClear);
  text_layer_set_font(s_left_layer,
                      fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_left_layer));

  s_right_layer =
      text_layer_create(GRect(bounds.size.w - 64, status_y, 60, 20));
  text_layer_set_background_color(s_right_layer, GColorClear);
  text_layer_set_text_alignment(s_right_layer, GTextAlignmentRight);
  text_layer_set_font(s_right_layer,
                      fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_right_layer));

  if (persist_read_int(KEY_CONFIG_SET) == 1) {
    window_set_background_color(
        s_main_window, GColorFromRGB(persist_read_int(KEY_COLOR_RED),
                                     persist_read_int(KEY_COLOR_GREEN),
                                     persist_read_int(KEY_COLOR_BLUE)));
    text_layer_set_text_color(
        s_time_layer, GColorFromRGB(persist_read_int(T_KEY_COLOR_RED),
                                    persist_read_int(T_KEY_COLOR_GREEN),
                                    persist_read_int(T_KEY_COLOR_BLUE)));
    text_layer_set_text_color(
        s_day_layer, GColorFromRGB(persist_read_int(D_KEY_COLOR_RED),
                                   persist_read_int(D_KEY_COLOR_GREEN),
                                   persist_read_int(D_KEY_COLOR_BLUE)));
    text_layer_set_text_color(
        s_date_layer, GColorFromRGB(persist_read_int(D_KEY_COLOR_RED),
                                    persist_read_int(D_KEY_COLOR_GREEN),
                                    persist_read_int(D_KEY_COLOR_BLUE)));
    text_layer_set_text_color(
        s_right_layer, GColorFromRGB(persist_read_int(S_KEY_COLOR_RED),
                                     persist_read_int(S_KEY_COLOR_GREEN),
                                     persist_read_int(S_KEY_COLOR_BLUE)));
  } else {
    window_set_background_color(s_main_window, GColorBlack);
    text_layer_set_text_color(s_time_layer, GColorWhite);
    text_layer_set_text_color(s_day_layer, GColorPictonBlue);
    text_layer_set_text_color(s_date_layer, GColorPictonBlue);
    text_layer_set_text_color(s_right_layer, GColorWhite);
  }
  text_layer_set_text_color(s_left_layer, GColorWhite);

  toggle_seconds_service();
  update_time();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_left_layer);
  text_layer_destroy(s_right_layer);
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_day_font);
}

static void init() {
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(),
                   app_message_outbox_size_maximum());
  s_main_window = window_create();
  window_set_window_handlers(
      s_main_window,
      (WindowHandlers){.load = main_window_load, .unload = main_window_unload});
  window_stack_push(s_main_window, true);
}

static void deinit() {
  tick_timer_service_unsubscribe();
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}