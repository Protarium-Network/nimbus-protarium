# Build environment

`Dockerfile` builds an image with devkitARM, libctru, citro2d/citro3d, tex3ds, makerom 0.18.4 and bannertool: enough to build the app
(the patches need decompressed `code.bin` files and the plugin needs CTRPluginFramework, neither is in this image).

    docker build -t nimbus-build docker
    docker run --rm -v "$PWD:/work" nimbus-build sh -c 'git config --global --add safe.directory "*"; make -C app 3dsx cia'

Build from a checkout with LF line endings (`git -c core.autocrlf=false clone ...`): GNU make chokes on CRLF Makefiles.

Test build for protarium.online, with its own title id so it installs next to the regular app:

    make -C app 3dsx cia RSF_FILE=meta/nimbus-cia-online-test.rsf PROTARIUM_VARIANT=protarium-online PROTARIUM_DOMAIN=protarium.online APP_TITLE="Nimbus Protarium (online test)"
