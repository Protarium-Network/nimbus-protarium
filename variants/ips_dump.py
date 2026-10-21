#!/usr/bin/env python3
"""Print the records of an IPS patch: offset, length, and the text when the data is printable."""
import sys

def records(path):
	d = open(path, 'rb').read()
	assert d[:5] == b'PATCH', 'not an IPS file'
	i = 5
	while d[i:i + 3] != b'EOF':
		off, size = int.from_bytes(d[i:i + 3], 'big'), int.from_bytes(d[i + 3:i + 5], 'big')
		i += 5
		if size == 0:  # RLE record
			run, val = int.from_bytes(d[i:i + 2], 'big'), d[i + 2]
			i += 3
			yield off, bytes([val]) * run
		else:
			yield off, d[i:i + size]
			i += size

if __name__ == '__main__':
	for path in sys.argv[1:]:
		print('==', path)
		for off, data in records(path):
			text = data.decode('latin1') if all(32 <= c < 127 or c == 0 for c in data) else None
			print('  0x%06x  %4d bytes  %s' % (off, len(data), repr(text) if text is not None else data[:16].hex() + ('...' if len(data) > 16 else '')))
