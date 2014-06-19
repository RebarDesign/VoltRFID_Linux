var util = require('util'),
    spawn = require('child_process').spawn,
    exec = require('child_process').exec,
    child;

var listener;

var children = [];
var port = 3700;

var io = require('socket.io').listen(port);

io.sockets.on('connection', function(socket)
{
  
  socket.on('stop', function () {
    child.kill('SIGINT');
    for(var i = 0; i<children.length; i++) 
    {  
        process.kill(Number(children[i]), 'SIGINT');
    }
  }); // end on stop
  
  socket.on('start', function () {
    child = spawn('./rfid-reader', []);	
  }); // end on start


}); // end on connection

var net = require('net');
var server = net.createServer(function(c) { //'connection' listener
  c.on('end', function() {
    console.log('client disconnected');
  });
  c.on('data', function(da){
	var input = da.toString().split('\n'); 
	console.log(da.toString());
	var i = 0;
	var reader, uid;
        for(i = 0; i<input.length; i++)
        {
	   if(input[i].indexOf("number") != -1) //found reader line
	   {
		//split line for finding the number
		var temp = input[i].split(':');
		// charAt(1) to avoid space
		reader = temp[1].charAt(1);
	   }
	   else if(input[i].indexOf("UID") != -1)
	   {
		var temp = input[i].split(':');
		uid = temp[1].split(" ").join("");
	   }
	   else if(input[i].indexOf("Fork") != -1)
	   {

		var temp = input[i].split(':');
		children.push(temp[1]);
	   }
	}

        if(typeof uid !== "undefined" && typeof reader !== "undefined") 
	{
	  io.sockets.emit('data', { UID: uid, reader: reader });
	  console.log('data', {UID : uid, reader: reader });
        }
  
  });

});
server.listen(8124, function() { //'listening' listener
  console.log('server bound');
  child = spawn('./rfid-reader', []);
});

