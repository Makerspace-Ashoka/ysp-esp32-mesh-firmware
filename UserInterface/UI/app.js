var http = require('http');
var fs = require('fs');

const PORT = 3000

fs.readFile('./public/home.html',function(error,html) {
    if(error) throw error;
    http.createServer(function(request,response){
        response.writeHeader(200,{'content-Type':'Text/html'});
        response.write(html);
        response.end();
    }).listen(PORT)
});


