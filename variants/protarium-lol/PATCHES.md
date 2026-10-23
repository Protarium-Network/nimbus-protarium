# Nimbus Protarium patches

Luma3DS IPS patches used by the "protarium" variant of Nimbus (the 3DS Pretendo patcher). `ips/` holds the patch files
as shipped, `ips_dump.py` lists their records. What each one does, read from the patch data:

| File (title ID) | Effect |
|---|---|
| `000400300000BC02/BD02/BE02` | Code patch for the three regional Miiverse applets (JPN/USA/EUR; same patch in each): endpoint domain becomes `.protarium.lol/v1/endpoint`, the CA path becomes `3ds/protaverse.pem`, and ISRG Root X1 (1391 bytes) is embedded. Also three ARM code hunks and small byte edits whose purpose I did not analyse. |
| `0004013000003202` | Friends module: NASC base URL becomes `protarium.lol/ac/` (two occurrences). |
| `0004013000003802` | Account module: `https://account.nintendo.net/v1/api/` becomes `https://account.protarium.lol/v1/api/`. |
| `0004013000002E02` | Replaces `nncs1/nncs2.app.nintendowifi.net` with `nncs1/nncs2.app.protarium.lol`, plus ARM glue to use them. |
| `0004013000002902` | A domain remap table: `ninja.ctr.shop.nintendo.net` -> `ninja.ctr.shp.protarium.lol`, and `nintendowifi.net`, `nintendo.net`, `pokemon-gl.com` -> `protarium.lol` (suffix replacement), with the ARM code that applies it. |
| `0004013000002F02` | 24 bytes of ARM code at 0x6c56, not analysed. |

`protaverse.pem` is the CA the Miiverse patch points at, `juxt-prod.pem` the stock Pretendo one.

Status: read from the binary patches only. The ARM hunks are not disassembled here, so treat the descriptions of what
the code does as inferred from the strings around it.
