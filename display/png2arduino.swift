#!/usr/bin/swift

import Cocoa

if CommandLine.argc < 2 {
	print("Usage: \(CommandLine.arguments[0]) emotion.png")
	exit(1)
}

guard let image = NSImage(byReferencingFile: CommandLine.arguments[1]) else {
	print("Provided file is not an image")
	exit(1)
}
let imageRep = image.representations[0] as! NSBitmapImageRep
let pathURL = URL(fileURLWithPath: CommandLine.arguments[1])
let name = pathURL.lastPathComponent.components(separatedBy: ".")[0].replacingOccurrences(of: "-", with: "_")

print("void \(name)() {\n    ", terminator: "")
for x in 0..<Int(imageRep.pixelsWide) {
	for y in 0..<Int(imageRep.pixelsHigh) {
		let color = imageRep.colorAt(x: x, y: y)!
		if color.redComponent < 0.5 && color.greenComponent < 0.5 && color.blueComponent < 0.5 && color.alphaComponent > 0.5 {
			//print("\(x),\(y) -> \(color.redComponent), \(color.greenComponent), \(color.blueComponent), \(color.alphaComponent)")
			print("lcd.setPixel(\(x), \(y), 1); ", terminator: "");
		}
	}
}
print("\n}\n")