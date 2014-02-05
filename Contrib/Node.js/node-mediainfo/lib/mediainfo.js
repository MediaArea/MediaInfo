var child_process = require("child_process"),
    filesizeParser = require("filesize-parser"),
    expat = require("node-expat");

module.exports = function() {
  var files = Array.prototype.slice.apply(arguments);

  var done = files.pop();

  child_process.execFile("mediainfo", ["--Output=XML"].concat(files), function(err, stdout, stderr) {
    if (err) {
      return done(err);
    }

    var files = [],
        file = null,
        track = null,
        key = null;

    var parser = new expat.Parser();

    parser.on("startElement", function(name, attribs) {
      name = name.toLowerCase();

      if (file === null && name === "file") {
        file = {tracks: []};

        for (var k in attribs) {
          file[k.toLowerCase()] = attribs[k];
        }

        return;
      }

      if (track === null && name === "track") {
        if (attribs.type === "General") {
          track = file;
        } else {
          track = {};

          for (var k in attribs) {
            track[k.toLowerCase()] = attribs[k];
          }
        }

        return;
      }

      if (track !== null) {
        key = name;
      }
    });

    parser.on("endElement", function(name) {
      name = name.toLowerCase();

      if (track !== null && name === "track") {
        if (track !== file) { file.tracks.push(track); }
        track = null;
      }

      if (file !== null && name === "file") {
        if (file.file_size) {
          file.file_size_bytes = filesizeParser(file.file_size);
        }

        files.push(file);

        file = null;
      }

      key = null;
    });

    parser.on("text", function(text) {
      if (track !== null && key !== null) {
        track[key] = (track[key] || "") + text;
      }
    });

    if (!parser.parse(stdout)) {
      return done(Error(parser.getError()));
    }

    return done(null, files);
  });
};
