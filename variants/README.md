# Network patch sets

`3ds/nimbus/variants/<network>/` on the SD card holds the Luma patches Nimbus Protarium installs when a network is selected:

| Directory | Content |
|---|---|
| `pretendo/` | The stock Pretendo patches + `juxt-prod.pem` (built from `patches/`) |
| `protarium/` | The Protarium patches + `protaverse.pem` |
| `nintendo/` | (not shipped) selecting Nintendo just removes the installed patches |

Files are named after the module they patch: `0004013000003202.ips` (friends), `...3802` (act), `...2902`, `...2E02`, `...2F02`
go to `/luma/sysmodules/<id>.ips`; `000400300000BC02/BD02/BE02.ips` (Miiverse JPN/USA/EUR) go to `/luma/titles/<id>/code.ips`.

The Protarium `.ips` files here are the binary patches as released. Their sources are not in this repository yet, only a description
of what they change: see `variants/protarium/PATCHES.md`.
