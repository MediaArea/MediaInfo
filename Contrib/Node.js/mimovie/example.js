#!/usr/bin/env node

var util = require("util"),
    mimovie = require("./");

mimovie("./test/movie.m4v", function(err, res) {
  if (err) {
    return console.log(err);
  }
  
  console.log(util.inspect(res, null, null, true));

});
