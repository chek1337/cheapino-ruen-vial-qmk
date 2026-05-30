FROM qmkfm/qmk_cli:latest

ENV QMK_HOME=/qmk
WORKDIR /qmk

RUN git config --system --add safe.directory '*'
