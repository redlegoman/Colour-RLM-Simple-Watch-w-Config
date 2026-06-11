Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://redlegoman.github.io/Colour-RLM-Simple-Watch-w-Config/testindex.html?2.2';
  
  // Retrieve saved settings from the phone's local storage
  var savedConfig = localStorage.getItem('rlm_watch_config');
  if (savedConfig) {
    try {
      var configData = JSON.parse(savedConfig);
      var params = [];
      for (var key in configData) {
        params.push(encodeURIComponent(key) + '=' + encodeURIComponent(configData[key]));
      }
      // Append saved settings to the URL so the HTML page can read them
      url += '?' + params.join('&');
    } catch(e) {
      console.log('Error parsing saved config');
    }
  }

  console.log('Showing configuration page: ' + url);
  Pebble.openURL(url);
});

// Safely extract RGB regardless of how the HTML formats the string/number
function parseColor(colorValue) {
  if (colorValue === undefined || colorValue === null || colorValue === '') {
    return {r: 0, g: 0, b: 0};
  }
  
  var hex;
  if (typeof colorValue === 'number') {
    hex = colorValue.toString(16);
  } else {
    hex = colorValue.toString().replace('#', '').replace('0x', '');
  }
  
  while (hex.length < 6) {
    hex = '0' + hex;
  }
  
  return {
    r: parseInt(hex.substring(0, 2), 16) || 0,
    g: parseInt(hex.substring(2, 4), 16) || 0,
    b: parseInt(hex.substring(4, 6), 16) || 0
  };
}

Pebble.addEventListener('webviewclosed', function(e) {
  if (!e.response || e.response === "CANCELLED" || e.response === "{}") {
    console.log('Configuration cancelled');
    return;
  }

  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

  // Save the new settings to local storage so they are remembered next time
  localStorage.setItem('rlm_watch_config', JSON.stringify(configData));

  var bg = parseColor(configData['background_color']);
  var time = parseColor(configData['time_color']);
  var date = parseColor(configData['date_color']);
  var secs = parseColor(configData['secs_color']); 
  
  var dict = {};
  
  dict['KEY_COLOR_RED'] = bg.r;
  dict['KEY_COLOR_GREEN'] = bg.g;
  dict['KEY_COLOR_BLUE'] = bg.b;
  
  dict['T_KEY_COLOR_RED'] = time.r;
  dict['T_KEY_COLOR_GREEN'] = time.g;
  dict['T_KEY_COLOR_BLUE'] = time.b;
  
  dict['D_KEY_COLOR_RED'] = date.r;
  dict['D_KEY_COLOR_GREEN'] = date.g;
  dict['D_KEY_COLOR_BLUE'] = date.b;
  
  dict['S_KEY_COLOR_RED'] = secs.r;
  dict['S_KEY_COLOR_GREEN'] = secs.g;
  dict['S_KEY_COLOR_BLUE'] = secs.b;
  
  dict['KEY_CONFIG_SET'] = parseInt(configData['config_set']) || 1;
  dict['KEY_SHOW_SECS'] = configData['show_secs'] ? 1 : 0;
  
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful.');
  }, function(error) {
    console.log('Send failed: ' + JSON.stringify(error));
  });
});