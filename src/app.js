Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://cdn.rawgit.com/redlegoman/Colour-RLM-Simple-Watch-w-Config/master/web/index.html';
  
  /*
  
  Above URL is on github (https://github.com/redlegoman/Colour-RLM-Simple-Watch-w-Config) and RELIES
  ON IT BEING THERE - DO NOT DELETE IT!
  
  */
  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

  var backgroundColor = configData['background_color'];
  var timeColor = configData['time_color'];
  var dateColor = configData['date_color'];
  var secsColor = configData['secs_color'];

  var dict = {};
  //if(configData['high_contrast'] === true) {
    //dict['KEY_HIGH_CONTRAST'] = configData['high_contrast'] ? 1 : 0;  // Send a boolean as an integer
  //} else {
    dict['KEY_COLOR_RED'] = parseInt(backgroundColor.substring(2, 4), 16);
    dict['KEY_COLOR_GREEN'] = parseInt(backgroundColor.substring(4, 6), 16);
    dict['KEY_COLOR_BLUE'] = parseInt(backgroundColor.substring(6), 16);
    
    dict['T_KEY_COLOR_RED'] = parseInt(timeColor.substring(2, 4), 16);
    dict['T_KEY_COLOR_GREEN'] = parseInt(timeColor.substring(4, 6), 16);
    dict['T_KEY_COLOR_BLUE'] = parseInt(timeColor.substring(6), 16);
    
    dict['D_KEY_COLOR_RED'] = parseInt(dateColor.substring(2, 4), 16);
    dict['D_KEY_COLOR_GREEN'] = parseInt(dateColor.substring(4, 6), 16);
    dict['D_KEY_COLOR_BLUE'] = parseInt(dateColor.substring(6), 16);
    
    dict['S_KEY_COLOR_RED'] = parseInt(secsColor.substring(2, 4), 16);
    dict['S_KEY_COLOR_GREEN'] = parseInt(secsColor.substring(4, 6), 16);
    dict['S_KEY_COLOR_BLUE'] = parseInt(secsColor.substring(6), 16);
    
    
    
  //}

  // Send to watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function() {
    console.log('Send failed!');
  });
});