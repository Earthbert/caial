FROM nixos/nix

WORKDIR /project
RUN git clone https://github.com/Earthbert/rocket-chip
WORKDIR /project/rocket-chip
RUN git submodule update --init --recursive

RUN nix develop --extra-experimental-features nix-command --extra-experimental-features flakes --command true

RUN echo '#!/bin/sh' > /project/entrypoint.sh && \
    echo 'cd /project/rocket-chip' >> /project/entrypoint.sh && \
    echo 'exec nix develop --extra-experimental-features nix-command --extra-experimental-features flakes --command sh -c "cd /project/caial; exec sh"' >> /project/entrypoint.sh && \
    chmod +x /project/entrypoint.sh

WORKDIR /project/caial


ENV SHELL=/project/entrypoint.sh

VOLUME [ "/project/caial" ]

ENTRYPOINT [ "/bin/sh", "-c" ]
CMD [ "if [ -t 0 ]; then $SHELL; else exec \"$@\"; fi" ]