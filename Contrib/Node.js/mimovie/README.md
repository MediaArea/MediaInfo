mimovie
=======

Wrapper around the `mediainfo' for obtaining information about movie and video files

Overview
--------

Inspired by [node-mediainfo](https://github.com/deoxxa/node-mediainfo) this module provides an interface to the `mediainfo` command, it returns the common properties of a video or movie file in a JSON object.

Install
--------

```
npm install mimovie
```

Usage
-----

The module exposes just one function, which has a signature of:

`mimovie("/path/to/video_file", callback)`

The callback function is called, in familiar node fashion, with two arguments.
The first is an Error object on error, or null on success. The second argument is an object with all the video/movie information as a JSON object.

It's really easier to just see the output, so here:

```javascript
{
    video_tracks: [{
        width: 1280,
        height: 720,
        codec: 'AVC',
        fps: 23.976,
        bitrate: 912222,
        profile: 'High@L4.0',
        aspect: '16:9'
    }],
    audio_tracks: [{
        ch: 2,
        ch_pos: 'Front: L R',
        sammple_rate: 48000,
        codec: 'AAC LC',
        bitrate: 163836,
        bitrate_mode: 'VBR',
        lang: 'en'
    }, {
        ch: 6,
        ch_pos: 'Front: L C R, Side: L R, LFE',
        sammple_rate: 48000,
        codec: 'AC3',
        bitrate: 448000,
        bitrate_mode: 'CBR',
        lang: 'en'
    }],
    subtitles: ['es'],
    bitrate: 1533377,
    path: './test/movie.m4v',
    size: 878363651,
    duration: 4582635,
    menu: false
}
```

Example
-------

```javascript
var mimovie = require("mimovie");

mimovie("/path/to/movie.m4v", function(err, res) {
  if (err) {
    return console.log(err);
  }

  console.log(res);
});
```

Requirements
------------

The `mediainfo` command has to be available somewhere in the PATH of the user node is running as.

License
-------

(The MIT License)

Copyright (c) by Rodrigo Polo http://rodrigopolo.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Contact
-------

* GitHub ([rodrigopolo](http://github.com/rodrigopolo/))
* Twitter ([@rodrigopolo](http://twitter.com/rodrigopolo))
