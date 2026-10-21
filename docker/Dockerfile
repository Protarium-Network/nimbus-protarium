FROM devkitpro/devkitarm:latest

RUN apt-get update && apt-get install -y --no-install-recommends curl unzip ca-certificates zip git make && rm -rf /var/lib/apt/lists/*

# libctru, citro2d, citro3d, tex3ds, 3dsxtool, smdhtool...
RUN dkp-pacman -Syu --noconfirm --disable-download-timeout && dkp-pacman -S --noconfirm --needed --disable-download-timeout libctru citro2d citro3d tex3ds 3dstools picasso 3ds-pkg-config devkitARM && dkp-pacman -Scc --noconfirm

# CIA packaging tools (prebuilt Linux releases)
RUN curl -fsSL -o /tmp/makerom.zip https://github.com/3DSGuy/Project_CTR/releases/download/makerom-v0.18.4/makerom-v0.18.4-ubuntu_x86_64.zip \
 && unzip -o /tmp/makerom.zip -d /usr/local/bin && chmod +x /usr/local/bin/makerom \
 && curl -fsSL -o /tmp/bannertool.zip https://github.com/diasurgical/bannertool/releases/download/1.2.0/bannertool.zip \
 && unzip -o /tmp/bannertool.zip -d /tmp/bt && find /tmp/bt -path '*linux-x86_64*' -name bannertool -exec cp {} /usr/local/bin/bannertool \; \
 && chmod +x /usr/local/bin/bannertool && rm -rf /tmp/makerom.zip /tmp/bannertool.zip /tmp/bt

ENV DEVKITPRO=/opt/devkitpro DEVKITARM=/opt/devkitpro/devkitARM
ENV PATH=/opt/devkitpro/tools/bin:/opt/devkitpro/devkitARM/bin:$PATH
WORKDIR /work
