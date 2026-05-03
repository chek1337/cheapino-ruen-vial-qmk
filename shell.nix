{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "vial-qmk-ergoheaven";

  buildInputs = with pkgs; [
    qmk                 # QMK CLI (python wrapper) + propagated python deps (milc, jsonschema, hjson, ...)
    gcc-arm-embedded    # ARM cross-compiler for RP2040 / ChibiOS
    gnumake             # `qmk compile` shells out to make
    git                 # submodule + version checks during build
    dfu-util            # only needed if you ever flash via DFU; safe to keep
  ];

  shellHook = ''
    export QMK_HOME="$PWD"
    echo "QMK_HOME=$QMK_HOME"
  '';
}
