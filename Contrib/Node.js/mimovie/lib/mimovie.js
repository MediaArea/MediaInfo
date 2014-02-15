var	child_process = require("child_process");

 module.exports = function(file, done){

	var MiB = 1048576;
	var video = {};
	video.video_tracks=[];
	video.audio_tracks=[];
	video.subtitles=[];

	var vvt;
	var vat;

	// prevent errors using windows paths
	file = file.replace(/\\/g, '/');

	child_process.execFile("mediainfo", [
		'--Inform=file://'+__dirname+'/media_json.txt'.replace(/\\/g, '/'),
		file
	],{ 
	  encoding: 'utf8',
	  maxBuffer: 1*MiB
	}, function(err, stdout, stderr) {



		if (err) {
			return done(err,null);
		}else{
			
			var mediainfo;
			var error = false;

			try {
				mediainfo = JSON.parse(stdout);
			}
			catch (e) {
				// console.log('FILE: '+file);
				// console.log(stdout);
				// console.log(e);
				// process.exit(1);
				error = true;
				done(e,null);
			}
			if(!error){
				
				// var util = require("util");
				// console.log(util.inspect(mediainfo, null, null, true));

				video.bitrate = 		mediainfo.General.bitrate;
				video.path = 			mediainfo.General.path;
				video.size = 			mediainfo.General.size;
				video.duration = 		mediainfo.General.duration;
				video.video_tracks = 	mediainfo.Video;
				video.audio_tracks = 	mediainfo.Audio;
				video.subtitles = 		mediainfo.Subs;
				video.menu =		 	mediainfo.General.menu
				done(null,video);
			}
		}
	});
};
