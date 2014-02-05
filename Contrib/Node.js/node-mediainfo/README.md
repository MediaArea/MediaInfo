Mediainfo
=========

Wrapper around the `mediainfo` command for obtaining information about media
files.

Overview
--------

This module provides an interface to the `mediainfo` command. It returns the
(parsed) output of the command with all the keys lower-cased. There's not a lot
to say here.

Usage
-----

The module exposes just one function, which has a signature of:

`mediainfo("/path/to/file", "/path/to/other/file", [...], callback)`

The callback function is called, in familiar node fashion, with two arguments.
The first is an Error object on error, or null on success. The second argument
is an array of objects, forming what you'd expect from the output of mediainfo,
if it were mangled into a JSON object.

It's really easier to just see the output, so here:

```
[
  {
    tracks: [
      {
        type: 'Audio',
        format: 'MPEG Audio',
        format_version: 'Version 1',
        format_profile: 'Layer 3',
        mode: 'Joint stereo',
        duration: '3mn 2s',
        bit_rate_mode: 'Constant',
        bit_rate: '320 Kbps',
        channel_s_: '2 channels',
        sampling_rate: '44.1 KHz',
        compression_mode: 'Lossy',
        stream_size: '6.97 MiB (100%)',
        writing_library: 'LAME3.97'
      }
    ],
    complete_name: 'test1.mp3',
    format: 'MPEG Audio',
    file_size: '6.98 MiB',
    duration: '3mn 2s',
    overall_bit_rate: '320 Kbps',
    album: 'We No Speak Americano (High Rankin\'s Bait n Wobble Mix)',
    track_name: 'We No Speak Americano (High Rankin\'s Bait n Wobble Mix)',
    track_name_position: '1',
    track_name_total: '1',
    performer: 'Yolanda Be Cool & DCUP',
    accompaniment: 'High Rankin',
    genre: 'Dubstep',
    recorded_date: '2010',
    writing_library: 'LAME in FL Studio 8 / LAME3.97'
  },
  {
    tracks: [
      {
        type: 'Audio',
        format: 'MPEG Audio',
        format_version: 'Version 1',
        format_profile: 'Layer 3',
        mode: 'Joint stereo',
        duration: '3mn 2s',
        bit_rate_mode: 'Constant',
        bit_rate: '320 Kbps',
        channel_s_: '2 channels',
        sampling_rate: '44.1 KHz',
        compression_mode: 'Lossy',
        stream_size: '6.97 MiB (100%)',
        writing_library: 'LAME3.97'
      }
    ],
    complete_name: 'test2.mp3',
    format: 'MPEG Audio',
    file_size: '6.98 MiB',
    duration: '3mn 2s',
    overall_bit_rate: '320 Kbps',
    album: 'We No Speak Americano (High Rankin\'s Bait n Wobble Mix)',
    track_name: 'We No Speak Americano (High Rankin\'s Bait n Wobble Mix)',
    track_name_position: '1',
    track_name_total: '1',
    performer: 'Yolanda Be Cool & DCUP',
    accompaniment: 'High Rankin',
    genre: 'Dubstep',
    recorded_date: '2010',
    writing_library: 'LAME in FL Studio 8 / LAME3.97'
  }
]
```

(bite me, it's a decent track)

Example
-------

```javascript
var mediainfo = require("mediainfo");

mediainfo("/path/to/something.mp3", function(err, res) {
  if (err) {
    return console.log(err);
  }

  console.log(res);
});
```

Requirements
------------

The `mediainfo` command has to be available somewhere in the PATH of the user
node is running as.

License
-------

3-clause BSD. A copy is included with the source.

Contact
-------

* GitHub ([deoxxa](http://github.com/deoxxa))
* Twitter ([@deoxxa](http://twitter.com/deoxxa))
* Email ([deoxxa@fknsrs.biz](mailto:deoxxa@fknsrs.biz))
