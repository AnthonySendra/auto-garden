/*
 * 000000    | 000000    | 00000      | 000000000000000
 * sender    | receiver  | data type  | data
 * 0x3F      | 0x3F      | 0x1F       | 0x7FFF
 *
*/

var rpi433    = require('rpi-433')
const ME = 0x3F
const SONDE_HUMIDITY = 0x01

const SYNC = 0x00

rfSniffer = rpi433.sniffer({
  pin: 2,
  debounceDelay: 200
})
rfEmitter = rpi433.emitter({
  pin: 0,
  pulseLength: 350
})



rfSniffer.on('data', function (data) {

  let sender = (data.code >> 26) & 0x3F
  let receiver = (data.code >> 20) & 0x3F
  let dataType = (data.code >> 15) & 0x1F
  let value = data.code & 0x7FFF

  if (dataType === SYNC) {
  	let message = (ME << 26) | (SONDE_HUMIDITY << 20) | (SYNC << 16) | 1
  	rfEmitter.sendCode(message)
  }
});


