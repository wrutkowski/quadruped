var oledWidth = 128;
var oledHeight = 64;
var pointSize = 5;

var oled;
var mouseChanges;
var drawingEnabled = false;

function mousePressed() {
	drawingEnabled = true;
	changePoint();
}

function mouseDragged(event) {
	changePoint();
}

function changePoint() {
	if (!drawingEnabled) return;

	if (event.target.tagName == "CANVAS") {
		pointX = floor(mouseX / pointSize);
		pointY = floor(mouseY / pointSize);
		if (pointX >= 0 && pointX < oledWidth && pointY >= 0 && pointY < oledHeight) {
			// console.log("clicked", pointX, pointY);
			if (mouseChanges[pointX][pointY] == 0) { // only one change per drag
				oled[pointX][pointY] = oled[pointX][pointY] == 0 ? 1 : 0;
				mouseChanges[pointX][pointY] = 1;
				redraw();
			}
		}
	}
}

function mouseReleased() {
	mouseChanges = new Array(oledHeight);
	for (x = 0; x < oledWidth; x++) {
		mouseChanges[x] = new Array(oledHeight);
		for (y = 0; y < oledHeight; y++) {
			mouseChanges[x][y] = 0;
		}
	}

	drawingEnabled = false;
}

function windowResized() {
    resizeCanvas(windowWidth, windowHeight);
}

function setup() {
    createCanvas(windowWidth, windowHeight);
	noLoop();

	// gui
	gui = createGui('Info', width - 220, 20);
	gui.prototype.addHTML("DISPLAY", "Generate frames for a display");
	gui.prototype.addHTML("Instructions", "mouse drag - select points");

	var guiFile = createGui('Frame', width - 220, 20 + 121 + 20);
	guiFile.prototype.addButton("Save", function() {
		var cmd = "";
		for (x = 0; x < oledWidth; x++) {
			for (y = 0; y < oledHeight; y++) {
				if (oled[x][y] == 1) {
					cmd += "lcd.setPixel(" + x + ", " + y + ", 1);";
				}
			}
		}
		console.log(cmd);
	});

	oled = new Array(oledHeight);
	for (x = 0; x < oledWidth; x++) {
		oled[x] = new Array(oledHeight);
		for (y = 0; y < oledHeight; y++) {
			oled[x][y] = 0;
		}
	}

	mouseChanges = oled;
}

function draw() {
	stroke(220);
	for (y = 0; y < oledHeight; y++) {
		for (x = 0; x < oledWidth; x++) {
			if (oled[x][y] == 0) {
				fill(255);
			} else {
				fill(0);
			}
			rect(x * pointSize, y * pointSize, pointSize, pointSize);
		}
	}
}
