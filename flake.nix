{
  description = "A high-performance MaxSAT solver";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
  };
  outputs =
    {
      self,
      nixpkgs,
    }:
    let
      inherit (nixpkgs) lib;
      systems = lib.intersectLists lib.systems.flakeExposed lib.platforms.linux;
      forAllSystems = lib.genAttrs systems;
      nixpkgsFor = forAllSystems (system: nixpkgs.legacyPackages.${system});
      fs = lib.fileset;

      evalmaxsat-package =
        {
          stdenv,
          cmake,
          zlib,
        }:
        stdenv.mkDerivation {
          name = "evalmaxsat";
          src = fs.toSource {
            root = ./.;
            fileset = fs.unions [
              ./lib
              ./main
              ./CMakeLists.txt
            ];
          };

          nativeBuildInputs = [ cmake ];
          buildInputs = [ zlib ];

          postPatch = ''
            # Enable building the main executable
            sed -i 's|# add_subdirectory(main)|add_subdirectory(main)|' CMakeLists.txt
          '';
        };
    in
    {
      packages = forAllSystems (
        system:
        let
          evalmaxsat = nixpkgsFor.${system}.callPackage evalmaxsat-package {
          };
        in
        {
          inherit evalmaxsat;
          default = evalmaxsat;
        }
      );
    };
}
