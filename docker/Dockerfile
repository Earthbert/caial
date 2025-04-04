FROM nixos/nix

RUN nix-env -iA nixpkgs.bash && \
    mkdir -p /bin && \
    ln -s "$(which bash)" /bin/bash

WORKDIR /project
RUN git clone https://github.com/Earthbert/rocket-chip
WORKDIR /project/rocket-chip
RUN git submodule update --init --recursive

RUN nix develop --extra-experimental-features nix-command --extra-experimental-features flakes --command true

SHELL ["/root/.nix-profile/bin/nix", "develop", "--extra-experimental-features", "nix-command", "--extra-experimental-features", "flakes", "--command", "/bin/bash", "-c"]

RUN mill emulator[freechips.rocketchip.system.TestHarness,freechips.rocketchip.system.DefaultConfig].verilator.elf

RUN echo '#!/bin/bash' > /project/entrypoint.sh && \
    echo 'cd /project/rocket-chip' >> /project/entrypoint.sh && \
    echo 'exec nix develop --extra-experimental-features nix-command --extra-experimental-features flakes --command sh -c "cd /project/caial; exec bash"' >> /project/entrypoint.sh && \
    chmod +x /project/entrypoint.sh

WORKDIR /project/caial

ENV SHELL=/project/entrypoint.sh

VOLUME [ "/project/caial" ]

ENTRYPOINT [ "/bin/bash", "-c" ]
CMD [ "if [ -t 0 ]; then $SHELL; else exec \"$@\"; fi" ]
