#!/usr/bin/env python
import sys

print('''<!doctype html>
<html lang="en">
	<head>
		<title>''' + sys.argv[1] + '''</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<style>
			img {
				-ms-interpolation-mode: nearest-neighbor;
				image-rendering: -moz-crisp-edges;
				image-rendering: pixelated;
				width: ''' + sys.argv[2] + '''px;
				height: ''' + sys.argv[2] + '''px;
			}
		</style>
	</head>
	<body>
		<a href="results">View raw results</a>''')

lastVariationCount = ''
for line in sys.stdin:
	[variationCount, file] = line.split()
	if lastVariationCount != variationCount:
		if lastVariationCount != '': print('		</section>')
		print('		<section>')
		print('			<h1>' + variationCount + '</h1>')
		lastVariationCount = variationCount
	print('			<img src="' + file + '.png">')

print('''		</section>
	</body>
</html>''')
