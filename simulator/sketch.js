var bodyX = 75;
var bodyY = 120;
var bodyZ = 45;
var hipLength = 40;
var thighLength = 65;
var tibiaLength = 105;

var L = (hipLength + thighLength + tibiaLength) * 0.95; // max leg length

var leg1alpha;
var leg1beta;
var leg1theta;
var leg2alpha;
var leg2beta;
var leg2theta;
var leg3alpha;
var leg3beta;
var leg3theta;
var leg4alpha;
var leg4beta;
var leg4theta;

var legRatio1x = 0.5;
var legRatio1y = 0.5;
var legRatio1z = -0.25;
var legRatio2x = 0.5;
var legRatio2y = 0.5;
var legRatio2z = -0.25;
var legRatio3x = 0.5;
var legRatio3y = 0.5;
var legRatio3z = -0.25;
var legRatio4x = 0.5;
var legRatio4y = 0.5;
var legRatio4z = -0.25;

var leg1x = legRatio1x * L;
var leg1y = legRatio1y * L;
var leg1z = legRatio1z * L;
var leg2x = legRatio2x * L;
var leg2y = legRatio2y * L;
var leg2z = legRatio2z * L;
var leg3x = legRatio3x * L;
var leg3y = legRatio3y * L;
var leg3z = legRatio3z * L;
var leg4x = legRatio4x * L;
var leg4y = legRatio4y * L;
var leg4z = legRatio4z * L;

var setLeg1x = leg1x;
var setLeg1y = leg1y;
var setLeg1z = leg1z;
var setLeg2x = leg2x;
var setLeg2y = leg2y;
var setLeg2z = leg2z;
var setLeg3x = leg3x;
var setLeg3y = leg3y;
var setLeg3z = leg3z;
var setLeg4x = leg4x;
var setLeg4y = leg4y;
var setLeg4z = leg4z;

var robotAnimation = 'none';
var speed = 5;
var animation = ['none', 'walk forward', 'walk forward 2', 'stand', 'sit', 'dead', 'wave'];
var animationTimer;

function computeXYZToAngles(x, y, z) {
	var z = -z;
	// console.log("x = " + x + ", y = " + y + ", z = " + z);

	var alpha = atan2(y, x);

	var L1 = sqrt(pow(x, 2) + pow(y, 2));
	var L = sqrt(pow(L1 - hipLength, 2) + pow(z, 2));
	var beta = acos(z / L) + acos((pow(thighLength, 2) + pow(L, 2) - pow(tibiaLength, 2)) / (2 * thighLength * L));
	var theta = acos((pow(tibiaLength, 2) + pow(thighLength, 2) - pow(L, 2)) / (2 * tibiaLength * thighLength));

	if (isNaN(alpha) || isNaN(beta) || isNaN(theta)) {
		// console.log("Warning! Position not reachable.");
		return undefined
	} else {
		alpha = degrees(alpha);
		beta = degrees(beta) - 90;
		theta = degrees(theta) - 180;

		// console.log("LEG: " + alpha + ", " + beta + ", " + theta);
		return {"alpha": alpha, "beta": beta, "theta": theta};
	}
}

function setLeg(legNumber, x, y, z) {
	if (legNumber == 1) {
		setLeg1x = x * L;
		setLeg1y = y * L;
		setLeg1z = z * L;
	} else if (legNumber == 2) {
		var t = x; x = y; y = t;
		setLeg2x = x * L;
		setLeg2y = y * L;
		setLeg2z = z * L;
	} else if (legNumber == 3) {
		setLeg3x = x * L;
		setLeg3y = y * L;
		setLeg3z = z * L;
	} else if (legNumber == 4) {
		var t = x; x = y; y = t;
		setLeg4x = x * L;
		setLeg4y = y * L;
		setLeg4z = z * L;
	}
}

function stand() {
	setLeg(1, 0.5, 0.5, -0.25);
	setLeg(2, 0.5, 0.5, -0.25);
	setLeg(3, 0.5, 0.5, -0.25);
	setLeg(4, 0.5, 0.5, -0.25);
}

function sit() {
	setLeg(1, 0.4, 0.4, 0);
	setLeg(2, 0.4, 0.4, 0);
	setLeg(3, 0.4, 0.4, 0);
	setLeg(4, 0.4, 0.4, 0);
}

function dead() {
	setLeg(1, 0, 1, 0);
	setLeg(2, 0, 1, 0);
	setLeg(3, 0, 1, 0);
	setLeg(4, 0, 1, 0);
}

function wave(step) {
	var stepTime = 500;
	var step = step || 0;
	// console.log("walk forward: " + step);
	if (step == 0) {
		stand();

		animationTimer = setTimeout(function(){ wave(1); }, stepTime);
	} else if (step == 1) {
		setLeg(1, -0.2, 0.4, 0.6);

		animationTimer = setTimeout(function(){ wave(2); }, stepTime);
	} else if (step == 2) {
		setLeg(1, 0.2, 0.4, 0.6);

		animationTimer = setTimeout(function(){ wave(1); }, stepTime);
	}
}

function walkForward(step) {
	var stepTime = 1700 / speed;
	var step = step || 0;
	// console.log("walk forward: " + step);

	var movementMin = -0.2;
	var movementMax = 0.6;
	var movementStep = 0.2;

	if (step == 0) {
		setLeg(1, 0.5, movementMax - 4 * movementStep, -0.25);
		setLeg(2, 0.5, movementMax - 2 * movementStep, -0.25);
		setLeg(3, 0.5, movementMin + 3 * movementStep, -0.25);
		setLeg(4, 0.5, movementMin + 1 * movementStep, -0.25);

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 1) {
		setLeg(1, 0.5, movementMax - 4 * movementStep, 0); // up

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 2) {
		setLeg(1, 0.5, movementMax - 0 * movementStep, 0); // move forward

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 3) {
		setLeg(1, 0.5, movementMax - 0 * movementStep, -0.25); // down

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 4) {
		// move body
		setLeg(1, 0.5, movementMax - 1 * movementStep, -0.25);
		setLeg(2, 0.5, movementMax - 3 * movementStep, -0.25);
		setLeg(3, 0.5, movementMin + 4 * movementStep, -0.25);
		setLeg(4, 0.5, movementMin + 2 * movementStep, -0.25);

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 5) {
		setLeg(3, 0.5, movementMin + 4 * movementStep, 0); // up

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 6) {
		setLeg(3, 0.5, movementMin + 0 * movementStep, 0); // move forward

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 7) {
		setLeg(3, 0.5, movementMin + 0 * movementStep, -0.25); // down

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 8) {
		// move body
		setLeg(1, 0.5, movementMax - 2 * movementStep, -0.25);
		setLeg(2, 0.5, movementMax - 4 * movementStep, -0.25);
		setLeg(3, 0.5, movementMin + 1 * movementStep, -0.25);
		setLeg(4, 0.5, movementMin + 3 * movementStep, -0.25);

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 9) {
		setLeg(2, 0.5, movementMax - 4 * movementStep, 0); // up

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 10) {
		setLeg(2, 0.5, movementMax - 0 * movementStep, 0); // move forward

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 11) {
		setLeg(2, 0.5, movementMax - 0 * movementStep, -0.25); // down

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 12) {
		// move body
		setLeg(1, 0.5, movementMax - 3 * movementStep, -0.25);
		setLeg(2, 0.5, movementMax - 1 * movementStep, -0.25);
		setLeg(3, 0.5, movementMin + 2 * movementStep, -0.25);
		setLeg(4, 0.5, movementMin + 4 * movementStep, -0.25);

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 13) {
		setLeg(4, 0.5, movementMin + 4 * movementStep, 0); // up

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 14) {
		setLeg(4, 0.5, movementMin + 0 * movementStep, 0); // move forward

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 15) {
		setLeg(4, 0.5, movementMin + 0 * movementStep, -0.25); // down

		animationTimer = setTimeout(function(){ walkForward(++step); }, stepTime);
	} else if (step == 16) {
		// move body
		setLeg(1, 0.5, movementMax - 4 * movementStep, -0.25);
		setLeg(2, 0.5, movementMax - 2 * movementStep, -0.25);
		setLeg(3, 0.5, movementMin + 3 * movementStep, -0.25);
		setLeg(4, 0.5, movementMin + 1 * movementStep, -0.25);

		animationTimer = setTimeout(function(){ walkForward(1); }, stepTime);
	}
}

function walkForward2(step) {
	var stepTime = 1700 / speed;
	var step = step || 0;
	// console.log("walk forward: " + step);

	var movementMin = -0.1;
	var movementMax = 0.6;
	var movementStep = 0.35;

	if (step == 0) {
		setLeg(1, 0.5, movementMax - 2 * movementStep, -0.25);
		setLeg(2, 0.5, movementMax - 1 * movementStep, -0.25);
		setLeg(3, 0.5, movementMax - 1 * movementStep, -0.25);
		setLeg(4, 0.5, movementMax - 0 * movementStep, -0.25);

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 1) {
		setLeg(4, 0.5, movementMax - 0 * movementStep, 0); // up

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 2) {
		setLeg(4, 0.5, movementMax - 2 * movementStep, 0); // move forward

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 3) {
		setLeg(4, 0.5, movementMax - 2 * movementStep, -0.25); // down

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 4) {
		setLeg(1, 0.5, movementMax - 2 * movementStep, 0); // up

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 5) {
		setLeg(1, 0.5, movementMax - 0 * movementStep, 0); // move forward

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 6) {
		setLeg(1, 0.5, movementMax - 0 * movementStep, -0.25); // down

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 7) {
		// move body
		setLeg(1, 0.5, movementMax - 1 * movementStep, -0.25);
		setLeg(2, 0.5, movementMax - 2 * movementStep, -0.25);
		setLeg(3, 0.5, movementMax - 0 * movementStep, -0.25);
		setLeg(4, 0.5, movementMax - 1 * movementStep, -0.25);

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 8) {
		setLeg(3, 0.5, movementMax - 0 * movementStep, 0); // up

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 9) {
		setLeg(3, 0.5, movementMax - 2 * movementStep, 0); // move forward

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 10) {
		setLeg(3, 0.5, movementMax - 2 * movementStep, -0.25); // down

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 11) {
		setLeg(2, 0.5, movementMax - 2 * movementStep, 0); // up

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 12) {
		setLeg(2, 0.5, movementMax - 0 * movementStep, 0); // move forward

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 13) {
		setLeg(2, 0.5, movementMax - 0 * movementStep, -0.25); // down

		animationTimer = setTimeout(function(){ walkForward2(++step); }, stepTime);
	} else if (step == 14) {
		// move body
		setLeg(1, 0.5, movementMax - 2 * movementStep, -0.25);
		setLeg(2, 0.5, movementMax - 1 * movementStep, -0.25);
		setLeg(3, 0.5, movementMax - 1 * movementStep, -0.25);
		setLeg(4, 0.5, movementMax - 0 * movementStep, -0.25);

		animationTimer = setTimeout(function(){ walkForward2(1); }, stepTime);
	}
}

// GUI

var worldRotateX = -60;
var worldRotateY = 5;
var worldRotateZ = 145;
var worldCameraX = 0;
var worldCameraY = 0;
var worldCameraZ = 0;

var orbitXKey = false;
var xKey = false;
var yKey = false;
var zKey = false;

var gui;
var guiLeg1;
var guiLeg2;
var guiLeg3;
var guiLeg4;

var planeSize = 800;
var gridSize = 40;

var showPlane = true;
var showAxes = true;

var originMouseX;
var originMouseY;

function keyPressed() {
	if (keyCode == 91) { // cmd
		orbitXKey = true;
	}
	if (keyCode == 88) { // x
		xKey = true;
	}
	if (keyCode == 89) { // y
		yKey = true;
	}
	if (keyCode == 90) { // z
		zKey = true;
	}
}
function keyReleased() {
	orbitXKey = false;
	xKey = false;
	yKey = false;
	zKey = false;
}

function mousePressed() {
	originMouseX = mouseX;
	originMouseY = mouseY;
}

function mouseDragged(event) {
	if (event.target.tagName == "CANVAS") {
		worldRotateZ += mouseX - originMouseX;
		if (orbitXKey) {
			worldRotateY += mouseY - originMouseY;
		} else {
			worldRotateX += mouseY - originMouseY;
		}
	}
	originMouseX = mouseX;
	originMouseY = mouseY;
}

function mouseWheel(event) {
	if (xKey) {
		worldCameraX += event.delta;
	}
	if (yKey) {
		worldCameraY += event.delta;
	}
	if (zKey) {
		worldCameraZ += event.delta;
	}
	return false;
}

function windowResized() {
    resizeCanvas(windowWidth, windowHeight);
}

function setup() {
    createCanvas(windowWidth, windowHeight, WEBGL);

    // gui
	gui = createGui('Info');
	gui.prototype.addHTML("QUADRUPED", "Inverse Kinematics implemented on 3DOF quadruped");
	gui.prototype.addHTML("Instructions", "mouse drag - pivot<br>x + scroll - move along X axis<br>y + scroll - move along Y axis<br>z + scroll - move along Z axis<br>Double click to collapse panels");
	gui.prototype.addHTML("World Camera", "");

	var guiView = createGui('View', 20, 20 + 255 + 20);
	guiView.addGlobals('showPlane');
	guiView.addGlobals('showAxes');
	guiView.prototype.addButton("TOP", function(){
		worldRotateX = 0;
		worldRotateY = 0;
		worldRotateZ = 0;
		worldCameraX = 0;
		worldCameraY = 0;
		worldCameraZ = 0;
	});
	guiView.prototype.addButton("FRONT", function(){
		worldRotateX = 270;
		worldRotateY = 0;
		worldRotateZ = 180;
		worldCameraX = 0;
		worldCameraY = 0;
		worldCameraZ = 0;
	});
	guiView.prototype.addButton("PERSPECTIVE", function(){
		worldRotateX = -60;
		worldRotateY = 5;
		worldRotateZ = -145;
		worldCameraX = 0;
		worldCameraY = 0;
		worldCameraZ = 0;
	});

	var guiRobot = createGui('Robot', 20, 20 + 255 + 20 + 226 + 20);
	guiRobot.addGlobals('animation');
	sliderRange(1, 20, 0.5);
	guiRobot.addGlobals('speed');

	guiLeg1 = createGui('Leg 1', width - 220, 20);
	sliderRange(-1, 1, 0.01);
	guiLeg1.addGlobals('legRatio1x');
	sliderRange(-1, 1, 0.01);
	guiLeg1.addGlobals('legRatio1y');
	sliderRange(-0.7, 0.7, 0.01);
	guiLeg1.addGlobals('legRatio1z');

	guiLeg2 = createGui('Leg 2', width - 220, 1 * (176 + 20) + 20);
	sliderRange(-1, 1, 0.01);
	guiLeg2.addGlobals('legRatio2x');
	sliderRange(-1, 1, 0.01);
	guiLeg2.addGlobals('legRatio2y');
	sliderRange(-0.7, 0.7, 0.01);
	guiLeg2.addGlobals('legRatio2z');

	guiLeg3 = createGui('Leg 3', width - 220, 2 * (176 + 20) + 20);
	sliderRange(-1, 1, 0.01);
	guiLeg3.addGlobals('legRatio3x');
	sliderRange(-1, 1, 0.01);
	guiLeg3.addGlobals('legRatio3y');
	sliderRange(-0.7, 0.7, 0.01);
	guiLeg3.addGlobals('legRatio3z');

	guiLeg4 = createGui('Leg 4', width - 220, 3 * (176 + 20) + 20);
	sliderRange(-1, 1, 0.01);
	guiLeg4.addGlobals('legRatio4x');
	sliderRange(-1, 1, 0.01);
	guiLeg4.addGlobals('legRatio4y');
	sliderRange(-0.7, 0.7, 0.01);
	guiLeg4.addGlobals('legRatio4z');

	// perspective
    var fov = 60 / 180 * PI;
    var cameraZ = (height/2.0) / tan(fov/2.0);
    perspective(60 / 180 * PI, width/height, cameraZ * 0.1, cameraZ * 10);
}

function draw() {
	if (animation != robotAnimation) {
		robotAnimation = animation;
		if (animationTimer) {
			clearTimeout(animationTimer);
		}
		if (robotAnimation == "none") {
			setLeg(1, 0.5, 0.5, 0);
			setLeg(2, 0.5, 0.5, 0);
			setLeg(3, 0.5, 0.5, 0);
			setLeg(4, 0.5, 0.5, 0);
		} else if (robotAnimation == "stand") {
			stand();
		} else if (robotAnimation == "sit") {
			sit();
		} else if (robotAnimation == "wave") {
			wave();
		} else if (robotAnimation == "dead") {
			dead();
		} else if (robotAnimation == "walk forward") {
			walkForward();
		} else if (robotAnimation == "walk forward 2") {
			walkForward2();
		}
	}

	if (robotAnimation != "none") {
		var movementSpeed = speed;
		if (leg1x < setLeg1x) { leg1x = min(leg1x + movementSpeed, setLeg1x); } else if (leg1x > setLeg1x) { leg1x = max(leg1x - movementSpeed, setLeg1x); }
		if (leg1y < setLeg1y) { leg1y = min(leg1y + movementSpeed, setLeg1y); } else if (leg1y > setLeg1y) { leg1y = max(leg1y - movementSpeed, setLeg1y); }
		if (leg1z < setLeg1z) { leg1z = min(leg1z + movementSpeed, setLeg1z); } else if (leg1z > setLeg1z) { leg1z = max(leg1z - movementSpeed, setLeg1z); }
		if (leg2x < setLeg2x) { leg2x = min(leg2x + movementSpeed, setLeg2x); } else if (leg2x > setLeg2x) { leg2x = max(leg2x - movementSpeed, setLeg2x); }
		if (leg2y < setLeg2y) { leg2y = min(leg2y + movementSpeed, setLeg2y); } else if (leg2y > setLeg2y) { leg2y = max(leg2y - movementSpeed, setLeg2y); }
		if (leg2z < setLeg2z) { leg2z = min(leg2z + movementSpeed, setLeg2z); } else if (leg2z > setLeg2z) { leg2z = max(leg2z - movementSpeed, setLeg2z); }
		if (leg3x < setLeg3x) { leg3x = min(leg3x + movementSpeed, setLeg3x); } else if (leg3x > setLeg3x) { leg3x = max(leg3x - movementSpeed, setLeg3x); }
		if (leg3y < setLeg3y) { leg3y = min(leg3y + movementSpeed, setLeg3y); } else if (leg3y > setLeg3y) { leg3y = max(leg3y - movementSpeed, setLeg3y); }
		if (leg3z < setLeg3z) { leg3z = min(leg3z + movementSpeed, setLeg3z); } else if (leg3z > setLeg3z) { leg3z = max(leg3z - movementSpeed, setLeg3z); }
		if (leg4x < setLeg4x) { leg4x = min(leg4x + movementSpeed, setLeg4x); } else if (leg4x > setLeg4x) { leg4x = max(leg4x - movementSpeed, setLeg4x); }
		if (leg4y < setLeg4y) { leg4y = min(leg4y + movementSpeed, setLeg4y); } else if (leg4y > setLeg4y) { leg4y = max(leg4y - movementSpeed, setLeg4y); }
		if (leg4z < setLeg4z) { leg4z = min(leg4z + movementSpeed, setLeg4z); } else if (leg4z > setLeg4z) { leg4z = max(leg4z - movementSpeed, setLeg4z); }

		legRatio1x = leg1x / L;
		legRatio1y = leg1y / L;
		legRatio1z = leg1z / L;
		legRatio2x = leg2y / L;
		legRatio2y = leg2x / L;
		legRatio2z = leg2z / L;
		legRatio3x = leg3x / L;
		legRatio3y = leg3y / L;
		legRatio3z = leg3z / L;
		legRatio4x = leg4y / L;
		legRatio4y = leg4x / L;
		legRatio4z = leg4z / L;

		guiLeg1.prototype.setRangeValue("legRatio1x", legRatio1x);
		guiLeg1.prototype.setRangeValue("legRatio1y", legRatio1y);
		guiLeg1.prototype.setRangeValue("legRatio1z", legRatio1z);
		guiLeg2.prototype.setRangeValue("legRatio2x", legRatio2x);
		guiLeg2.prototype.setRangeValue("legRatio2y", legRatio2y);
		guiLeg2.prototype.setRangeValue("legRatio2z", legRatio2z);
		guiLeg3.prototype.setRangeValue("legRatio3x", legRatio3x);
		guiLeg3.prototype.setRangeValue("legRatio3y", legRatio3y);
		guiLeg3.prototype.setRangeValue("legRatio3z", legRatio3z);
		guiLeg4.prototype.setRangeValue("legRatio4x", legRatio4x);
		guiLeg4.prototype.setRangeValue("legRatio4y", legRatio4y);
		guiLeg4.prototype.setRangeValue("legRatio4z", legRatio4z);
	} else {
		setLeg(1, legRatio1x, legRatio1y, legRatio1z);
		setLeg(2, legRatio2x, legRatio2y, legRatio2z);
		setLeg(3, legRatio3x, legRatio3y, legRatio3z);
		setLeg(4, legRatio4x, legRatio4y, legRatio4z);
		leg1x = setLeg1x;
		leg1y = setLeg1y;
		leg1z = setLeg1z;
		leg2x = setLeg2x;
		leg2y = setLeg2y;
		leg2z = setLeg2z;
		leg3x = setLeg3x;
		leg3y = setLeg3y;
		leg3z = setLeg3z;
		leg4x = setLeg4x;
		leg4y = setLeg4y;
		leg4z = setLeg4z;
	}

	var angles = computeXYZToAngles(leg1x, leg1y, leg1z);
	if (angles != undefined) {
		// console.log(angles);
		leg1alpha = angles.alpha;
		leg1beta = angles.beta;
		leg1theta = angles.theta;
	}
	var angles = computeXYZToAngles(leg2x, leg2y, leg2z);
	if (angles != undefined) {
		// console.log(angles);
		leg2alpha = angles.alpha;
		leg2beta = angles.beta;
		leg2theta = angles.theta;
	}
	var angles = computeXYZToAngles(leg3x, leg3y, leg3z);
	if (angles != undefined) {
		// console.log(angles);
		leg3alpha = angles.alpha;
		leg3beta = angles.beta;
		leg3theta = angles.theta;
	}
	var angles = computeXYZToAngles(leg4x, leg4y, leg4z);
	if (angles != undefined) {
		// console.log(angles);
		leg4alpha = angles.alpha;
		leg4beta = angles.beta;
		leg4theta = angles.theta;
	}

	background(30);
	noStroke();

	rotateY(radians(worldRotateY));
	rotateX(radians(worldRotateX));
	rotateZ(radians(worldRotateZ));

	camera(worldCameraX, worldCameraY, worldCameraZ);
	gui.prototype.setHTML("World Camera", "∢" + worldRotateX % 360 + "," + worldRotateY % 360 + "," + worldRotateZ % 360 + "<br>⌖" + worldCameraX  + "," + worldCameraY + "," + worldCameraZ);

	ambientLight(100);
	pointLight(250, 250, 250, 0, 0, 400);

	if (showPlane) {
		// Plane
		push();
		translate(0, 0, -0.25 * L);
		fill(50);
		plane(planeSize, planeSize);
		pop();

		for (var i = -planeSize / gridSize / 2 + 1; i < planeSize / gridSize / 2; i++) {
			push();
			translate(0, i * gridSize, -0.25 * L);
			fill(80);
			box(planeSize, 1, 1);
			pop();
			push();
			translate(i * gridSize, 0, -0.25 * L);
			fill(80);
			box(1, planeSize, 1);
			pop();
		}
	}

	// robot
	push();
	ambientMaterial(150);
	box(bodyX, bodyY, bodyZ);
	pop();

	push();
	translate(bodyX/2, -bodyY/2, 0);
	drawLeg(leg1alpha, leg1beta, leg1theta, leg1x, -leg1y, leg1z);
	pop();

	push();
	rotateZ(radians(90));
	translate(bodyY/2, -bodyX/2, 0);
	drawLeg(leg2alpha, leg2beta, leg2theta, leg2x, -leg2y, leg2z);
	pop();

	push();
	rotateZ(radians(180));
	translate(bodyX/2, -bodyY/2, 0);
	drawLeg(leg3alpha, leg3beta, leg3theta, leg3x, -leg3y, leg3z);
	pop();

	push();
	rotateZ(radians(270));
	translate(bodyY/2, -bodyX/2, 0);
	drawLeg(leg4alpha, leg4beta, leg4theta, leg4x, -leg4y, leg4z);
	pop();

	if (showAxes) {
		// X Y Z lines
		push();
		// x
		fill(255, 0, 0);
		box(1000, 1, 1);
		// y
		fill(0, 255, 0);
		box(1, 1000, 1);
		// z
		fill(0, 0, 255);
		box(1, 1, 1000);
		pop();
	}
}

function drawLeg(legAlpha, legBeta, legTheta, legX, legY, legZ) {
	// leg 1 hip
	push();
	ambientMaterial(100, 255, 100);
	rotateZ(radians(legAlpha));
	translate(hipLength / 2, 0, 0);
	box(hipLength, 24, 24);
	translate(hipLength / 2, 0, 0);

	// leg 1 thigh
	ambientMaterial(255, 100, 100);
	translate(thighLength * 0.5 * cos(radians(legBeta)), 0, thighLength * 0.5 * sin(radians(legBeta)));
	rotateY(radians(-legBeta));
	box(thighLength, 22, 22);
	translate(thighLength / 2, 0, 0);

	// leg 1 tibia
	ambientMaterial(100, 100, 255);
	translate(tibiaLength * 0.5 * cos(radians(legTheta)), 0, tibiaLength * 0.5 * sin(radians(legTheta)));
	rotateY(radians(-legTheta));
	box(tibiaLength, 20, 20);
	pop();

	// possible movement
	// fill(255, 255, 255, 20); sphere(L);

	// leg 1 X Y Z point
	push();
	fill(255);
	translate(legX, legY, legZ);
	sphere(2);
	pop();
}
