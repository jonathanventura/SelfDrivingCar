
// Blynk setup
var Blynk = require('blynk-library');

// cloud setup 
//var AUTH = '94707690fc7a4e04b44894c7f68d9742';
//var server = 'cloud.blynk.cc';
//var portname = '/dev/cu.usbmodem1421';

// RPi setup
var AUTH = '61edbe8ab7d449648c83fc0c46e8980f';
var server = 'localhost';
var portname = '/dev/ttyACM0';

var blynk = new Blynk.Blynk(AUTH, options = {
  connector : new Blynk.TcpClient( options = {addr:server} )
});

blynk.on('connect', function() { console.log("Blynk ready."); });
blynk.on('disconnect', function() { console.log("DISCONNECT"); });



// serial port setup
var SerialPort = require("serialport").SerialPort;

var serialPort = new SerialPort(portname, {
  baudrate: 115200
});



// parameters from Blynk app
var esc; // VP0
var servo; // VP1

// Blynk input virtual pins
var escpin = new blynk.VirtualPin(0);
escpin.on('write',function(param) {
  esc = param;
});

var servopin = new blynk.VirtualPin(1);
servopin.on('write',function(param) {
  servo = param;
});




// readings from Arduino
var gyro; // VP2
var pwm; // VP3

// Blynk output virtual pins
var gyropin = new blynk.VirtualPin(2);
gyropin.on('read',function() {
  gyropin.write(gyro);
});

var pwmpin = new blynk.VirtualPin(3);
pwmpin.on('read',function() {
  pwmpin.write(pwm);
});


// serial handlers
serialPort.on("open", function () {
  console.log('open');
  serialPort.on('data', function(data) {
    // input format:
    // gyro (int16)
    // pwm (int16)
    gyro = data.readInt16LE(0);
    pwm = data.readInt16LE(2);
  });
  setInterval( function() {
    // output format:
    // esc
    // servo
    data = new Buffer(4);
    data.writeInt16LE(esc,0);
    data.writeInt16LE(servo,2);
    serialPort.write(data);
  },1000);
});

