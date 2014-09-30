var mappo = require('../build/Release/mappo'),
    pkg = require('../package.json');

mappo.version = pkg.version;
module.exports = mappo;