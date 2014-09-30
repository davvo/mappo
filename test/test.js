var mappo = require('./lib/mappo');
console.log(mappo.version);

var map =  new mappo.Map(500, 500);
console.log(map.describe());