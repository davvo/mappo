var fs = require('fs'),
    mappo = require('../lib/mappo');

var map =  new mappo.Map(500, 500);

map.clear(100, 13, 43);
map.drawPolygon([100, 200, 300], [100, 300, 100]);

fs.createWriteStream('out.png').write(map.getPNG());