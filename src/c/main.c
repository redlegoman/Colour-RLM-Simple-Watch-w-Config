#include <pebble.h>
  
#define KEY_COLOR_RED     0
#define KEY_COLOR_GREEN   1
#define KEY_COLOR_BLUE    2
#define KEY_HIGH_CONTRAST 3
#define T_KEY_COLOR_RED     4
#define T_KEY_COLOR_GREEN   5
#define T_KEY_COLOR_BLUE    6
#define D_KEY_COLOR_RED     7
#define D_KEY_COLOR_GREEN   8
#define D_KEY_COLOR_BLUE    9
#define S_KEY_COLOR_RED     10
#define S_KEY_COLOR_GREEN   11
#define S_KEY_COLOR_BLUE    12
#define KEY_CONFIG_SET      13

  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_day_layer;
static TextLayer *s_date_layer;


static TextLayer *s_leftbar_layer;
static TextLayer *s_rightbar_layer;
static TextLayer *s_left_layer;
static TextLayer *s_right_layer;

/*
static bool gcolor_is_dark(GColor color) {
#if defined(PBL_BW)
  return gcolor_equal(color, GColorBlack);
#elif defined(PBL_COLOR)
  return color.r < 2 && color.g < 2 && color.b < 2;
#endif
}
*/




static void update_time(BatteryChargeState chargeState) {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";
  static char day[] = "Wednesday";
  static char date[] = "00 MTH";
  static char secs[] = "00";

  static char percent_show[] = "00 %";
  uint8_t percent = chargeState.charge_percent;
  snprintf(percent_show, 5, "%i%%", percent);
  //snprintf(percent_show, sizeof(battery_text), "%d%% charged", chargeState.charge_percent);

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "percent: %s", percent_show);
//  text_layer_set_text(s_left_layer, percent_show);

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
    //strftime(buffer, sizeof("44:44"), "00:00", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  strftime(day, sizeof("Wednesdayx"), "%A", tick_time);
  strftime(date, sizeof("00 MTH"), "%e %b", tick_time);
  strftime(secs, sizeof("00"), "%S", tick_time);
  
  //text_layer_set_text(s_date_layer, "00 MTH");
   
  
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
    text_layer_set_text(s_day_layer, day);
    text_layer_set_text(s_date_layer, date);
    text_layer_set_text(s_right_layer, secs);
    text_layer_set_text_alignment(s_right_layer, GTextAlignmentRight);

  text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);
  text_layer_set_text(s_left_layer, percent_show);

}
static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  // High contrast selected?
  Tuple *high_contrast_t = dict_find(iter, KEY_HIGH_CONTRAST);
  if(high_contrast_t && high_contrast_t->value->int8 > 0) {  // Read boolean as an integer
    // Change color scheme
    window_set_background_color(s_main_window, GColorBlack);
    //text_layer_set_text_color(s_text_layer, GColorWhite);

    // Persist value
    persist_write_bool(KEY_HIGH_CONTRAST, true);
  } else {
    persist_write_bool(KEY_HIGH_CONTRAST, false);
  }

  // Color scheme?
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
  //**
  Tuple *config_set_t = dict_find(iter, KEY_CONFIG_SET);
  //**

  
  if(color_red_t && color_green_t && color_blue_t) {
    // Apply the color if available
#if defined(PBL_BW)
    window_set_background_color(s_main_window, GColorBlack);
    //text_layer_set_text_color(s_text_layer, GColorBlack);
#elif defined(PBL_COLOR)
    int red = color_red_t->value->int32;
    int green = color_green_t->value->int32;
    int blue = color_blue_t->value->int32;
    int t_red = t_color_red_t->value->int32;
    int t_green = t_color_green_t->value->int32;
    int t_blue = t_color_blue_t->value->int32;
    int d_red = d_color_red_t->value->int32;
    int d_green = d_color_green_t->value->int32;
    int d_blue = d_color_blue_t->value->int32;
    int s_red = s_color_red_t->value->int32;
    int s_green = s_color_green_t->value->int32;
    int s_blue = s_color_blue_t->value->int32;
    //**
    int config_set = config_set_t->value->int32;
    //**
     APP_LOG(APP_LOG_LEVEL_DEBUG, " ** inbox_proc: config_set =  %d", config_set);

    // Persist values
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
    //**
    persist_write_int(KEY_CONFIG_SET, config_set);
    //**


    GColor bg_color = GColorFromRGB(red, green, blue);
    GColor t_color = GColorFromRGB(t_red, t_green, t_blue);
    GColor d_color = GColorFromRGB(d_red, d_green, d_blue);
    GColor s_color = GColorFromRGB(s_red, s_green, s_blue);
    window_set_background_color(s_main_window, bg_color);
    text_layer_set_text_color(s_time_layer, t_color);
    text_layer_set_text_color(s_day_layer, d_color);
    text_layer_set_text_color(s_date_layer, d_color);
    text_layer_set_text_color(s_right_layer, s_color);



    //text_layer_set_text_color(s_text_layer, gcolor_is_dark(bg_color) ? GColorWhite : GColorBlack);
#endif
  }
}


static void main_window_load(Window *window) {
  
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  static GFont s_time_font;
  static GFont s_day_font;
  //**
   //APP_LOG(APP_LOG_LEVEL_DEBUG, "Loop index now %d", i);
   APP_LOG(APP_LOG_LEVEL_DEBUG, " ** main_window_load **");
  //**


//  s_day_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_RENEGADO_14));
  s_day_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SARA_27));

  //s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_RENEGADO_39));
//  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SARA_53));
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_68));
//  GFont custom_font = fonts_load_custom_font (resource_get_handle(RESOURCE_ID_FONT_OSP_DIN_44));

  
  //day
  s_day_layer = text_layer_create(GRect(0, 15, 144, 35));
  text_layer_set_background_color(s_day_layer, GColorClear);
  text_layer_set_text_color(s_day_layer, GColorPictonBlue);
  text_layer_set_text(s_day_layer, "Thisaday");
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 43, 144, 69));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  //date       from left, from top, size from left, size from top
  //s_date_layer = text_layer_create(GRect(60, 104, 84, 40));
  s_date_layer = text_layer_create(GRect(0, 109, 144, 30));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorPictonBlue);
  text_layer_set_text(s_date_layer, "00 MTH");
   text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);
  

  

 //------  NOT NEEDED?? -------------------------------------------
  s_leftbar_layer = text_layer_create(GRect(0, 157, 72, 11));
  text_layer_set_background_color(s_leftbar_layer, GColorWhite);
  text_layer_set_text_color(s_leftbar_layer, GColorBlack);
  

  s_rightbar_layer = text_layer_create(GRect(72, 157, 144, 11));
  text_layer_set_background_color(s_rightbar_layer, GColorWhite);
  text_layer_set_text_color(s_rightbar_layer, GColorBlack);
//------  NOT NEEDED??----------------------------------------------
  
  
  //percent       from left, from top, size from left, size from top
  #ifdef PBL_COLOR
    s_left_layer = text_layer_create(GRect(0, 149, 52, 20));
  #else
    s_left_layer = text_layer_create(GRect(0, 153, 52, 20));
  #endif
  text_layer_set_background_color(s_left_layer, GColorClear);
  text_layer_set_text_color(s_left_layer, GColorDarkGray);

  text_layer_set_text(s_left_layer, "00 %");

  //seconds       from left, from top, size from left, size from top
  #ifdef PBL_COLOR
    s_right_layer = text_layer_create(GRect(123, 149, 20, 20)); /// CHECK DIMENSIONS
  #else
    s_right_layer = text_layer_create(GRect(123, 153, 20, 20)); /// CHECK DIMENSIONS
  #endif
  text_layer_set_background_color(s_right_layer, GColorClear);
  text_layer_set_text_color(s_right_layer, GColorDarkCandyAppleRed);

  text_layer_set_text(s_right_layer, "00");  /// SECONDS

  
  // Improve the layout to be more like a watchface
  //text_layer_set_font(s_time_layer, fonts_get_system_font(s_time_font));
  text_layer_set_font(s_time_layer, s_time_font);

  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
//  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_font(s_date_layer, s_day_font);
  
//  text_layer_set_font(s_day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_font(s_day_layer, s_day_font);
  
  
  
  text_layer_set_font(s_left_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));


  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
//  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_leftbar_layer));
//  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_rightbar_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_left_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_right_layer));
  

//####################################################################################
    // Color scheme?
    int red = persist_read_int(KEY_COLOR_RED);
    int green = persist_read_int(KEY_COLOR_GREEN);
    int blue = persist_read_int(KEY_COLOR_BLUE);
    int t_red = persist_read_int(T_KEY_COLOR_RED);
    int t_green = persist_read_int(T_KEY_COLOR_GREEN);
    int t_blue = persist_read_int(T_KEY_COLOR_BLUE);
    int d_red = persist_read_int(D_KEY_COLOR_RED);
    int d_green = persist_read_int(D_KEY_COLOR_GREEN);
    int d_blue = persist_read_int(D_KEY_COLOR_BLUE);
    int s_red = persist_read_int(S_KEY_COLOR_RED);
    int s_green = persist_read_int(S_KEY_COLOR_GREEN);
    int s_blue = persist_read_int(S_KEY_COLOR_BLUE);

    int config_set = persist_read_int(KEY_CONFIG_SET); 
  
    GColor bg_color = GColorFromRGB(red, green, blue);
    GColor t_color = GColorFromRGB(t_red, t_green, t_blue);
    GColor d_color = GColorFromRGB(d_red, d_green, d_blue);
    GColor s_color = GColorFromRGB(s_red, s_green, s_blue);
  
    APP_LOG(APP_LOG_LEVEL_DEBUG, " ** main_window_load : config_set  %d", config_set);

  /*
  if(
    persist_read_int(KEY_COLOR_RED) || 
    persist_read_int(T_KEY_COLOR_RED) || 
    persist_read_int(D_KEY_COLOR_RED) || 
    persist_read_int(S_KEY_COLOR_RED)  
    )
    */
  if (config_set == 1)
  {
    window_set_background_color(s_main_window, bg_color);
    text_layer_set_text_color(s_time_layer, t_color);
    text_layer_set_text_color(s_day_layer, d_color);
    text_layer_set_text_color(s_date_layer, d_color);
//    text_layer_set_text_color(s_right_layer, s_color);

  }else{
    window_set_background_color(s_main_window, GColorBlack);
    text_layer_set_text_color(s_time_layer, GColorWhite);
    text_layer_set_text_color(s_day_layer, GColorPictonBlue);
    text_layer_set_text_color(s_date_layer, GColorPictonBlue);
   // text_layer_set_text_color(s_right_layer, GColorWhite);
}
  text_layer_set_text_color(s_right_layer, GColorWhite);
  text_layer_set_text_color(s_left_layer, GColorWhite);
  
//####################################################################################
  
  
  
  
  // Make sure the time is displayed from the start
  update_time(battery_state_service_peek());
}


static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_date_layer);

  
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(battery_state_service_peek());
}
  
static void init() {
  
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch
  //window_set_background_color(s_main_window, GColorWhite);

  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
  
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
