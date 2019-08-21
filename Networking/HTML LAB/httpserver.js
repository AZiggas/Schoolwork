"use strict";

var http = require('http');
var url = require('url');
var fs = require('fs');

var mazeFile = "maze.txt";
var mazeContents = fs.readFileSync(mazeFile).toString().split('\n');

function makeTable(mazeContents, x, y){
	//x,y are player position
	var tableString = "<table>\n";
	
	//Here are constraints for the rendered area. 5 to left and right, and 5 to the top and bottom.
	
	//Row will look like this but with 10 of them<td width = 50px height = 50px></td>
	var y1;
	var y2;
	var x1;
	var x2;
	if(y < 5){
		y1 = 0;
		y2 = y+(10-y);
	}
	else if(y > mazeContents.length - 6){
		y2 = mazeContents.length;
		y1 = y - (10 - ((mazeContents.length)-y));
	}
	else{
		y1 =y -5;
		y2 = y+5;
	}
	
	if(x < 5){
		x1 = 0;
		x2 = x + (10-x);
	}
	else if(x > mazeContents[0].length- 6){
		x2 = mazeContents[0].length;
		x1 = x- (10 - ((mazeContents[0].length)-x));
	}
	else{
		x1 = x-5;
		x2 = x+5;
	}
	
	for(var j = y1; j < y2; j++){
		tableString = tableString + "<tr>\n"
		if(j >= 0 && j < mazeContents.length){
			for(var i = x1; i < x2; i++){
				if(i >= 0 && i < mazeContents[0].length){
					if(i == x && j == y){
						tableString = tableString + '<td bgcolor="yellow" width = 50px height = 50px></td>\n';
					}else{
						if(mazeContents[j][i] == "#"){
							tableString = tableString + '<td bgcolor="black" width = 50px height = 50px></td>\n';
						}
						if(mazeContents[j][i] == "."){
							tableString = tableString + '<td bgcolor="red" width = 50px height = 50px></td>\n';
						}
					}
				}else{
					tableString = tableString + '<td bgcolor="blue" width = 50px height = 50px></td>\n';
				}
				
			}
		}else{
			for(var q = 0; q <= 10; q++){
				tableString = tableString + '<td bgcolor="blue" width = 50px height = 50px></td>\n';
			}
		}
		tableString = tableString + "</tr>\n"
	}
	tableString = tableString + "</table>"
	
	return tableString;
}

var x;
var y;

var contenttypes = {
    '.jpeg' : 'image/jpeg',
    '.jpg' : 'image/jpeg',
    '.png': 'image/png',
    '.gif' : 'image/gif',
    '.js' : 'application/javascript',
    '.css' : 'text/css',
    '.htm' : 'text/html',
    '.html' : 'text/html' 
}

function handler(req,resp){
    var pu = url.parse(req.url);
    var p = process.cwd() + pu.pathname;
    console.log(pu.pathname);
	try{
        var s = fs.statSync(p);
    }
    catch(e){
		if(pu.path != '/' && pu.path != '/down' && pu.path != '/up' && pu.path != '/left' && pu.path != '/right'){
			console.log(e.toString());
			resp.writeHead(404,{'Content-type':'text/html'});
			resp.end("<HTML>Not found</HTML>");
			return;
		}
    }
	resp.writeHead(200,{"Content-type":"text/html"});
	resp.write("<!DOCTYPE html>\n");
	resp.write("<html>\n");
	resp.write("<title>MAZE</title>");
	resp.write("<head><meta charset='utf-8'></head>");
	resp.write("<body>");
	resp.write("<center>");
	resp.write("<button onclick =" + "\"window.location = '/up'\"" + "> &uarr;</button><br>");
	resp.write("<button  onclick =" + "\"window.location = '/left'\"" + "> &larr;</button>");
	resp.write("<button onclick =" + "\"window.location = '/right'\"" + "> &rarr;</button><br>");
	resp.write("<button onclick =" + "\"window.location = '/down'\"" + "> &darr;</button>");
	if(pu.path ==='/'){
		x = 1;
		y = 2;
		resp.write(makeTable(mazeContents, x, y));
	}
	else if(pu.path ==='/up'){
		if(mazeContents[y-1][x] != '#' && y-1 > 0){
			y = y-1;
			resp.write(makeTable(mazeContents, x, y));
		}
		else{
			resp.write(makeTable(mazeContents, x, y));
		}
	}
	else if(pu.path ==='/down'){
		if(mazeContents[y+1][x] != '#' && y+1 < 49){
			y = y+1;
			resp.write(makeTable(mazeContents, x, y));
		}
		else{
			resp.write(makeTable(mazeContents, x, y));
		}
	}
	else if(pu.path ==='/right'){
		if(mazeContents[y][x+1] != '#' && x+1 < 49){
			x = x+1;
			resp.write(makeTable(mazeContents, x, y));
		}
		else{
			resp.write(makeTable(mazeContents, x, y));
		}
	}
	else if(pu.path ==='/left'){
		if(mazeContents[y][x-1] != '#' && x-1 < 49){
			x = x-1;
			resp.write(makeTable(mazeContents, x, y));
		}
		else{
			resp.write(makeTable(mazeContents, x, y));
		}
	}
	resp.end("</center></body></html>");
}

var server = http.createServer(handler);
server.listen(8080,'127.0.0.1');
console.log("Listening on port 8080");