#!/usr/bin/env node

var util = require("util"),
    mediainfo = require("./");

mediainfo("test1.mp3", "test2.mp3", function(err, res) {
  if (err) {
    return console.log(err);
  }

  console.log(util.inspect(res, null, null, true));
});
