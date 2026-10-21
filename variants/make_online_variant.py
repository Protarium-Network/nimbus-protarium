#!/usr/bin/env python3
"""Build variants/protarium-online/ from variants/protarium/: same patches, domain protarium.lol -> protarium.online.

Only the strings that point at the Protarium servers are rewritten; every size is checked against the room the
patch has (see the notes per module). The nncs patch (2E02), the ssl patch (2F02) and the CA are copied as they are.

    python variants/make_online_variant.py
"""
import os
import shutil
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(HERE, 'protarium')
DST = os.path.join(HERE, 'protarium-online')
OLD, NEW = b'protarium.lol', b'protarium.online'

sys.path.insert(0, HERE)
from ips_dump import records  # noqa: E402


def write_ips(path, recs):
    out = bytearray(b'PATCH')
    for offset, data in recs:
        assert len(data) < 0x10000 and offset < 0x1000000
        out += offset.to_bytes(3, 'big') + struct.pack('>H', len(data)) + data
    out += b'EOF'
    open(path, 'wb').write(out)


def swap(data, old, new, what, room):
    """replace old by new inside data; the bytes that follow (up to `room` in total) must stay inside the patch's room"""
    assert data.count(old) == 1, f'{what}: expected exactly one {old!r}'
    result = data.replace(old, new)
    assert len(result) <= room, f'{what}: {len(result)} bytes do not fit in {room}'
    return result


os.makedirs(DST, exist_ok=True)

# friends (NASC): "https://nasc." + host + "/ac/" in two 34-39 byte slots. "protarium.online/ac/" is exactly as long as
# the "nintendowifi.net/ac/" it replaces, so the first slot (34 bytes with the prefix) is full but fits.
recs = []
for offset, data in records(os.path.join(SRC, '0004013000003202.ips')):
    recs.append((offset, swap(data, b'protarium.lol/ac/', b'protarium.online/ac/', 'friends NASC url', 21)))
write_ips(os.path.join(DST, '0004013000003202.ips'), recs)

# act (account): the patch lives in a 152 byte area, the new url is 3 bytes longer and is the last string of the patch
recs = [(o, swap(d, b'account.protarium.lol/', b'account.protarium.online/', 'act account url', 152)) for o, d in records(os.path.join(SRC, '0004013000003802.ips'))]
write_ips(os.path.join(DST, '0004013000003802.ips'), recs)

# http: the replacement string is the last thing of a 991 byte code cave; the patch measures it with strlen at run time.
# Needs 4 more bytes after the old end (the string itself is not NUL terminated in the old patch: the NUL was cave padding).
recs = []
for offset, data in records(os.path.join(SRC, '0004013000002902.ips')):
    if data.endswith(b'pokemon-gl.com\x00' + OLD):
        data = data[:-len(OLD)] + NEW + b'\x00'
    recs.append((offset, data))
write_ips(os.path.join(DST, '0004013000002902.ips'), recs)

# miiverse (JPN/USA/EUR): ".protarium.lol/v1/endpoint" overwrites ".olv.nintendo.net/v1/endpoint" (29 chars + NUL = 30):
# ".protarium.online/v1/endpoint" is 29 chars + NUL, a perfect fit.
for title in ['000400300000BC02', '000400300000BD02', '000400300000BE02']:
    recs = []
    for offset, data in records(os.path.join(SRC, f'{title}.ips')):
        if data.startswith(b'.protarium.lol/v1/endpoint'):
            data = swap(data, b'.protarium.lol/v1/endpoint', b'.protarium.online/v1/endpoint', 'miiverse endpoint', 30)
            assert len(data) == 30
        recs.append((offset, data))
    write_ips(os.path.join(DST, f'{title}.ips'), recs)

for name in ['0004013000002E02.ips', '0004013000002F02.ips', 'protaverse.pem']:
    shutil.copy(os.path.join(SRC, name), os.path.join(DST, name))

print('wrote', DST)
