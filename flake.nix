{
  description = "A Nix flake C dev environment";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs = { self, nixpkgs }:
    let
      overlays = [
        (final: prev: rec { })
      ];
      supportedSystems = [ "x86_64-linux" ];
      forEachSupportedSystem = f: nixpkgs.lib.genAttrs supportedSystems (system: f {
        pkgs = import nixpkgs { inherit overlays system; };
      });
    in

    {
      devShells = forEachSupportedSystem ({ pkgs }: {
        default = pkgs.mkShell {
          packages = with pkgs; [
            gcc
            ffmpeg
            (python310.withPackages (ps: with ps; [ pygments ])) # needed for minted building tex files

            (pkgs.texlive.combine {
              inherit (pkgs.texlive)
                scheme-medium

                beamer
                minted
                currfile
                bbold
                pgfplots
                adjustbox
                filehook
                stmaryrd
                mathtools
                algorithms
                algorithmicx
                siunitx
                caption
                booktabs
                numprint
                cancel
                wrapfig
                mdframed
                zref
                needspace
                ;
            })
          ];
        };
      });
    };
}
