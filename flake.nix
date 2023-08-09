{
  description = "C/C++ environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils, ... }@inputs:
    utils.lib.eachDefaultSystem (
      system:
      let
        p = import nixpkgs { inherit system; };
      in
      {

        devShell = p.mkShell.override
          {
            stdenv = p.clangStdenv;
          }
          rec {
            packages = with p; [

              # debugger
              #llvm.lldb
              gdb
              gcc
              # fix headers not found
              clang-tools

              # LSP and compiler
              #llvm.libstdcxxClang

              valgrind
            ];
            name = "C";
          };
      }
    );
}
