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

## protarium-online (test)

`protarium-online/` is the `protarium/` set pointed at `protarium.online` instead of `protarium.lol`. It is generated, not hand edited:

    python variants/make_online_variant.py

The script rewrites the NASC url (friends), the account url (act), the replacement domain of the http module and the
Miiverse endpoint, and asserts that each new string fits where the old one was (it fits exactly for the NASC url and the
Miiverse endpoint). Not changed: the nncs patch (2E02, still `nncs*.app.protarium.lol`) and the eShop rule inside the http
patch (still maps to `ninja.ctr.shp.protarium.lol`).

Build a test app that installs it and uses `.online` for the Azahar rules:

    make PROTARIUM_VARIANT=protarium-online PROTARIUM_DOMAIN=protarium.online APP_TITLE="Nimbus Protarium (online test)"

The http patch grows by 4 bytes at the end of its code cave; that is the one change that is not proven to fit, check it first on a console.
