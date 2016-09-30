/* need to be changed in case of picking another libtorrent revision */
var sha = 'b342192052d82be666e1ef9f320a32a7c24fed10';

var os = require('os');
var fs = require('fs');
var url = require('url');
var log = require('npmlog');
var path = require('path');
var https = require('https');
var admZip = require('adm-zip');
var cmakeJS = require('cmake-js');

/* where to download libtorrent from */
var githubURL = 'https://codeload.github.com/arvidn/libtorrent/zip/COMMIT';

/* construct exact path */
var snapshot = githubURL.replace('COMMIT', sha);
var destfile = path.join(os.tmpdir(), path.basename(url.parse(snapshot).pathname));

function info(message) {
    log.info('PKG', message);
}

/* download and unzip */
function downloadAndUnzip (url, destdir, callback) {
    var tempfile = path.join(os.tmpdir(), 'temporary_download');

    info('Downloading release...');

    https.get(url, function(response) {
        response.on('data', function (data) {
            fs.appendFileSync(tempfile, data)
        });

        response.on('end', function() {
            var zip = new admZip(tempfile);

            info('Unpacking...');
            zip.extractAllTo(destdir, /* overwrite? */ true);
            fs.unlink(tempfile);

            callback();
        });

        response.on('error', function() {
            fs.unlink(tempfile);
        });
    });
}

function build() {
    /* get needed runtime version from env */
    var defaultRuntime = "node";
    var defaultRuntimeVersion = "4.5.0";
    var defaultRuntimeArch = "ia32";

    var options = {
        runtime: process.env.NDLT_RUNTIME || defaultRuntime,
        runtimeVersion: process.env.NDLT_RUNTIME_VERSION || defaultRuntimeVersion,
        arch: process.env.NDLT_ARCH || defaultRuntimeArch
    };

    var buildSystem = new cmakeJS.BuildSystem(options);

    info('Running compilation for ' + options.runtime + '-' + options.runtimeVersion + '@' + options.arch);
    buildSystem.compile().catch( function() { process.exit(1); } );
}

function main() {
    var found = false;
    var libdir = path.join(process.cwd(), 'deps', 'libtorrent');
    var depsdir = path.join(process.cwd(), 'deps');

    info('Setting up compilation environment');

    /* check for already resolved dependency */
    if( fs.existsSync(libdir) ) {
        var files = fs.readdirSync(libdir);

        /* if CMake is here, everyone's in the place © */
        for(var x = 0; x < files.length; x ++) {
            if(files[x] == 'CMakeLists.txt')
                found = true;
        }
    }

    if( ! found ) {
        info('Resolving libtorrent dependency');

        if( !fs.existsSync(depsdir)) {
            fs.mkdirSync(depsdir);
        }    

        if( fs.existsSync(libdir) ) {
            fs.rmdirSync(libdir);
        }

        downloadAndUnzip(snapshot, path.join(depsdir, path.sep), function() {
            var libname = fs.readdirSync(depsdir)[0];

            fs.renameSync(path.join(depsdir, libname), libdir);
            build();
        });
    } else {
        info('Libtorrent already present');
        build();
    }
}

main();
